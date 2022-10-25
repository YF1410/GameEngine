#include "Player.h"

using namespace DirectX;

std::unique_ptr<Player> Player::Create(FbxModel* fbxmodel) {
	//3Dオブジェクトのインスタンスを生成
	std::unique_ptr<Player> player = std::make_unique<Player>();

	player->Initialize();
	player->SetModel(fbxmodel);

	return player;
}

void Player::Initialize() {
	FbxObject3d::Initialize();
	input=Input::GetInstance();
	collision.center = XMLoadFloat3(&position);
	collision.radius = 3.0f;
}

void Player::Update() {
	FbxObject3d::Update();

	SetPosition({ playerPos.x + xMoveAmount, playerPos.y,playerPos.z + zMoveAmount });

	collision.center = XMLoadFloat3(&position);
	collision.radius = 3.0f;
	isAttack = false;

	if (input->TriggerKey(DIK_SPACE) && (!input->TriggerKey(DIK_W) || !input->TriggerKey(DIK_A) || !input->TriggerKey(DIK_S) || !input->TriggerKey(DIK_D))) {
		isDash = true;
	}

	if (!isDash) {
		Move(defMoveAmount);
	}
	else if (isDash) {
		Move(dashMoveAmount);
		dashTimer--;
		if (dashTimer <= 0) {
			dashTimer = 8;
			isDash = false;
		}
	}
}

void Player::Attack()
{
	if (input->TriggerKey(DIK_1) && !isPlay) {
		collision.radius = 15.0f;
		attackPowor = 1;
		isAttack = true;
		PlayAnimation();
	}

	if (input->TriggerKey(DIK_2) && !isPlay && isHaveElement) {
		collision.radius = 50.0f;
		SetColor({ 1,1,1,1 });
		attackPowor = 2;
		isAttack = true;
		isHaveElement = false;
		PlayAnimation();
	}
}

void Player::Move(float moveAmount) {
	if (input->PushKey(DIK_W)) {
		zMoveAmount += moveAmount;
		rotation = { 0.0f,0.0f,0.0f };
		if (input->PushKey(DIK_A)) {
			xMoveAmount -= moveAmount;
			rotation = { 0.0f,315.0f,0.0f };
		}
		else if (input->PushKey(DIK_D)) {
			xMoveAmount += moveAmount;
			rotation = { 0.0f,45.0f,0.0f };
		}
	}
	else if (input->PushKey(DIK_S)) {
		zMoveAmount -= moveAmount;
		rotation = { 0.0f,180.0f,0.0f };
		if (input->PushKey(DIK_A)) {
			xMoveAmount -= moveAmount;
			rotation = { 0.0f,235.0f,0.0f };
		}
		else if (input->PushKey(DIK_D)) {
			xMoveAmount += moveAmount;
			rotation = { 0.0f,135.0f,0.0f };
		}
	}
	else if (input->PushKey(DIK_A)) {
		xMoveAmount -= moveAmount;
		rotation = { 0.0f,270.0f,0.0f };
	}
	else if (input->PushKey(DIK_D)) {
		xMoveAmount += moveAmount;
		rotation = { 0.0f,90.0f,0.0f };
	}
}