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
	receiveDamageColliderVisualizationObject->SetPosition(playerPos);
	receiveDamageColliderVisualizationObject->SetScale(receiveDamageCollision.radius);
	receiveDamageColliderVisualizationObject->SetColor({ 1,0,0,0.3f });

	inflictDamageColliderVisualizationModel = Model::CreateFromObject("SphereCollider");
	inflictDamageColliderVisualizationObject = Object3d::Create(inflictDamageColliderVisualizationModel.get());
	inflictDamageColliderVisualizationObject->SetPosition(position);
	inflictDamageColliderVisualizationObject->SetScale(inflictDamageCollision.radius);
	inflictDamageColliderVisualizationObject->SetColor({ 0,1,0,0.1f });
	//inflictDamageColliderVisualizationObject->
}

void Player::Update() {

	if (!isCharging && input->TriggerKey(DIK_SPACE) && (!input->TriggerKey(DIK_W) || !input->TriggerKey(DIK_A) || !input->TriggerKey(DIK_S) || !input->TriggerKey(DIK_D))) {
		isDash = true;
	}

	if (isCharging) {
		chargeTimer--;
	}

	if (chargeTimer <= 0) {
		inflictDamageCollision.radius += 0.1f;
		attackPowor += 0.01;
	}

	if (isDash) {
		dashTimer--;
		if (dashTimer <= 0) {
			dashTimer = 8;
			isDash = false;
		}
	}

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

	if (!isMapEnd) {
		savePos = position;
		saveXMoveAmount = xMoveAmount;
		saveZMoveAmount = zMoveAmount;
	}
	else if (isMapEnd) {
		position = savePos;
		xMoveAmount = saveXMoveAmount;
		zMoveAmount = saveZMoveAmount;
		SetPosition(position);
	}
	receiveDamageCollision.center = XMLoadFloat3(&position);
	inflictDamageCollision.center = XMLoadFloat3(&position);
	receiveDamageColliderVisualizationObject->SetPosition(position);
	receiveDamageColliderVisualizationObject->SetScale(receiveDamageCollision.radius);
	receiveDamageColliderVisualizationObject->Update();
	FbxObject3d::Update();
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList) {
	FbxObject3d::Draw(cmdList);
	Object3d::PreDraw(cmdList);
	receiveDamageColliderVisualizationObject->Draw();
	//if (isAttack) {
	inflictDamageColliderVisualizationObject->Draw();
	//}
	Object3d::PostDraw();
}

void Player::Attack()
{
	isAttack = false;

	if ((input->TriggerKey(DIK_1) || input->TriggerMouse(MouseButton::LeftButton)) && !isPlay) {
		inflictDamageCollision.radius = 15.0f;
		attackPowor = 1;
		isAttack = true;
		PlayAnimation();
	}

	if (input->PushKey(DIK_1) || input->PushMouse(MouseButton::LeftButton)) {
		isCharging = true;
	}
	else if ((input->ReleaseKey(DIK_1) || input->ReleaseMouse(MouseButton::LeftButton)) && chargeTimer >= 1) {
		isCharging = false;
		chargeTimer = 60;
	}
	else if ((input->ReleaseKey(DIK_1) || input->ReleaseMouse(MouseButton::LeftButton)) && chargeTimer <= 0) {
		isAttack = true;
		isCharging = false;
		chargeTimer = 60;
	}

	if ((input->TriggerKey(DIK_2) || input->TriggerMouse(MouseButton::RightButton)) && !isPlay && isHaveElement && !isReceivedDamage) {
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

void Player::Move(Vector3 vec) {
	Vector3 moveVec = XMVector3Normalize(vec);
	float b = XMConvertToDegrees(atan2(vec.z, vec.x));
	//if (!isMapEnd) {
	float moveAmount;
	if (!isDash) {
		moveAmount = defMoveAmount;
	}
	else if (isDash) {
		moveAmount = dashMoveAmount;
	}

	if (isCharging) {
		moveAmount = chargeMoveAmount;
	}

	if (!isNowCameraShake) {
		if (input->PushKey(DIK_W)) {
			xMoveAmount += moveVec.x * moveAmount;
			zMoveAmount += moveVec.z * moveAmount;
			rotation.y = -b + 90.0f;
			if (input->PushKey(DIK_A)) {
				xMoveAmount -= moveVec.z * moveAmount;
				zMoveAmount += moveVec.x * moveAmount;
				rotation.y = -b + 45.0f;
			}
			else if (input->PushKey(DIK_D)) {
				xMoveAmount += moveVec.z * moveAmount;
				zMoveAmount -= moveVec.x * moveAmount;
				rotation.y = -b + 135.0f;
			}
		}
		else if (input->PushKey(DIK_S)) {
			xMoveAmount -= moveVec.x * moveAmount;
			zMoveAmount -= moveVec.z * moveAmount;
			rotation.y = -b -90.0f;
			if (input->PushKey(DIK_A)) {
				xMoveAmount -= moveVec.z * moveAmount;
				zMoveAmount += moveVec.x * moveAmount;
				rotation.y = -b - 45.0f; ;
			}
			if (input->PushKey(DIK_D)) {
				xMoveAmount += moveVec.z * moveAmount;
				zMoveAmount -= moveVec.x * moveAmount;
				rotation.y = -b - 135.0f;
			}
		}
		else if (input->PushKey(DIK_A)) {
			xMoveAmount -= moveVec.z * moveAmount;
			zMoveAmount += moveVec.x * moveAmount;
			rotation.y = -b;
		}
		else if (input->PushKey(DIK_D)) {
			xMoveAmount += moveVec.z * moveAmount;
			zMoveAmount -= moveVec.x * moveAmount;
			rotation.y = -b + 180.0f;
		}
	}

	SetPosition({ playerPos.x + xMoveAmount, playerPos.y,playerPos.z + zMoveAmount });
	//}
	//else {
		/*if (input->PushKey(DIK_W)) {
			zMoveAmount -= moveAmount*4;
			rotation = { 0.0f,180.0f,0.0f };
			if (input->PushKey(DIK_A)) {
				xMoveAmount += moveAmount * 4;
				rotation = { 0.0f,135.0f,0.0f };
			}
			else if (input->PushKey(DIK_D)) {
				xMoveAmount -= moveAmount * 4;
				rotation = { 0.0f,235.0f,0.0f };
			}
		}
		else if (input->PushKey(DIK_S)) {
			zMoveAmount += moveAmount * 4;
			rotation = { 0.0f,0.0f,0.0f };
			if (input->PushKey(DIK_A)) {
				xMoveAmount += moveAmount * 4;
				rotation = { 0.0f,45.0f,0.0f };
			}
			else if (input->PushKey(DIK_D)) {
				xMoveAmount -= moveAmount * 4;
				rotation = { 0.0f,315.0f,0.0f };
			}
		}
		else if (input->PushKey(DIK_A)) {
			xMoveAmount += moveAmount * 4;
			rotation = { 0.0f,90.0f,0.0f };
		}
		else if (input->PushKey(DIK_D)) {
			xMoveAmount -= moveAmount * 4;
			rotation = { 0.0f,270.0f,0.0f };
		}*/
		//}
}