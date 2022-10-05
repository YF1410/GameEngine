#include "NamiGame.h"

void NamiGame::Initialize() {
	win = std::make_unique<WinApp>();
	dxCommon = std::make_unique<DirectXCommon>();
	input = std::make_unique<Input>();
	audio = std::make_unique<Audio>();
	gameScene = std::make_unique<GameScene>();
	postEffect = std::make_unique<PostEffect>();
	// ゲームウィンドウの作成
	win->CreateGameWindow();

	//DirectX初期化処理
	dxCommon->Initialize(win->GetHwnd());

#pragma region 汎用機能初期化

	//入力の初期化
	input->Initialize(win->GetInstance(), win->GetHwnd());

	// オーディオの初期化
	if (!audio->Initialize()) {
		assert(0);
	}
	// スプライト静的初期化
	if (!Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height)) {
		assert(0);
	}

	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxCommon->GetDevice());
	// ライト静的初期化
	LightGroup::StaticInitialize(dxCommon->GetDevice());
	// パーティクルマネージャ初期化
	ParticleManager::GetInstance()->Initialize(dxCommon->GetDevice());
	//FBX初期化
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());

#pragma endregion 汎用機能初期化

	// ゲームシーンの初期化
	gameScene->Initialize(dxCommon.get(), input.get(), audio.get());

	//ポストエフェクト用のテクスチャの読み込み
	//Sprite::LoadTexture(100, L"Resources/white1x1.png");
	//ポストエフェクトの初期化
	postEffect->Initialize();

}

void NamiGame::Finalize() {
	gameScene.release();
	//解放
	// 各種解放
	FbxLoader::GetInstance()->Finalize();
	ParticleManager::GetInstance()->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();
}

void NamiGame::Update() {
	// メッセージ処理
	if (win->ProcessMessage()) {
		isEnd_ = true;
		return;
	}

#pragma region DirectX毎フレーム処理

	// 入力関連の毎フレーム処理
	input->Update();
	// ゲームシーンの毎フレーム処理
	gameScene->Update();
}

void NamiGame::Draw() {
	//レンダーテクスチャへの描画
	//postEffect->PreDrawScene(dxCommon->GetCommandList());
	//gameScene->Draw();
	//postEffect->PostDrawScene(dxCommon->GetCommandList());
#pragma region グラフィックスコマンド
	// 描画開始
	dxCommon->PreDraw();

	//ポストエフェクトの描画
	//postEffect->Draw(dxCommon->GetCommandList());

	// ゲームシーンの描画
	gameScene->Draw();

	// 描画終了
	dxCommon->PostDraw();

#pragma endregion グラフィックスコマンド
}