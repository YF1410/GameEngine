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
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	spriteBG2->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();
}
