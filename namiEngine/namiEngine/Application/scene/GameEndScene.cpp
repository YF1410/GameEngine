#include "GameEndScene.h"
#include "SceneManager.h"

void GameEndScene::Initialize()
{
	if (!Sprite::LoadTexture(10, L"Resources/gameend.png")) {
		assert(0);
		return;
	}
	endBG = Sprite::Create(10, { 0.0f,0.0f });
	fadeSprite = Sprite::Create(2, { 0.0f,0.0f }, fadeColor);
}

void GameEndScene::Finalize()
{
}

void GameEndScene::Update()
{
	if (isFadeIn) {
		fadeColor.w -= 0.05f;
		fadeSprite->SetColor(fadeColor);
		if (fadeColor.w <= 0.0f) {
			isFadeIn = false;
		}
	}

	if ((Input::GetInstance()->TriggerKey(DIK_1) || Input::GetInstance()->TriggerKey(DIK_SPACE)
		|| Input::GetInstance()->TriggerMouse(MouseButton::LeftButton)) && !isFadeIn) {
		isFadeOut = true;
	}

	if (isFadeOut) {
		fadeColor.w += 0.02f;
		fadeSprite->SetColor(fadeColor);
		if (fadeColor.w >= 1.0f) {
			isFadeOut = false;
			SceneManager::GetInstance()->ToTitleScene();
		}
	}
}

void GameEndScene::Draw()
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
