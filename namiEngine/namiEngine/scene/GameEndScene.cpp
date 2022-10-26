#include "GameEndScene.h"

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
	/*if (input->TriggerKey(DIK_1) || input->TriggerKey(DIK_SPACE)) {
		playerPos = { 0.0f,0.0f,0.0f };
		cameraEye[0] = 0.0f;
		cameraEye[1] = 20.0f;
		cameraEye[2] = -50.0f;
		cameraTarget = { 0.0f,0.0f,0.0f };
		xMoveAmount = 0.0f;
		zMoveAmount = 0.0f;
		for (std::unique_ptr<BaseEnemy>& enemyObj : enemy) {
			enemyObj->RestartInitialize();
		}
		playerStatus.HP = 3;
		playerStatus.isHaveElement = false;
		playerStatus.isDash = false;
		playerStatus.isAttack = false;
		player->StopAnimation();

		isNowEnd = false;
		isNowTitle = true;
	}*/
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
