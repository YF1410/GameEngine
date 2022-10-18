#include "BaseEnemy.h"
#include <DirectXMath.h>

BaseEnemy::BaseEnemy()
{
}

BaseEnemy::~BaseEnemy()
{
}

std::unique_ptr<BaseEnemy> BaseEnemy::Create(FbxModel* fbxModel) {
	std::unique_ptr<BaseEnemy> enemy = std::make_unique<BaseEnemy>();

	enemy->Initialize();
	enemy->SetModel(fbxModel);

	return enemy;
}

void BaseEnemy::Initialize() {
	FbxObject3d::Initialize();
	HP = 3;
	savePos = position;
	isDamage = false;
	isActive = true;
	damageShakeCount = 0;
	moveX = 0;
	moveZ = 0;
	defaultPos = { 0,-5.0f,-20.0f };
	position = defaultPos;
	collision.center = XMLoadFloat3(&position);
	collision.radius = 3.0f;
}

void BaseEnemy::RestartInitialize() {
	HP = 3;
	position = defaultPos;
	savePos = position;
	isDamage = false;
	isActive = true;
	damageShakeCount = 0;
	for (int i = 0; i < 3; i++) {
		shakeObjectPos[i] = 0;
	}
	moveX = position.x;
	moveZ = position.z;
}

void BaseEnemy::Update() {
	FbxObject3d::Update();
	collision.center = XMLoadFloat3(&position);
}

void BaseEnemy::Damage(XMFLOAT3 pos, int DamageQuantity) {
	float rad = atan2(pos.z - position.z, pos.x - position.x);
	if (isDamage) {
		damageShakeCount++;
		position = savePos;
		for (int i = 0; i < 3; i++) {
			shakeObjectPos[i] = static_cast<float>(rand() % 4 - 2);
		}
		if (damageShakeCount >= 10) {
			damageShakeCount = 0;
			for (int i = 0; i < 3; i++) {
				shakeObjectPos[i] = 0;
			}
			HP -= DamageQuantity;
			isDamage = false;
			SetColor({ 1,1,1,1 });
		}
	}
	else {
		savePos = position;
		moveX = (float)(cos(rad) * 0.1f + position.x);
		moveZ = (float)(sin(rad) * 0.1f + position.z);
		if (HP <= 0) {
			isActive = false;
			/*isEnemeyActive = false;
			isElementActive = true;*/
		}
	}
	position = { moveX + shakeObjectPos[0], position.y + shakeObjectPos[1], moveZ + shakeObjectPos[2] };
}

//void BaseEnemy::SelectAction()
//{
//	/*if () {
//		Sweep();
//	}
//	else if () {
//		Stab();
//	}
//	else if () {
//		Tackle();
//	}*/
//}
//
//void BaseEnemy::Sweep()
//{
//}
//
//void BaseEnemy::Stab()
//{
//}
//
//void BaseEnemy::Tackle()
//{
//}
