#include "TitleScene.h"

void TitleScene::Initialize()
{
	spriteBG1 = Sprite::Create(1, { 0.0f,0.0f });
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_1) || Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		
	}
}

void TitleScene::Draw()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �w�i�X�v���C�g�`��
	spriteBG1->Draw();

	/// <summary>
	/// �����ɔw�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	DirectXCommon::GetInstance()->ClearDepthBuffer();
}