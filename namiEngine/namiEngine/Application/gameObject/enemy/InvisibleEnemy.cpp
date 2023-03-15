#include "InvisibleEnemy.h"

InvisibleEnemy::InvisibleEnemy()
{
	HP = 2;
	defaultColor = { 1,1,0,1 };
	SetColor(defaultColor);
}

InvisibleEnemy::~InvisibleEnemy()
{
}

std::unique_ptr<InvisibleEnemy> InvisibleEnemy::Create(Player* player, Camera* camera)
{
	std::unique_ptr<InvisibleEnemy> enemy = std::make_unique<InvisibleEnemy>();

	if (enemyModel == nullptr) {
		enemyModel = FbxLoader::GetInstance()->LoadModelFromFile("ZR");
	}

	enemy->SetModel(enemyModel.get());
	enemy->Initialize(player, camera);

	return enemy;
}

void InvisibleEnemy::Update()
{
	if (!isInvisible) {
		shotInterval--;
		if (shotInterval <= 0) {
			shotInterval = 30;
			XMFLOAT3 playerPos = player->GetPosition();
			for (int i = -1; i < 2; i++) {
				bullet.push_back(Bullet::Create(position, { playerPos.x + (i * 10),1.0f,playerPos.z + (i * 10) }, player));
			}
			shotCount++;
			if (shotCount == 3) {
				isInvisible = true;
				shotCount = 0;
				randX = rand() % 15 - 30;
				randZ = rand() % 15 - 30;
			}
		}
	}
	else {
		invisibleTimer--;
		if (invisibleTimer <= 0) {
			isInvisible = false;
			invisibleTimer = 60;
		}
	}
}

void InvisibleEnemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (!isInvisible) {
		FbxObject3d::Draw(cmdList);
	}
	Object3d::PreDraw(cmdList);
	for (std::unique_ptr<Bullet>& bulletObj : bullet) {
		bulletObj->Draw();
	}
	colliderVisualizationObject->Draw();
	Object3d::PostDraw();
}

void InvisibleEnemy::Move()
{
	XMFLOAT3 pos = player->GetPosition();
	float rad = atan2(pos.z - position.z, pos.x - position.x);
	if (!isDamage && isInvisible) {
		savePos = position;
		moveX = randX/10 * moveAmount + position.x;
		moveZ = randZ/10 * moveAmount + position.x;
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