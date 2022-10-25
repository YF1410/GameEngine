#include "GameScene.h"
#include "FbxLoader.h"

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

void GameScene::Initialize(DirectXCommon* dxCommon, Audio* audio) {
	// nullptrチェック
	assert(dxCommon);
	assert(audio);

	this->dxCommon = dxCommon;
	this->audio = audio;

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

	input = Input::GetInstance();

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(1, L"Resources/title.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(2, L"Resources/gameend.png")) {
		assert(0);
		return;
	}
	// 背景スプライト生成

	spriteBG1 = Sprite::Create(1, { 0.0f,0.0f });
	spriteBG2 = Sprite::Create(2, { 0.0f,0.0f });
	// パーティクルマネージャ生成
	particleMan = ParticleManager::GetInstance();
	particleMan->SetCamera(cameraObject.get());

	// カメラ注視点をセット
	cameraObject->SetTarget(cameraTarget);
	cameraObject->SetEye({ cameraEye[0],cameraEye[1],cameraEye[2] });

	// モデル名を指定してファイル読み込み
	playerModel = FbxLoader::GetInstance()->LoadModelFromFile("Walking");
	enemyModel = FbxLoader::GetInstance()->LoadModelFromFile("cube");
	elementModel = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");
	groundModel = Model::CreateFromObject("stage1");
	skydomeModel = Model::CreateFromObject("skydome");
	// ライト生成
	lightGroup = LightGroup::Create();
	// 3Dオブエクトにライトをセット
	Object3d::SetLightGroup(lightGroup.get());
	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(cameraObject.get());

	// カメラをセット
	FbxObject3d::SetCamera(cameraObject.get());

	//object1 = new FbxObject3d;
	player = Player::Create(playerModel.get());

	for (int i = 0; i < 20; i++) {
		enemy.push_back(BaseEnemy::Create(enemyModel.get()));
	}

	for (std::unique_ptr<BaseEnemy>& enemyObj : enemy) {
		/*enemyObj->Initialize();
		enemyObj->SetModel(enemyModel.get());*/
		enemyObj->SetScale({ 0.3f,0.3f,0.3f });
		enemyObj->SetPosition({ static_cast<float>(rand() % 100 - 50),-5,static_cast<float>(rand() % 100 - 50) });
	}

	//enemy.end()->get()->Initialize();

	/*enemy = std::make_unique<BaseEnemy>();
	enemy->Initialize();
	enemy->SetModel(enemyModel.get());
	enemy->SetScale({ 0.3f,0.3f,0.3f });*/

	//element = std::make_unique<ElementObject>();
	//element->Initialize();
	//element->SetModel(elementModel.get());
	//element->SetRotation({ 0,90.0f,0, });
	//element->SetColor({ 0,0,1,1 });
	//element->LoopAnimation();

	groundObj = Object3d::Create(groundModel.get());
	groundObj->SetScale({ 2.0f,2.0f,2.0f });
	skydomeObj = Object3d::Create(skydomeModel.get());
	skydomeObj->SetScale(5.0f);

	//player->SetPosition({ playerPos.x + xMoveAmount, playerPos.y,playerPos.z + zMoveAmount });
	//enemy->SetPosition({ enemyPos[0] + shakeObjectPos[0], enemyPos[1] + shakeObjectPos[1], enemyPos[2] + shakeObjectPos[2] });
	//element->SetPosition({ elementPos[0], elementPos[1], elementPos[2] });

	srand(static_cast<unsigned int>(time(NULL)));
}

void GameScene::TitleUpdate() {
	if (input->TriggerKey(DIK_1) || input->TriggerKey(DIK_SPACE)) {
		isNowTitle = false;
		isNowGame = true;
	}
}

