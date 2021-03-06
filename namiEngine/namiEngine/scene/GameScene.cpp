#include "GameScene.h"
#include "FbxLoader.h"

#include <cassert>
#include <sstream>
#include <iomanip>
#include "imgui.h"

using namespace DirectX;

GameScene::GameScene() {
}

GameScene::~GameScene() {
	safe_delete(object1);
	safe_delete(model1);
	safe_delete(spriteBG);
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio) {
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	// カメラ生成
	cameraObject = new DebugCamera(WinApp::window_width, WinApp::window_height, input);

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
	cameraObject->SetTarget({ 0, 2.5f, 0 });
	cameraObject->SetDistance(50.0f);

	// モデル名を指定してファイル読み込み
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");
	model2 = FbxLoader::GetInstance()->LoadModelFromFile("cube");

	// デバイスをセット
	FbxObject3d::SetDevice(dxCommon->GetDevice());
	// カメラをセット
	FbxObject3d::SetCamera(cameraObject);
	// グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	object1 = new FbxObject3d;
	object1->Initialize();
	object1->SetModel(model1);
	object1->SetRotation({ 0,90.0f,0 });
	object1->PlayAnimation();

	object2 = new FbxObject3d;
	object2->Initialize();
	object2->SetModel(model2);
	object2->SetRotation({ 10.0f,20.0f,0 });
}

void GameScene::Update() {
	cameraObject->Update();
	particleMan->Update();
	//object1->PlayAnimation();
	/*if (input->PushPad(ButtonA) || input->TriggerKey(DIK_P)) {
		object1->PlayAnimation();
	}
	if (input->PushPad(ButtonB) || input->TriggerKey(DIK_L)) {
		object1->StopAnimation();
	}*/

	object2->SetPosition({ object2Pos[0], object2Pos[1], object2Pos[2] });

	//object1->Update();
	object2->Update();
}

void GameScene::Draw() {

	/*object2Pos[0] = object2->GetPosition().x;
	object2Pos[1] = object2->GetPosition().y;
	object2Pos[2] = object2->GetPosition().z;*/

	//ImGui::Begin("cube");
	//ImGui::SetWindowPos(ImVec2(0, 0));
	//ImGui::SetWindowSize(ImVec2(500, 200));
	//ImGui::InputFloat3("cubePos", object2Pos);
	////ImGui::SliderFloat3("object2Pos", object2Pos,-1000,1000);
	////ImGui::DragFloat3("object2Pos", object2Pos);
	//ImGui::End();

	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	//// 背景スプライト描画前処理
	//Sprite::PreDraw(cmdList);
	//// 背景スプライト描画
	//spriteBG->Draw();

	///// <summary>
	///// ここに背景スプライトの描画処理を追加できる
	///// </summary>

	//// スプライト描画後処理
	//Sprite::PostDraw();
	//// 深度バッファクリア
	//dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D描画
	//object1->Draw(cmdList);
	object2->Draw(cmdList);

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