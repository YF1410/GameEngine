#include "WinApp.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "GameScene.h"
#include "FbxLoader.h"
#include "PostEffect.h"

//# Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//ポインタ置き場
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	GameScene* gameScene = nullptr;
	PostEffect* postEffect = nullptr;

	// ゲームウィンドウの作成
	win = new WinApp();
	win->CreateGameWindow();

	//DirectX初期化処理
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化

	//入力の初期化
	input = new Input();
	input->Initialize(win->GetInstance(), win->GetHwnd());

	// オーディオの初期化
	audio = new Audio();
	if (!audio->Initialize()) 	{
		assert(0);
		return 1;
	}
	// スプライト静的初期化
	if (!Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height)) 	{
		assert(0);
		return 1;
	}

	// パーティクルマネージャ初期化
	ParticleManager::GetInstance()->Initialize(dxCommon->GetDevice());
	//FBX初期化
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());

#pragma endregion 汎用機能初期化


	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize(dxCommon, input, audio);

	//ポストエフェクト用のテクスチャの読み込み
	//Sprite::LoadTexture(100, L"Resources/white1x1.png");
	//ポストエフェクトの初期化
	postEffect = new PostEffect();
	postEffect->Initialize();


	while (true)  // ゲームループ
	{
		// メッセージ処理
		if (win->ProcessMessage()) 		{
			break;
		}

#pragma region DirectX毎フレーム処理

		// 入力関連の毎フレーム処理
		input->Update();
		// ゲームシーンの毎フレーム処理
		gameScene->Update();

		//レンダーテクスチャへの描画
		postEffect->PreDrawScene(dxCommon->GetCommandList());
		gameScene->Draw();
		postEffect->PostDrawScene(dxCommon->GetCommandList());

#pragma endregion DirectX毎フレーム処理

#pragma region グラフィックスコマンド
		// 描画開始
		dxCommon->PreDraw();

		//ポストエフェクトの描画
		postEffect->Draw(dxCommon->GetCommandList());

		// ゲームシーンの描画
		//gameScene->Draw();

		// 描画終了
		dxCommon->PostDraw();

#pragma endregion グラフィックスコマンド
	}

	//解放
	// 各種解放
	safe_delete(gameScene);
	FbxLoader::GetInstance()->Finalize();
	safe_delete(input);
	safe_delete(audio);
	safe_delete(dxCommon);
	safe_delete(postEffect);

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();
	safe_delete(win);

	return 0;
}

//s
