#include "WinApp.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "GameScene.h"
#include "FbxLoader.h"
#include "PostEffect.h"

//# Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//ポインタ置き場
	std::unique_ptr<WinApp> win(new WinApp());
	std::unique_ptr<DirectXCommon> dxCommon(new DirectXCommon());
	std::unique_ptr<Input> input(new Input());
	std::unique_ptr<Audio> audio(new Audio());
	std::unique_ptr<GameScene> gameScene(new GameScene());
	std::unique_ptr<PostEffect> postEffect(new PostEffect());

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
		return 1;
	}
	// スプライト静的初期化
	if (!Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height)) {
		assert(0);
		return 1;
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


	while (true)  // ゲームループ
	{
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

#pragma region DirectX毎フレーム処理

		// 入力関連の毎フレーム処理
		input->Update();
		// ゲームシーンの毎フレーム処理
		gameScene->Update();

		//レンダーテクスチャへの描画
		//postEffect->PreDrawScene(dxCommon->GetCommandList());
		//gameScene->Draw();
		//postEffect->PostDrawScene(dxCommon->GetCommandList());

#pragma endregion DirectX毎フレーム処理

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

	//解放
	// 各種解放
	FbxLoader::GetInstance()->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}