#include "TackleEnemy.h"

TackleEnemy::TackleEnemy()
{
	HP = 5;
	defaultColor = { 0,0,1,1 };
	SetColor(defaultColor);
	tackleRange.center = XMLoadFloat3(&position);
	tackleRange.radius = 30.0f;
}

TackleEnemy::~TackleEnemy()
{
}

std::unique_ptr<TackleEnemy> TackleEnemy::Create(Player* player, Camera* camera)
{
	std::unique_ptr<TackleEnemy> enemy = std::make_unique<TackleEnemy>();

	if (enemyModel == nullptr) {
		enemyModel = FbxLoader::GetInstance()->LoadModelFromFile("ZR");
	}

	enemy->SetModel(enemyModel.get());
	enemy->Initialize(player, camera);

	return enemy;
}

void TackleEnemy::Update()
{
	BaseEnemy::Update();

	if (isShotRange) {
		shotInterval--;
	}

	if (shotInterval <= 0) {
		shotInterval = 60;
	}

	for (std::unique_ptr<Bullet>& bulletObj : bullet) {
		bulletObj->EnemyBulletUpdate(camera);
	}
	shotRange.center = XMLoadFloat3(&position);
}

void TackleEnemy::Move()
{
	if (Collision::CheckSphere2Sphere(player->GetReceiveDamageCollision(), tackleRange)) {
		isTackleRange = true;
	}
	else {
		isTackleRange = false;
		shotInterval = 60;
	}

	XMFLOAT3 pos = player->GetPosition();
	float rad = atan2(pos.z - position.z, pos.x - position.x);
	if (!isDamage && !isTackleRange) {
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
