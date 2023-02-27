#include "TitleScene.h"
#include "SceneManager.h"

void TitleScene::Initialize()
{
	if (!Sprite::LoadTexture(1, L"Resources/title.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(2, L"Resources/fadeSprite.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(3, L"Resources/tutorial1.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(4, L"Resources/tutorial2.png")) {
		assert(0);
		return;
	}

	titleBG = Sprite::Create(1, { 0.0f,0.0f });
	fadeSprite = Sprite::Create(2, { 0.0f,0.0f },fadeColor);
	tutorial[0] = Sprite::Create(3, {0.0f,0.0f});
	tutorial[1] = Sprite::Create(4, {0.0f,0.0f});
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	if (isFadeIn) {
		fadeColor.w -= 0.05f;
		fadeSprite->SetColor(fadeColor);
		if (fadeColor.w <= 0.0f) {
			isFadeIn = false;
		}
	}

	if (Input::GetInstance()->TriggerKey(DIK_2) && !isFadeIn) {
		isTutorial = true;
	}

	if ((Input::GetInstance()->TriggerKey(DIK_1) || Input::GetInstance()->TriggerKey(DIK_SPACE) 
		|| Input::GetInstance()->TriggerMouse(MouseButton::LeftButton)) && !isFadeIn) {
		if (isTutorial) {
			tutorialNum++;
		}

		isTutorial = true;

		if (tutorialNum > 1) {
			isTutorial = false;
		}

		if (!isTutorial) {
			isFadeOut = true;
		}
	}

	if (isFadeOut) {
		fadeColor.w += 0.02f;
		fadeSprite->SetColor(fadeColor);
		if (fadeColor.w >= 1.0f) {
			isFadeOut = false;
			SceneManager::GetInstance()->ToGameScene();
			//SceneManager::GetInstance()->ToTutorialScene();
		}
	}
}

void TitleScene::Draw()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	titleBG->Draw();
	if (isTutorial) {
		tutorial[tutorialNum]->Draw();
	}
	fadeSprite->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();
}