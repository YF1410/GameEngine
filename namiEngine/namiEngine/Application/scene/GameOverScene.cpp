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
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	endBG->Draw();
	fadeSprite->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();
}
