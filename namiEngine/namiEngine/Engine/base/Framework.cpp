#include "Framework.h"

void Framework::Run()
{
	Initialize();

	while (true)  // �Q�[�����[�v
	{
		Update();

		//�I����������������
		if (isEnd_) {
			break;
		}

		Draw();
	}

	Finalize();
}

void Framework::Initialize() {
	win = std::make_unique<WinApp>();
	// �Q�[���E�B���h�E�̍쐬
	win->CreateGameWindow();

	//DirectX����������
	DirectXCommon::GetInstance()->Initialize(win->GetHwnd());

	//���͂̏�����
	Input::GetInstance()->Initialize(win->GetInstance(), win->GetHwnd());

	// �I�[�f�B�I�̏�����
	if (!Audio::GetInstance()->Initialize()) {
		assert(0);
	}
}

void Framework::Finalize() {
	// �Q�[���E�B���h�E�̔j��
	win->TerminateGameWindow();
}

void Framework::Update() {
	// ���b�Z�[�W����
	if (win->ProcessMessage() || Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
		isEnd_ = true;
		return;
	}

	// ���͊֘A�̖��t���[������
	Input::GetInstance()->Update();
}