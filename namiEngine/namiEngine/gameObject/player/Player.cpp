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
	receiveDamageCollision.center = XMLoadFloat3(&position);
	receiveDamageCollision.radius = 3.0f;
	inflictDamageCollision.center = XMLoadFloat3(&position);
	inflictDamageCollision.radius = 15.0f;

	receiveDamageColliderVisualizationModel = Model::CreateFromObject("SphereCollider");
	receiveDamageColliderVisualizationObject = Object3d::Create(receiveDamageColliderVisualizationModel.get());
	receiveDamageColliderVisualizationObject->SetPosition(position);
	receiveDamageColliderVisualizationObject->SetScale(receiveDamageCollision.radius);
	receiveDamageColliderVisualizationObject->SetColor({ 1,0,0,0.3f });

	inflictDamageColliderVisualizationModel = Model::CreateFromObject("SphereCollider");
	inflictDamageColliderVisualizationObject = Object3d::Create(inflictDamageColliderVisualizationModel.get());
	inflictDamageColliderVisualizationObject->SetPosition(position);
	inflictDamageColliderVisualizationObject->SetScale(inflictDamageCollision.radius);
	inflictDamageColliderVisualizationObject->SetColor({ 0,1,0,0.1f });
}

void Player::Update() {
	FbxObject3d::Update();	

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
		SetColor({ 1,0,0,1 });
		damageTimer--;
		if (damageTimer <= 0) {
			damageTimer = 60;
			SetColor({ 1,1,1,1 });
			isReceivedDamage = false;
		}
	}

	if (HP <= 0) {
		isActive = false;
	}
	receiveDamageCollision.center = XMLoadFloat3(&position);
	inflictDamageCollision.center = XMLoadFloat3(&position);
	receiveDamageColliderVisualizationObject->SetPosition(position);
	receiveDamageColliderVisualizationObject->SetScale(receiveDamageCollision.radius);
	receiveDamageColliderVisualizationObject->Update();
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList) {
	FbxObject3d::Draw(cmdList);
	Object3d::PreDraw(cmdList);
	receiveDamageColliderVisualizationObject->Draw();
	if (isAttack) {
		inflictDamageColliderVisualizationObject->Draw();
	}
	Object3d::PostDraw();
}

void Player::Attack()
{
	isAttack = false;

	if (input->TriggerKey(DIK_1) && !isPlay) {
		inflictDamageCollision.radius = 15.0f;
		attackPowor = 1;
		isAttack = true;
		PlayAnimation();
	}

	if (input->TriggerKey(DIK_2) && !isPlay && isHaveElement && !isReceivedDamage) {
		inflictDamageCollision.radius = 50.0f;
		SetColor({ 1,1,1,1 });
		attackPowor = 2;
		isAttack = true;
		isHaveElement = false;
		PlayAnimation();
	}

	inflictDamageColliderVisualizationObject->SetPosition({ position.x,position.y,position.z });
	inflictDamageColliderVisualizationObject->SetScale(inflictDamageCollision.radius);
	inflictDamageColliderVisualizationObject->Update();
}

void Player::Move(float moveAmount) {
	if (!isMapEnd) {
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
	else {
		if (input->PushKey(DIK_W)) {
			zMoveAmount -= moveAmount;
			rotation = { 0.0f,180.0f,0.0f };
			if (input->PushKey(DIK_A)) {
				xMoveAmount += moveAmount;
				rotation = { 0.0f,135.0f,0.0f };
			}
			else if (input->PushKey(DIK_D)) {
				xMoveAmount -= moveAmount;
				rotation = { 0.0f,235.0f,0.0f };
			}
		}
		else if (input->PushKey(DIK_S)) {
			zMoveAmount += moveAmount;
			rotation = { 0.0f,0.0f,0.0f };
			if (input->PushKey(DIK_A)) {
				xMoveAmount += moveAmount;
				rotation = { 0.0f,45.0f,0.0f };
			}
			else if (input->PushKey(DIK_D)) {
				xMoveAmount -= moveAmount;
				rotation = { 0.0f,315.0f,0.0f };
			}
		}
		else if (input->PushKey(DIK_A)) {
			xMoveAmount += moveAmount;
			rotation = { 0.0f,90.0f,0.0f };
		}
		else if (input->PushKey(DIK_D)) {
			xMoveAmount -= moveAmount;
			rotation = { 0.0f,270.0f,0.0f };
		}
	}
}