#include "GameEndScene.h"
#include "SceneManager.h"

void GameEndScene::Initialize()
{
	if (!Sprite::LoadTexture(2, L"Resources/gameend.png")) {
		assert(0);
		return;
	}
	spriteBG2 = Sprite::Create(2, { 0.0f,0.0f });
}

void GameEndScene::Finalize()
{
}

void GameEndScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_1) || Input::GetInstance()->TriggerKey(DIK_SPACE) || Input::GetInstance()->TriggerMouse(MouseButton::LeftButton)) {
		SceneManager::GetInstance()->ToTitleScene();
	}
}

void GameEndScene::Draw()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �w�i�X�v���C�g�`��
	spriteBG2->Draw();

	/// <summary>
	/// �����ɔw�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	DirectXCommon::GetInstance()->ClearDepthBuffer();
}
