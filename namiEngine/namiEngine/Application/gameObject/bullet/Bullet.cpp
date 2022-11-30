#include "Bullet.h"

std::unique_ptr<Bullet> Bullet::Create(XMFLOAT3 startPos, Player* player)
{
	std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>();

	bullet->Initialize(startPos, player);
	bullet->SetScale(1.0f);
	bullet->SetColor({ 1,1,1,1 });

	return bullet;
}

std::unique_ptr<Bullet> Bullet::Create(XMFLOAT3 startPos, BaseEnemy*enemy)
{
	std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>();

	bullet->Initialize(startPos, enemy);
	bullet->SetScale(1.0f);
	bullet->SetColor({ 1,1,1,1 });

	return bullet;
}

bool Bullet::Initialize(XMFLOAT3 startPos, Player* player)
{
	bulletModel = Model::CreateFromObject("SphereCollider");
	bulletObject = Object3d::Create(bulletModel.get());
	bulletObject->SetPosition(startPos);
	startPosition = startPos;
	endPosition = player->GetPosition();
	collision.radius = 1.0f;
	targetCollision.radius = 3.0f;
	isActive = true;
	this->player = player;

	return true;
}


bool Bullet::Initialize(XMFLOAT3 startPos, BaseEnemy*enemy)
{
	bulletModel = Model::CreateFromObject("SphereCollider");
	bulletObject = Object3d::Create(bulletModel.get());
	bulletObject->SetPosition(startPos);
	startPosition = startPos;
	endPosition = enemy->GetPosition();
	collision.radius = 1.0f;
	targetCollision.radius = 3.0f;
	isActive = true;
	this->enemy = enemy;

	return true;
}

void Bullet::PlayerBulletUpdate()
{
	collision.center = XMLoadFloat3(&startPosition);
	targetCollision.center = XMLoadFloat3(&enemy->GetPosition());
	float rad = atan2(enemy->GetPosition().z - startPosition.z, enemy->GetPosition().x - startPosition.x);

	float moveX = (float)(cos(rad) * 0.5f + startPosition.x);
	float moveZ = (float)(sin(rad) * 0.5f + startPosition.z);

	startPosition = { moveX, 1.0f, moveZ };
	bulletObject->SetPosition(startPosition);
	bulletObject->Update();

	if (Collision::CheckSphere2Sphere(collision, targetCollision)) {
		isActive = false;
		enemy->SetIsDamage(true);
		enemy->SetColor({ 1,0,0,1 });
		enemy->Damage();
	}
}

void Bullet::EnemyBulletUpdate(Camera*camera)
{
	collision.center = XMLoadFloat3(&startPosition);
	XMFLOAT3 pPos = player->GetPosition();
	targetCollision.center = XMLoadFloat3(&pPos);
	float rad = atan2(pPos.z - startPosition.z, pPos.x - startPosition.x);

	float moveX = (float)(cos(rad) * 0.5f + startPosition.x);
	float moveZ = (float)(sin(rad) * 0.5f + startPosition.z);

	startPosition = { moveX, 1.0f, moveZ };
	bulletObject->SetPosition(startPosition);
	bulletObject->Update();

	if (!player->GetIsPlay() && Collision::CheckSphere2Sphere(collision, player->GetInflictDamageCollision())) {
		isActive = false;
	}else if (Collision::CheckSphere2Sphere(collision, player->GetReceiveDamageCollision())) {
		isActive = false;
		player->Damage(1);
		camera->SetShakeFlag(true, 6);
		player->SetIsReceivedDamage(true);
	}
}

void Bullet::Draw()
{
	bulletObject->Draw();
}
