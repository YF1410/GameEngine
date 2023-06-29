#include "GameClearScene.h"
#include "SceneManager.h"
#include "easing.h"
#include "AllLoadSprite.h"

void GameClearScene::Initialize()
{
	AllLoadSprite::AllLoadTexture();
	baseBG = Sprite::Create(1, { 0.0f,0.0f });
	gameclear = Sprite::Create(21, { 0.0f,0.0f });
	selectToRetry[0] = Sprite::Create(22, { 0.0f,0.0f });
	selectToRetry[1] = Sprite::Create(23, { 0.0f,0.0f });
	selectToTitle[0] = Sprite::Create(24, { 0.0f,0.0f });
	selectToTitle[1] = Sprite::Create(25, { 0.0f,0.0f });
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
		fadeColor.w -= 0.04f;
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

	SpecifiedMove();

	if (isFadeOut) {
		fadeColor.w += 0.04f;
		fadeSprite->SetColor(fadeColor);
		if (fadeColor.w >= 1.0f) {
			isFadeOut = false;
			if (isRetry) {
				SceneManager::GetInstance()->ToGameScene(StageManager::GetInstance()->GetStageType());
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

	baseBG->Draw();
	gameclear->Draw();

	// �w�i�X�v���C�g�`��
	if (isRetry) {
		selectToRetry[0]->Draw();
		selectToTitle[1]->Draw();
	}
	else if (!isRetry) {
		selectToRetry[1]->Draw();
		selectToTitle[0]->Draw();
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

void GameClearScene::SpecifiedMove()
{
	if (maxSpecifiedMoveTimer >= specifiedMoveTimer) {
		specifiedMoveTimer++;
		if (maxSpecifiedMoveTimer <= specifiedMoveTimer) {
			specifiedMoveTimer = 0;
			if (!isUp) {
				isUp = true;
			}
			else if (isUp) {
				isUp = false;
			}
		}
	}

	float eTime = (float)(specifiedMoveTimer / static_cast<double>(maxSpecifiedMoveTimer));

	if (isUp) {
		gameClearObjectPosition = static_cast<float>(Ease(In, ease::Quint, eTime, specifiedBouncePosUp, specifiedBouncePosDown));
	}
	else if (!isUp) {
		gameClearObjectPosition = static_cast<float>(Ease(Out, ease::Quint, eTime, specifiedBouncePosDown, specifiedBouncePosUp));
	}

	if (isRetry) {
		gameclear->SetPosition({ 0,gameClearObjectPosition });
		selectToRetry[0]->SetPosition({ 0,gameClearObjectPosition });
		selectToTitle[1]->SetPosition({ 0,defaultObjectPosition });
	}
	else if (!isRetry) {
		gameclear->SetPosition({ 0,gameClearObjectPosition });
		selectToRetry[1]->SetPosition({ 0,defaultObjectPosition });
		selectToTitle[0]->SetPosition({ 0,gameClearObjectPosition });
	}
}