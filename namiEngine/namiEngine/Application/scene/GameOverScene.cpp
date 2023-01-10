#include "GameOverScene.h"
#include "SceneManager.h"

void GameOverScene::Initialize()
{
	if (!Sprite::LoadTexture(11, L"Resources/gameend.png")) {
		assert(0);
		return;
	}
	endBG = Sprite::Create(11, { 0.0f,0.0f });
	fadeSprite = Sprite::Create(2, { 0.0f,0.0f }, fadeColor);
}

void GameOverScene::Finalize()
{
}

void GameOverScene::Update()
{
	Input* input = Input::GetInstance();
	if (isFadeIn) {
		fadeColor.w -= 0.05f;
		fadeSprite->SetColor(fadeColor);
		if (fadeColor.w <= 0.0f) {
			isFadeIn = false;
		}
	}

	if (input->TriggerKey(DIK_W)) {
		isRetry = true;
	}
	else if (input->TriggerKey(DIK_S)) {
		isRetry = false;
	}

	if ((input->TriggerKey(DIK_1) || input->TriggerKey(DIK_SPACE)) && !isFadeIn) {
		isFadeOut = true;
	}

	if (input->TriggerMouse(MouseButton::LeftButton) && !isFadeIn) {
		XMFLOAT2 mousePos = input->GetMousePosition();
	}

	if (isFadeOut) {
		fadeColor.w += 0.02f;
		fadeSprite->SetColor(fadeColor);
		if (fadeColor.w >= 1.0f) {
			isFadeOut = false;
			if (isRetry) {
				SceneManager::GetInstance()->ToGameScene();
			}
			else if (!isRetry) {
				SceneManager::GetInstance()->ToTitleScene();
			}
		}
	}
}

void GameOverScene::Draw()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �w�i�X�v���C�g�`��
	endBG->Draw();
	fadeSprite->Draw();

	/// <summary>
	/// �����ɔw�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	DirectXCommon::GetInstance()->ClearDepthBuffer();
}
