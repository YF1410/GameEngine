#include "NamiGame.h"

void NamiGame::Initialize() {
	Framework::Initialize();
	gameScene = std::make_unique<GameScene>();
	postEffect = std::make_unique<PostEffect>();
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
	//解放
	// 各種解放
	FbxLoader::GetInstance()->Finalize();
	ParticleManager::GetInstance()->Finalize();
	LightGroup::StaticFinalize();
	Object3d::StaticFinalize();
	Sprite::StaticFinalize();
	postEffect.release();
	gameScene.release();
	Framework::Finalize();
}

void NamiGame::Update() {
	Framework::Update();
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