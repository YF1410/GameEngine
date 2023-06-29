#include "NamiGame.h"
#include "GameScene.h"
#include "GameOverScene.h"
#include "TitleScene.h"
#include "TutorialScene.h"
#include "AllLoadSprite.h"

void NamiGame::Initialize() {
	Framework::Initialize();
	scene_ = SceneManager::GetInstance();
	//postEffect = std::make_unique<PostEffect>();
	// �X�v���C�g�ÓI������
	if (!Sprite::StaticInitialize(WinApp::window_width, WinApp::window_height)) {
		assert(0);
	}
	AllLoadSprite::AllLoadTexture();

	// 3D�I�u�W�F�N�g�ÓI������
	Object3d::StaticInitialize();
	// FBX�I�u�W�F�N�g�O���t�B�b�N�X�p�C�v���C������
	FbxObject3d::CreateGraphicsPipeline();
	// ���C�g�ÓI������
	LightGroup::StaticInitialize();
	// �p�[�e�B�N���}�l�[�W��������
	ParticleManager::GetInstance()->Initialize();
	//FBX������
	FbxLoader::GetInstance()->Initialize();

#pragma endregion �ėp�@�\������

	// �Q�[���V�[���̏�����
	scene_->Initialize(new GameOverScene);

	//�|�X�g�G�t�F�N�g�p�̃e�N�X�`���̓ǂݍ���
	//Sprite::LoadTexture(100, L"Resources/white1x1.png");
	//�|�X�g�G�t�F�N�g�̏�����
	//postEffect->Initialize();

}

void NamiGame::Finalize() {
	//���
	// �e����
	FbxLoader::GetInstance()->Finalize();
	ParticleManager::GetInstance()->Finalize();
	LightGroup::StaticFinalize();
	FbxObject3d::StaticFinalize();
	Object3d::StaticFinalize();
	Sprite::StaticFinalize();
	//postEffect.reset();
	Framework::Finalize();
}

void NamiGame::Update() {
	Framework::Update();
	// �Q�[���V�[���̖��t���[������
	scene_->Update();
}

void NamiGame::Draw() {
	//�����_�[�e�N�X�`���ւ̕`��
	//postEffect->PreDrawScene(dxCommon->GetCommandList());
	//gameScene->Draw();
	//postEffect->PostDrawScene(dxCommon->GetCommandList());
#pragma region �O���t�B�b�N�X�R�}���h
	// �`��J�n
	DirectXCommon::GetInstance()->PreDraw();

	//�|�X�g�G�t�F�N�g�̕`��
	//postEffect->Draw(dxCommon->GetCommandList());

	// �Q�[���V�[���̕`��
	scene_->Draw();

	// �`��I��
	DirectXCommon::GetInstance()->PostDraw();

#pragma endregion �O���t�B�b�N�X�R�}���h
}