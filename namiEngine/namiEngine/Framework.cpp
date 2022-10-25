#include "Framework.h"

void Framework::Run()
{
	Initialize();

	while (true)  // ゲームループ
	{
		Update();

		//終了処理が入ったら
		if (isEnd_) {
			break;
		}

		Draw();
	}

	Finalize();
}

void Framework::Initialize() {
	win = std::make_unique<WinApp>();
	dxCommon = std::make_unique<DirectXCommon>();
	audio = std::make_unique<Audio>();
	// ゲームウィンドウの作成
	win->CreateGameWindow();

	//DirectX初期化処理
	dxCommon->Initialize(win->GetHwnd());

	//入力の初期化
	input = Input::GetInstance();
	input->Initialize(win->GetInstance(), win->GetHwnd());

	// オーディオの初期化
	if (!audio->Initialize()) {
		assert(0);
	}
}

void Framework::Finalize() {
	// ゲームウィンドウの破棄
	win->TerminateGameWindow();
}

void Framework::Update() {
	// メッセージ処理
	if (win->ProcessMessage()) {
		isEnd_ = true;
		return;
	}

	// 入力関連の毎フレーム処理
	input->Update();
}