void GameScene::GameUpdate() {

	if (input->TriggerKey(DIK_0)) {
		element.push_back(ElementObject::Create(playerModel.get(),{0,0,-50}));
	}

	for (std::unique_ptr<BaseEnemy>& enemyObj : enemy) {
		enemyObj->Damage(player->GetPosition(), player->GetAttackPowor());
		if (!enemyObj->GetIsActive()) {
			element.push_back(ElementObject::Create(elementModel.get(), enemyObj->GetPosition()));
		}
	}

	enemy.remove_if([](std::unique_ptr<BaseEnemy>& enemyObj) {return !enemyObj->GetIsActive(); });
	element.remove_if([](std::unique_ptr<ElementObject>& elementObj) {return !elementObj->GetIsActive(); });

	cameraObject->SetEye({ cameraEye[0] + player->GetXMoveAmount(), cameraEye[1],cameraEye[2] + player->GetZMoveAmount()});
	cameraObject->SetTarget({ cameraTarget.x + player->GetXMoveAmount(), cameraTarget.y,cameraTarget.z + player->GetZMoveAmount() });

	player->Attack();

	for (std::unique_ptr<BaseEnemy>& enemyObj : enemy) {
		if (Collision::CheckSphere2Sphere(player->GetCollision(), enemyObj->GetCollision()) && enemyObj->GetIsActive() && player->GetIsAttack()) {
			enemyObj->SetColor({ 1,0,0,1 });
			enemyObj->SetIsDamage(true);
		}
		else if (Collision::CheckSphere2Sphere(player->GetCollision(), enemyObj->GetCollision()) && enemyObj->GetIsActive() && !player->GetIsAttack() && !player->GetIsReceivedDamage()) {
			player->Damage(1);
			//player->SetColor({ 1,0,0,1 });
			player->SetIsReceivedDamage(true);
		}
	}

	for (std::unique_ptr<ElementObject>& elementObj : element) {
		elementObj->GetIsActive();
		if (Collision::CheckSphere2Sphere(player->GetCollision(), elementObj->GetCollision()) && elementObj->GetIsActive() && !player->GetIsPlay()) {
			elementObj->SetIsActive(false);

			player->SetColor({ 0,0,1,1 });
			player->SetIsHaveElement(true);
			player->StopAnimation();

			/*isNowEnd = true;
			isNowGame = false;*/
		}
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
	particleMan->Update();
}

void GameScene::EndUpdate() {
	/*if (input->TriggerKey(DIK_1) || input->TriggerKey(DIK_SPACE)) {
		playerPos = { 0.0f,0.0f,0.0f };
		cameraEye[0] = 0.0f;
		cameraEye[1] = 20.0f;
		cameraEye[2] = -50.0f;
		cameraTarget = { 0.0f,0.0f,0.0f };
		xMoveAmount = 0.0f;
		zMoveAmount = 0.0f;
		for (std::unique_ptr<BaseEnemy>& enemyObj : enemy) {
			enemyObj->RestartInitialize();
		}
		playerStatus.HP = 3;
		playerStatus.isHaveElement = false;
		playerStatus.isDash = false;
		playerStatus.isAttack = false;
		player->StopAnimation();

		isNowEnd = false;
		isNowTitle = true;
	}*/
}

void GameScene::Update() {
	if (isNowTitle) {
		TitleUpdate();
	}
	else if (isNowGame) {
		GameUpdate();
	}
	else if (isNowEnd) {
		EndUpdate();
	}
}

void GameScene::TitleDraw() {
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	spriteBG1->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
};

void GameScene::GameDraw() {

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

	player->Draw(cmdList);
	for (std::unique_ptr<BaseEnemy>& enemyObj : enemy) {
		enemyObj->Draw(cmdList);
	}


	for (std::unique_ptr<ElementObject>& elementObj : element) {
		elementObj->Draw(cmdList);
	}

	// パーティクルの描画
	particleMan->Draw(cmdList);
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>


	// デバッグテキストの描画
	debugText->DrawAll();

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void GameScene::EndDraw() {
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	spriteBG2->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
};

void GameScene::Draw() {
	if (isNowTitle) {
		TitleDraw();
	}
	else if (isNowGame) {
		GameDraw();
	}
	else if (isNowEnd) {
		EndDraw();
	}
}

//void GameScene::DamageShake(bool isDamage,FbxObject3d* enemy) {
//	float rad = atan2(player->GetPosition().z - enemy->GetPosition().z, player->GetPosition().x - enemy->GetPosition().x);
//	if (isDamageShake) {
//		damageShakeCount++;
//		enemy->SetPosition(savePos);
//		for (int i = 0; i < 3; i++) {
//			shakeObjectPos[i] = static_cast<float>(rand() % 4 - 2);
//		}
//		if (damageShakeCount >= 10) {
//			damageShakeCount = 0;
//			for (int i = 0; i < 3; i++) {
//				shakeObjectPos[i] = 0;
//			}
//			isDamageShake = false;
//			enemy->SetColor({ 1,1,1,1 });
//		}
//	}
//	else {
//		savePos = enemy->GetPosition();
//		enemyMoveX = (float)(cos(rad) * 0.1f + enemy->GetPosition().x);
//		enemyMoveZ = (float)(sin(rad) * 0.1f + enemy->GetPosition().z);
//		if (enemyHP <= 0) {
//			isEnemeyActive = false;
//			isElementActive = true;
//		}
//	}
//}