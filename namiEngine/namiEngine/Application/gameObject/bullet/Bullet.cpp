#include "Bullet.h"

std::unique_ptr<Model> Bullet::bulletModel;

std::unique_ptr<Bullet> Bullet::Create(XMFLOAT3 startPos,XMFLOAT3 endPos, Player* player)
{
	std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>();

	bullet->Initialize(startPos, endPos, player);
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

bool Bullet::Initialize(XMFLOAT3 startPos,XMFLOAT3 endPos, Player* player)
{
	if (bulletModel == nullptr) {
		bulletModel = Model::CreateFromObject("SphereCollider");
	}
	bulletObject = Object3d::Create(bulletModel.get());
	bulletObject->SetPosition(startPos);
	startPosition = startPos;
	bulletPosX = startPos.x;
	bulletPosZ = startPos.z;
	endPosition = endPos;
	collision.radius = 1.0f;
	isActive = true;
	this->player = player;

	return true;
}


bool Bullet::Initialize(XMFLOAT3 startPos, BaseEnemy*enemy)
{
	if (bulletModel == nullptr) {
		bulletModel = Model::CreateFromObject("SphereCollider");
	}
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
	float rad = atan2(endPosition.z - startPosition.z, endPosition.x - startPosition.x);

	
	XMFLOAT2 moveRad = XMFLOAT2{endPosition.x - startPosition.x,endPosition.z - startPosition.z };
	XMStoreFloat2(&moveRad, XMVector2Normalize(XMLoadFloat2(&moveRad)));

	bulletPosX += moveRad.x * 0.7f;
	bulletPosZ += moveRad.y * 0.7f;

	bulletObject->SetPosition({bulletPosX,1.0f,bulletPosZ});
	bulletObject->Update();

	if (!player->GetIsPlay() && Collision::CheckSphere2Sphere(collision, player->GetInflictDamageCollision())) {
		isActive = false;
	}else if (Collision::CheckSphere2Sphere(collision, player->GetReceiveDamageCollision())) {
		isActive = false;
		if (player->GetHP() > 0 && !player->GetIsReceivedDamage()) {
			player->Damage(1);
			camera->SetShakeFlag(true, 6);
			player->SetIsReceivedDamage(true);
		}
	}
}

void Bullet::Draw()
{
	bulletObject->Draw();
}
