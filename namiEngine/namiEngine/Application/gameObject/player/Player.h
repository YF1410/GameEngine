#pragma once
#include "Object3d.h"
#include "FbxObject3d.h"
#include "CollisionPrimitive.h"
#include "input.h"
#include "Vector3.h"
#include "Bullet.h"
#include "BaseEnemy.h"

class BaseEnemy;
class Bullet;
class GameScene;

class Player :
	public FbxObject3d {
public:
	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<Player> Create(FbxModel* fbxmodel, std::list<std::unique_ptr<BaseEnemy>>* enemy);


public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	void Initialize(std::list<std::unique_ptr<BaseEnemy>>* enemy);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);

	void Attack();

	void Move(Vector3 vec);

	void Damage(int damageQuantity) { HP -= damageQuantity; }

	float GetAttackPowor() { return attackPowor; }

	float GetXMoveAmount() { return xMoveAmount; }
	float GetZMoveAmount() { return zMoveAmount; }
	Sphere GetReceiveDamageCollision() { return receiveDamageCollision; }
	Sphere GetInflictDamageCollision() { return inflictDamageCollision; }
	bool GetIsAttack() { return isAttack; }
	bool GetIsReceivedDamage() { return isReceivedDamage; }
	bool GetIsActive() { return isActive; }
	int GetHP() { return HP; }

	void SetIsReceivedDamage(bool isReceivedDamage) { this->isReceivedDamage = isReceivedDamage; }
	void SetIsHaveElement(bool isHaveElement) { this->isHaveElement = isHaveElement; }
	void SetIsMapEnd(bool isMapEnd) { this->isMapEnd = isMapEnd; }
	void SetIsNowCameraShake(bool isNowCameraShake) { this->isNowCameraShake = isNowCameraShake; }
	void SetDefColor(XMFLOAT4 color) { defColor = color; }
private:
	std::list<std::unique_ptr<BaseEnemy>>* enemy;

	Input* input = nullptr;
	XMFLOAT3 playerPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 savePos = { 0.0f,0.0f,0.0f };
	int HP = 3;
	const float chargeMoveAmount = 0.2f;
	const float defMoveAmount = 0.7f;
	const float dashMoveAmount = 1.5f;
	int dashTimer = 16;
	int damageTimer = 60;

	XMFLOAT4 defColor = { 1,1,1,1 };

	int chargeTimer = 60;
	float attackPowor = 1;
	bool isActive = true;
	bool isHaveElement = false;
	bool isDash = false;
	bool isAttack = false;
	bool isReceivedDamage = false;
	bool isNowCameraShake = false;
	bool isMapEnd = false;
	bool isCharging = false;
	bool isMove = false;
	bool isIdle = false;
	Sphere receiveDamageCollision;
	Sphere inflictDamageCollision;
	float xMoveAmount = 0.0f;
	float zMoveAmount = 0.0f;
	float saveXMoveAmount = 0.0f;
	float saveZMoveAmount = 0.0f;
	std::unique_ptr<Model> receiveDamageColliderVisualizationModel;
	std::unique_ptr<Object3d> receiveDamageColliderVisualizationObject;
	std::unique_ptr<Model> inflictDamageColliderVisualizationModel;
	std::unique_ptr<Object3d> inflictDamageColliderVisualizationObject;
	std::unique_ptr<FbxModel> attackModel;
	std::unique_ptr<FbxModel> moveModel;
	std::unique_ptr<FbxModel> rollModel;
	std::unique_ptr<FbxModel> idleModel;
	std::list<std::unique_ptr<Bullet>> bullet;
	//Mediatorパターン
};