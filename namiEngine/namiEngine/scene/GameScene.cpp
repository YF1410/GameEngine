#include "GameScene.h"
#include "FbxLoader.h"

#include <cassert>
#include <sstream>
#include <iomanip>
#include "imgui.h"
#include "Collision.h"

using namespace DirectX;

GameScene::GameScene() {
}

GameScene::~GameScene() {
	safe_delete(object1);
	safe_delete(object2);
	safe_delete(object3);
	safe_delete(spriteBG);
}

void GameScene::Initialize(std::shared_ptr<DirectXCommon> dxCommon, std::shared_ptr <Input> input, std::shared_ptr <Audio> audio) {
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	// カメラ生成
	cameraObject = new Camera(WinApp::window_width, WinApp::window_height);

	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
		return;
	}
	// デバッグテキスト初期化
	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(1, L"Resources/background.png")) {
		assert(0);
		return;
	}
	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	// パーティクルマネージャ生成
	particleMan = ParticleManager::GetInstance();
	particleMan->SetCamera(cameraObject);

	// カメラ注視点をセット
	cameraObject->SetTarget(cameraTarget);
	cameraObject->SetEye({ cameraEye[0],cameraEye[1],cameraEye[2] });

	// モデル名を指定してファイル読み込み
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("Walking");
	model2 = FbxLoader::GetInstance()->LoadModelFromFile("cube");
	model3 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	// デバイスをセット
	FbxObject3d::SetDevice(dxCommon->GetDevice());
	// カメラをセット
	FbxObject3d::SetCamera(cameraObject);
	// グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	object1 = new FbxObject3d;
	object1->Initialize();
	object1->SetModel(model1);

	object2 = new FbxObject3d;
	object2->Initialize();
	object2->SetModel(model2);
	object2->SetScale({ 0.3f,0.3f,0.3f });

	object3 = new FbxObject3d;
	object3->Initialize();
	object3->SetModel(model3);
}

void GameScene::Update() {
	//object1->PlayAnimation();
	if (input->PushPad(ButtonA) || input->TriggerKey(DIK_P)) {
		object1->PlayAnimation();
	}
	if (input->PushPad(ButtonB) || input->TriggerKey(DIK_L)) {
		object1->StopAnimation();
	}

	if (input->TriggerKey(DIK_SPACE)) {
		isDash = true;
	}

	if (!isDash) {
		Move(defMoveAmount);
	}
	else if (isDash) {
		Move(dashMoveAmount);
		dashTimer--;
		if (dashTimer <= 0) {
			dashTimer = 8;
			isDash = false;
		}
	}

	cameraObject->SetEye({ cameraEye[0] + xMoveAmount, cameraEye[1],cameraEye[2] + zMoveAmount });
	cameraObject->SetTarget({cameraTarget.x + xMoveAmount, cameraTarget.y,cameraTarget.z + zMoveAmount});

	object1->SetPosition({ object1Pos.x + xMoveAmount, object1Pos.y,object1Pos.z + zMoveAmount });
	object2->SetPosition({ object2Pos[0], object2Pos[1], object2Pos[2] });
	object3->SetPosition({ object3Pos[0], object3Pos[1], object3Pos[2] });


	object1Collision.center = XMLoadFloat3(&object1->GetPosition());
	object1Collision.radius = 3.0f;

	object2Collision.center = XMLoadFloat3(&object2->GetPosition());
	object2Collision.radius = 3.0f;

	object3Collision.center = XMLoadFloat3(&object3->GetPosition());
	object3Collision.radius = 1.0f;

	if (Collision::CheckSphere2Sphere(object1Collision, object2Collision) && isObject2Active) {
		isObject2Active = false;
		isObject3Active = true;
	}

	if (Collision::CheckSphere2Sphere(object1Collision, object3Collision) && isObject3Active) {
		isObject3Active = false;
		object1->PlayAnimation();
	}

	if (input->TriggerKey(DIK_R)) {
		object1Pos = { 0.0f,0.0f,0.0f };
		object2Pos[0] = 0;
		object2Pos[1] = -5.0f;
		object2Pos[2] = 20.0f;
		object3Pos[0] = 10.0f;
		object3Pos[1] = 0;
		object3Pos[2] = 20.0f;
		cameraEye[0] = 0.0f;
		cameraEye[1] = 50.0f;
		cameraEye[2] = -20.0f;
		cameraTarget = { 0.0f,0.0f,0.0f };
		xMoveAmount = 0.0f;
		zMoveAmount = 0.0f;
		isObject2Active = true;
		isObject3Active = false;
		object1->StopAnimation();
	}

	cameraObject->Update();
	object1->Update();
	if (isObject2Active) {
		object2->Update();
	}

	if (isObject3Active) {
		object3->Update();
	}
	particleMan->Update();
}

void GameScene::Draw() {

	object2Pos[0] = object2->GetPosition().x;
	object2Pos[1] = object2->GetPosition().y;
	object2Pos[2] = object2->GetPosition().z;

	//ImGui::Begin("cube");
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
	spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D描画
	object1->Draw(cmdList);
	if (isObject2Active) {
		object2->Draw(cmdList);
	}

	if (isObject3Active) {
		object3->Draw(cmdList);
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

void GameScene::Move(float moveAmount) {
	if (input->PushKey(DIK_W)) {
		zMoveAmount += moveAmount;
		object1->SetRotation({ 0.0f,0.0f,0.0f });
		if (input->PushKey(DIK_A)) {
			xMoveAmount -= moveAmount;
			object1->SetRotation({ 0.0f,315.0f,0.0f });
		}
		else if (input->PushKey(DIK_D)) {
			xMoveAmount += moveAmount;
			object1->SetRotation({ 0.0f,45.0f,0.0f });
		}
	}
	else if (input->PushKey(DIK_S)) {
		zMoveAmount -= moveAmount;
		object1->SetRotation({ 0.0f,180.0f,0.0f });
		if (input->PushKey(DIK_A)) {
			xMoveAmount -= moveAmount;
			object1->SetRotation({ 0.0f,235.0f,0.0f });
		}
		else if (input->PushKey(DIK_D)) {
			xMoveAmount += moveAmount;
			object1->SetRotation({ 0.0f,135.0f,0.0f });
		}
	}
	else if (input->PushKey(DIK_A)) {
		xMoveAmount -= moveAmount;
		object1->SetRotation({ 0.0f,270.0f,0.0f });
	}
	else if (input->PushKey(DIK_D)) {
		xMoveAmount += moveAmount;
		object1->SetRotation({ 0.0f,90.0f,0.0f });
	}
}