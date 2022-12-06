#include "BulletEnemy.h"

BulletEnemy::BulletEnemy()
{
	HP = 1.0f;
	defaultColor = { 0,1,0,1 };
	SetColor(defaultColor);
	shotRange.center = XMLoadFloat3(&position);
	shotRange.radius = 30.0f;
}

BulletEnemy::~BulletEnemy()
{
}

std::unique_ptr<BulletEnemy> BulletEnemy::Create(FbxModel* fbxmodel, Player* player,Camera*camera) {
	std::unique_ptr<BulletEnemy> enemy = std::make_unique<BulletEnemy>();

	enemy->SetModel(fbxmodel);
	enemy->Initialize(player,camera);

	return enemy;
}

void BulletEnemy::Update() {
	BaseEnemy::Update();

	if (isShotRange) {
		shotInterval--;
	}

	if (shotInterval <= 0) {
		shotInterval = 60;
		bullet.push_back(Bullet::Create(position, player));
	}

	bullet.remove_if([](std::unique_ptr<Bullet>& bulletObj) {return !bulletObj->GetIsActive(); });

	for (std::unique_ptr<Bullet>& bulletObj : bullet) {
		bulletObj->EnemyBulletUpdate(camera);
	}
	shotRange.center = XMLoadFloat3(&position);
}

void BulletEnemy::Draw(ID3D12GraphicsCommandList* cmdList) {
	FbxObject3d::Draw(cmdList);
	Object3d::PreDraw(cmdList);
	for (std::unique_ptr<Bullet>& bulletObj : bullet) {
		bulletObj->Draw();
	}
	colliderVisualizationObject->Draw();
	Object3d::PostDraw();
}

void BulletEnemy::Move()
{
	if (Collision::CheckSphere2Sphere(player->GetReceiveDamageCollision(), shotRange)) {
		isShotRange = true;
	}
	else {
		isShotRange = false;
		shotInterval = 60;
	}

	XMFLOAT3 pos = player->GetPosition();
	float rad = atan2(pos.z - position.z, pos.x - position.x);
	if (!isDamage && !isShotRange) {
		savePos = position;
		moveX = (float)(cos(rad) * moveAmount + position.x);
		moveZ = (float)(sin(rad) * moveAmount + position.z);
	}

	if (isFirstMove) {
		savePos = position;
		moveX = (float)(cos(rad) * moveAmount + position.x);
		moveZ = (float)(sin(rad) * moveAmount + position.z);
		isFirstMove = false;
	}

	position = { moveX + shakeObjectPos[0], position.y + shakeObjectPos[1], moveZ + shakeObjectPos[2] };
	rotation = { 0,-XMConvertToDegrees(rad) + 90.0f,0 };
}