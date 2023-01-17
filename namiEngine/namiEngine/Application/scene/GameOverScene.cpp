#include "GameOverScene.h"
#include "SceneManager.h"

void GameOverScene::Initialize()
{
	if (!Sprite::LoadTexture(12, L"Resources/RetryFromGameOver.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(13, L"Resources/TitleFromGameOver.png")) {
		assert(0);
		return;
	}
	retryFromGameOverBG = Sprite::Create(12, { 0.0f,0.0f });
	titleFromGameOverBG = Sprite::Create(13, { 0.0f,0.0f });
	fadeSprite = Sprite::Create(2, { 0.0f,0.0f }, fadeColor);

	dieModel = FbxLoader::GetInstance()->LoadModelFromFile("Die");
	dieObject = std::make_unique<FbxObject3d>();
	dieObject->Initialize();
	dieObject->SetModel(dieModel.get());
	dieObject->SetPosition({ 0.0f,11.0f,-35.0f });
	dieObject->SetRotation({ 0.0f, 180.0f, 0.0f });
	dieObject->PlayAnimation(true);

	cameraObject = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);
	cameraObject->SetTarget(cameraTarget);
	cameraObject->SetEye({ cameraEye[0],cameraEye[1],cameraEye[2] });
	cameraObject->Update();
	FbxObject3d::SetCamera(cameraObject.get());
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

	if ((input->TriggerKey(DIK_1) || input->TriggerKey(DIK_SPACE) || input->TriggerMouse(MouseButton::LeftButton)) && !isFadeIn) {
		isFadeOut = true;
	}

	/*if (input->TriggerMouse(MouseButton::LeftButton) && !isFadeIn) {
		XMFLOAT2 mousePos = input->GetMousePosition();
	}*/

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
	dieObject->Update();
}

void GameOverScene::Draw()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	if (isRetry) {
		retryFromGameOverBG->Draw();
	}
	else if (!isRetry) {
		titleFromGameOverBG->Draw();
	}
	Sprite::PostDraw();
	// 深度バッファクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();


	dieObject->Draw(cmdList);

	Sprite::PreDraw(cmdList);
	fadeSprite->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
}
