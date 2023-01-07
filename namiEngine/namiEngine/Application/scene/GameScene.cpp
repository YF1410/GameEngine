﻿#include "GameScene.h"
#include "FbxLoader.h"
#include "SceneManager.h"
#include "Vector3.h"

#include <cassert>
#include <sstream>
#include <iomanip>
#include "imgui.h"
#include "Collision.h"
#include <stdlib.h>
#include <time.h>


using namespace DirectX;

GameScene::GameScene() {
}

GameScene::~GameScene() {
}

void GameScene::Initialize() {
	srand(static_cast<unsigned int>(time(NULL)));
	// nullptrチェック
	dxCommon = DirectXCommon::GetInstance();
	input = Input::GetInstance();
	audio = Audio::GetInstance();

	// カメラ生成
	cameraObject = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);

	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
		return;
	}
	// デバッグテキスト初期化
	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextTexNumber);

	// テクスチャ読み込み

	//スプライト生成
	fadeSprite = Sprite::Create(2, { 0.0f,0.0f }, fadeColor);
	// 背景スプライト生成

	// パーティクルマネージャ生成
	particleMan = ParticleManager::GetInstance();
	particleMan->SetCamera(cameraObject.get());

	// カメラ注視点をセット
	cameraObject->SetTarget(cameraTarget);
	cameraObject->SetEye({ cameraEye[0],cameraEye[1],cameraEye[2] });

	// モデル名を指定してファイル読み込み
	playerModel = FbxLoader::GetInstance()->LoadModelFromFile("Walking");
	elementModel = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");
	groundModel = Model::CreateFromObject("stage1");
	skydomeModel = Model::CreateFromObject("skydome");
	// ライト生成
	lightGroup = LightGroup::Create();
	lightGroup->SetCircleShadowActive(0, true);
	// 3Dオブエクトにライトをセット
	Object3d::SetLightGroup(lightGroup.get());
	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(cameraObject.get());

	// カメラをセット
	FbxObject3d::SetCamera(cameraObject.get());

	player = Player::Create(playerModel.get(), &enemy);

	for (int i = 0; i < 5; i++) {
		enemy.push_back(BaseEnemy::Create(player.get(), cameraObject.get()));
		enemy.push_back(BulletEnemy::Create(player.get(), cameraObject.get()));
	}

	for (int i = 0; i < 2; i++) {
		enemy.push_back(ElementEnemy::Create(player.get(), cameraObject.get()));
	}

	groundObj = Object3d::Create(groundModel.get());
	groundObj->SetScale(8.5f);
	skydomeObj = Object3d::Create(skydomeModel.get());
	skydomeObj->SetScale(5.0f);

	cameraCollider.center = XMLoadFloat3(&cameraObject->GetEye());
	cameraCollider.radius = 5.0f;

	skydomeCollider.center = { 0,0,0 };
	skydomeCollider.radius = 400.0f;


	if (!Sprite::LoadTexture(5, L"Resources/0.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(6, L"Resources/1.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(7, L"Resources/2.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(8, L"Resources/3.png")) {
		assert(0);
		return;
	}

	HP[0] = Sprite::Create(5, { 0.0f,0.0f });
	HP[1] = Sprite::Create(6, { 0.0f,0.0f });
	HP[2] = Sprite::Create(7, { 0.0f,0.0f });
	HP[3] = Sprite::Create(8, { 0.0f,0.0f });
}

void GameScene::Finalize()
{
}

void GameScene::Update() {
	if (isFadeIn) {
		if (updateCount == 0) {
			cameraObject->Update();
			player->Update();
			for (std::unique_ptr<BaseEnemy>& enemyObj : enemy) {
				enemyObj->Update();
			}

			for (std::unique_ptr<ElementObject>& elementObj : element) {
				elementObj->Update();
			}
			groundObj->Update();
			skydomeObj->Update();
			//skydomeCollider.Update();
			particleMan->Update();
			updateCount++;
		}
		fadeColor.w -= 0.01f;
		fadeSprite->SetColor(fadeColor);
		if (fadeColor.w <= 0.0f) {
			isFadeIn = false;
		}
	}
	else if (!isFadeIn && !isFadeOut) {
		XMFLOAT3 pPos = player->GetPosition();
		XMFLOAT3 cPos = cameraObject->GetEye();
		XMFLOAT3 colliderCenter = { (pPos.x + cPos.x) / 2,(pPos.y + cPos.y) / 2 ,(pPos.z + cPos.z) / 2 };
		cameraCollider.center = XMLoadFloat3(&colliderCenter);

		Vector3 a = { pPos.x - cPos.x, pPos.y - cPos.y,pPos.z - cPos.z };

		for (std::unique_ptr<BaseEnemy>& enemyObj : enemy) {
			enemyObj->Move();
			enemyObj->Damage();
			if (!enemyObj->GetIsActive() && enemyObj->GetHaveElement()) {
				element.push_back(ElementObject::Create(elementModel.get(), enemyObj->GetPosition()));
			}
		}

		enemy.remove_if([](std::unique_ptr<BaseEnemy>& enemyObj) {return !enemyObj->GetIsActive(); });
		element.remove_if([](std::unique_ptr<ElementObject>& elementObj) {return !elementObj->GetIsActive(); });

		player->Attack();

		for (std::unique_ptr<BaseEnemy>& enemyObj : enemy) {
			enemyObj->CheckCollisionToPlayer();
		}

		for (std::unique_ptr<ElementObject>& elementObj : element) {
			elementObj->GetIsActive();
			if (Collision::CheckSphere2Sphere(player->GetReceiveDamageCollision(), elementObj->GetCollision()) && elementObj->GetIsActive()) {
				elementObj->SetIsActive(false);

				player->SetDefColor({ 0,0,1,1 });
				player->SetColor({ 0,0,1,1, });
				player->SetIsHaveElement(true);
			}
		}

		player->Move(a);

		lightGroup->SetCircleShadowDir(0, XMLoadFloat3(&circleShadowDir));
		lightGroup->SetCircleShadowCasterPos(0, player->GetPosition());
		lightGroup->SetCircleShadowDistanceCasterLight(0, 450.0f);
		lightGroup->SetCircleShadowAtten(0, circleShadowAtten);
		lightGroup->SetCircleShadowFactorAngle(0, circleShadowFactorAngle);

		if (Collision::CheckSphereInside2Sphere(cameraCollider, skydomeCollider)) {
			player->SetIsMapEnd(false);
			cameraObject->SetEye({ cameraEye[0] + player->GetXMoveAmount(), cameraEye[1],cameraEye[2] + player->GetZMoveAmount() });
			cameraObject->SetTarget({ cameraTarget.x + player->GetXMoveAmount(), cameraTarget.y,cameraTarget.z + player->GetZMoveAmount() });
		}
		else {
			player->SetIsMapEnd(true);
		}

		if (cameraObject->GetShakeFlag()) {
			player->SetIsNowCameraShake(true);
		}
		else if (!cameraObject->GetShakeFlag()) {
			player->SetIsNowCameraShake(false);
		}

		cameraObject->CameraShake();

		if (enemy.empty() || !player->GetIsActive()) {
			isFadeOut = true;
		}

		cameraObject->Update();
		player->Update();
		for (std::unique_ptr<BaseEnemy>& enemyObj : enemy) {
			enemyObj->Update();
		}

		for (std::unique_ptr<ElementObject>& elementObj : element) {
			elementObj->Update();
		}

		groundObj->Update();
		skydomeObj->Update();
		//skydomeCollider.Update();
		particleMan->Update();
		lightGroup->Update();
	}
	else if (isFadeOut) {
		if (isFadeOut) {
			fadeColor.w += 0.02f;
			fadeSprite->SetColor(fadeColor);
			if (fadeColor.w >= 1.0f) {
				isFadeOut = false;
				SceneManager::GetInstance()->ToGameOverScene();
			}
		}
	}
}



void GameScene::Draw() {
	/*object2Pos[0] = object2->GetPosition().x;
	object2Pos[1] = object2->GetPosition().y;
	object2Pos[2] = object2->GetPosition().z;*/

	//ImGui::Begin("window");
	//ImGui::SetWindowPos(ImVec2(0, 0));
	//ImGui::SetWindowSize(ImVec2(500, 200));
	////ImGui::InputFloat3("cubePos", object2Pos);
	////ImGui::SliderFloat3("object2Pos", object2Pos,-1000,1000);
	//ImGui::DragFloat3("object2Pos", object2Pos);
	//ImGui::DragFloat3("cameraEye", cameraEye);
	//ImGui::End();

	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	//spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D描画
	// 3Dオブジェクトの描画
	Object3d::PreDraw(cmdList);
	groundObj->Draw();
	skydomeObj->Draw();
	Object3d::PostDraw();

	for (std::unique_ptr<BaseEnemy>& enemyObj : enemy) {
		enemyObj->Draw(cmdList);
	}


	for (std::unique_ptr<ElementObject>& elementObj : element) {
		elementObj->Draw(cmdList);
	}

	player->Draw(cmdList);
	//lightGroup->Draw(cmdList, 3);

	// パーティクルの描画
	particleMan->Draw(cmdList);
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	HP[player->GetHP()]->Draw();


	fadeSprite->Draw();

	// デバッグテキストの描画
	debugText->DrawAll();

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}