#include "NamiGame.h"

void NamiGame::Initialize() {
	win = std::make_unique<WinApp>();
	dxCommon = std::make_unique<DirectXCommon>();
	input = std::make_unique<Input>();
	audio = std::make_unique<Audio>();
	gameScene = std::make_unique<GameScene>();
	postEffect = std::make_unique<PostEffect>();
	// �Q�[���E�B���h�E�̍쐬
	win->CreateGameWindow();

	//DirectX����������
	dxCommon->Initialize(win->GetHwnd());

#pragma region �ėp�@�\������

	//���͂̏�����
	input->Initialize(win->GetInstance(), win->GetHwnd());

	// �I�[�f�B�I�̏�����
	if (!audio->Initialize()) {
		assert(0);
	}
	// �X�v���C�g�ÓI������
	if (!Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height)) {
		assert(0);
	}

	// 3D�I�u�W�F�N�g�ÓI������
	Object3d::StaticInitialize(dxCommon->GetDevice());
	// ���C�g�ÓI������
	LightGroup::StaticInitialize(dxCommon->GetDevice());
	// �p�[�e�B�N���}�l�[�W��������
	ParticleManager::GetInstance()->Initialize(dxCommon->GetDevice());
	//FBX������
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());

#pragma endregion �ėp�@�\������

	// �Q�[���V�[���̏�����
	gameScene->Initialize(dxCommon.get(), input.get(), audio.get());

	//�|�X�g�G�t�F�N�g�p�̃e�N�X�`���̓ǂݍ���
	//Sprite::LoadTexture(100, L"Resources/white1x1.png");
	//�|�X�g�G�t�F�N�g�̏�����
	postEffect->Initialize();

}

void NamiGame::Finalize() {
	gameScene.release();
	//���
	// �e����
	FbxLoader::GetInstance()->Finalize();
	ParticleManager::GetInstance()->Finalize();

	// �Q�[���E�B���h�E�̔j��
	win->TerminateGameWindow();
}

void NamiGame::Update() {
	// ���b�Z�[�W����
	if (win->ProcessMessage()) {
		isEnd_ = true;
		return;
	}

#pragma region DirectX���t���[������

	// ���͊֘A�̖��t���[������
	input->Update();
	// �Q�[���V�[���̖��t���[������
	gameScene->Update();
}

void NamiGame::Draw() {
	//�����_�[�e�N�X�`���ւ̕`��
	//postEffect->PreDrawScene(dxCommon->GetCommandList());
	//gameScene->Draw();
	//postEffect->PostDrawScene(dxCommon->GetCommandList());
#pragma region �O���t�B�b�N�X�R�}���h
	// �`��J�n
	dxCommon->PreDraw();

	//�|�X�g�G�t�F�N�g�̕`��
	//postEffect->Draw(dxCommon->GetCommandList());

	// �Q�[���V�[���̕`��
	gameScene->Draw();

	// �`��I��
	dxCommon->PostDraw();

#pragma endregion �O���t�B�b�N�X�R�}���h
}