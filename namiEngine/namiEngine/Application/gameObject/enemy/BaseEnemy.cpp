#include "BaseEnemy.h"
#include <DirectXMath.h>

BaseEnemy::BaseEnemy()
{
}

BaseEnemy::~BaseEnemy()
{
}

std::unique_ptr<BaseEnemy> BaseEnemy::Create(FbxModel* fbxmodel, Player* player,Camera*camera) {
	std::unique_ptr<BaseEnemy> enemy = std::make_unique<BaseEnemy>();

	enemy->SetModel(fbxmodel);
	enemy->Initialize(player,camera);

	return enemy;
}

void BaseEnemy::Initialize(Player*player,Camera*camera) {
	FbxObject3d::Initialize();
	this->player = player;
	this->camera = camera;
	savePos = position;
	isDamage = false;
	isActive = true;
	damageShakeCount = 0;
	moveX = 0;
	moveZ = 0;
	defaultPos = { 0,-5.0f,-20.0f };
	position = defaultPos;
	XMFLOAT3 fixCollisionPos = { position.x,0,position.z };
	collision.center = XMLoadFloat3(&fixCollisionPos);
	collision.radius = 3.0f;

	colliderVisualizationModel = Model::CreateFromObject("SphereCollider");
	colliderVisualizationObject = Object3d::Create(colliderVisualizationModel.get());
	colliderVisualizationObject->SetPosition(fixCollisionPos);
	colliderVisualizationObject->SetScale(collision.radius);
	colliderVisualizationObject->SetColor({ 1,0,0,0.3f });
	LoopAnimation();
}

void BaseEnemy::Update() {
	XMFLOAT3 fixCollisionPos = { position.x,0,position.z };
	collision.center = XMLoadFloat3(&fixCollisionPos);
	colliderVisualizationObject->SetPosition(fixCollisionPos);
	colliderVisualizationObject->SetScale(collision.radius);
	colliderVisualizationObject->Update();
	FbxObject3d::Update();
}

void BaseEnemy::Draw(ID3D12GraphicsCommandList* cmdList) {
	FbxObject3d::Draw(cmdList);
	Object3d::PreDraw(cmdList);
	colliderVisualizationObject->Draw();
	Object3d::PostDraw();
}

void BaseEnemy::Move()
{
	XMFLOAT3 pos = player->GetPosition();
	float rad = atan2(pos.z - position.z, pos.x - position.x);
	if (!isDamage) {
		savePos = position;
		moveX = (float)(cos(rad) * moveAmount + position.x);
		moveZ = (float)(sin(rad) * moveAmount + position.z);
	}
	position = { moveX + shakeObjectPos[0], position.y + shakeObjectPos[1], moveZ + shakeObjectPos[2] };
	rotation = { 0,-XMConvertToDegrees(rad) + 90.0f,0 };
}

void BaseEnemy::Damage() {
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
			HP -= player->GetAttackPowor();
			isDamage = false;
			SetColor(defaultColor);
			if (HP <= 0) {
				isActive = false;
			}
		}
	}
	position = { moveX + shakeObjectPos[0], position.y + shakeObjectPos[1], moveZ + shakeObjectPos[2] };
}

void BaseEnemy::CheckCollisionToPlayer()
{
	if (Collision::CheckSphere2Sphere(player->GetInflictDamageCollision(), collision) && isActive && player->GetIsAttack()) {
		SetColor({ 1,0,0,1 });
		isDamage = true;
	}
	else if (Collision::CheckSphere2Sphere(player->GetReceiveDamageCollision(), collision) && isActive && !player->GetIsAttack() && !player->GetIsReceivedDamage()) {
		player->Damage(1);
		camera->SetShakeFlag(true, 6);
		player->SetIsReceivedDamage(true);
	}
}