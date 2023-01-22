#include "GameClearScene.h"
#include "SceneManager.h"

void GameClearScene::Initialize()
{
	if (!Sprite::LoadTexture(10, L"Resources/RetryFromGameClear.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(11, L"Resources/TitleFromGameClear.png")) {
		assert(0);
		return;
	}
	retryFromGameClearBG = Sprite::Create(10, { 0.0f,0.0f });
	titleFromGameClearBG = Sprite::Create(11, { 0.0f,0.0f });
	fadeSprite = Sprite::Create(2, { 0.0f,0.0f }, fadeColor);

	clapModel = FbxLoader::GetInstance()->LoadModelFromFile("Clap");
	clapObject = std::make_unique<FbxObject3d>();
	clapObject->Initialize();
	clapObject->SetModel(clapModel.get());
	clapObject->SetPosition({ 0.0f,11.0f,-35.0f });
	clapObject->SetRotation({0.0f, 180.0f, 0.0f});
	clapObject->LoopAnimation();

	cameraObject = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);
	cameraObject->SetTarget(cameraTarget);
	cameraObject->SetEye({ cameraEye[0],cameraEye[1],cameraEye[2] });
	cameraObject->Update();
	FbxObject3d::SetCamera(cameraObject.get());
}

void GameClearScene::Finalize()
{
}

void GameClearScene::Update()
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

	if ((input->TriggerKey(DIK_1) || input->TriggerKey(DIK_SPACE)|| input->TriggerMouse(MouseButton::LeftButton)) && !isFadeIn) {
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

	clapObject->Update();
}

void GameClearScene::Draw()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �w�i�X�v���C�g�`��
	if (isRetry) {
		retryFromGameClearBG->Draw();
	}
	else if (!isRetry) {
		titleFromGameClearBG->Draw();
	}
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	DirectXCommon::GetInstance()->ClearDepthBuffer();


	clapObject->Draw(cmdList);

	Sprite::PreDraw(cmdList);
	fadeSprite->Draw();
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
}
