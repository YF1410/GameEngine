#include "BulletEnemy.h"

BulletEnemy::BulletEnemy()
{
	HP = 1.0f;
	shotRange.center = XMLoadFloat3(&position);
	shotRange.radius = 30.0f;
}

BulletEnemy::~BulletEnemy()
{
}

std::unique_ptr<BulletEnemy> BulletEnemy::Create(FbxModel* fbxmodel, Player* player) {
	std::unique_ptr<BulletEnemy> enemy = std::make_unique<BulletEnemy>();

	enemy->SetModel(fbxmodel);
	enemy->Initialize(player);

	return enemy;
}

void BulletEnemy::Update() {
	BaseEnemy::Update();
	shotRange.center = XMLoadFloat3(&position);
}

void BulletEnemy::Move()
{
	XMFLOAT3 pos = player->GetPosition();

	if (Collision::CheckSphere2Sphere(player->GetReceiveDamageCollision(), shotRange)) {
		isShotRange = true;
	}
	else {
		isShotRange = false;
	}

	moveX = position.x;
	moveZ = position.z;

	float rad = atan2(pos.z - position.z, pos.x - position.x);
	if (!isDamage && !isShotRange) {
		savePos = position;
		moveX = (float)(cos(rad) * 0.1f + position.x);
		moveZ = (float)(sin(rad) * 0.1f + position.z);
	}
	position = { moveX + shakeObjectPos[0], position.y + shakeObjectPos[1], moveZ + shakeObjectPos[2] };
	rotation = { 0,-XMConvertToDegrees(rad) + 90.0f,0 };
}