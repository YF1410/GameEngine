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
	dxCommon = std::make_unique<DirectXCommon>();
	audio = std::make_unique<Audio>();
	// �Q�[���E�B���h�E�̍쐬
	win->CreateGameWindow();

	//DirectX����������
	dxCommon->Initialize(win->GetHwnd());

	//���͂̏�����
	input = Input::GetInstance();
	input->Initialize(win->GetInstance(), win->GetHwnd());

	// �I�[�f�B�I�̏�����
	if (!audio->Initialize()) {
		assert(0);
	}
}

void Framework::Finalize() {
	// �Q�[���E�B���h�E�̔j��
	win->TerminateGameWindow();
}

void Framework::Update() {
	// ���b�Z�[�W����
	if (win->ProcessMessage()) {
		isEnd_ = true;
		return;
	}

	// ���͊֘A�̖��t���[������
	input->Update();
}