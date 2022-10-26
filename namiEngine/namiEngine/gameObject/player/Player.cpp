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
	input = Input::GetInstance();
	collision.center = XMLoadFloat3(&position);
	collision.radius = 3.0f;

	colliderVisualizationModel = Model::CreateFromObject("SphereCollider");
	colliderVisualizationObject = Object3d::Create(colliderVisualizationModel.get());
	colliderVisualizationObject->SetPosition(position);
	colliderVisualizationObject->SetScale(collision.radius);
	colliderVisualizationObject->SetColor({ 1,1,1,0.1f });
}

void Player::Update() {
	collision.center = XMLoadFloat3(&position);
	if (!isAttack) {
		collision.radius = 3.0f;
	}
	colliderVisualizationObject->SetPosition(position);
	colliderVisualizationObject->SetScale(collision.radius);
	colliderVisualizationObject->Update();
	FbxObject3d::Update();	

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

	SetPosition({ playerPos.x + xMoveAmount, playerPos.y,playerPos.z + zMoveAmount });

	if (isReceivedDamage) {
		damageTimer--;
		if (damageTimer <= 0) {
			damageTimer = 60;
			isReceivedDamage = false;
		}
	}

	if (HP <= 0) {
		isActive = false;
	}
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList) {
	FbxObject3d::Draw(cmdList);
	Object3d::PreDraw(cmdList);
	colliderVisualizationObject->Draw();
	Object3d::PostDraw();
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

	colliderVisualizationObject->SetPosition({ position.x,position.y - collision.radius,position.z });
	colliderVisualizationObject->SetScale(collision.radius);
	colliderVisualizationObject->Update();
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