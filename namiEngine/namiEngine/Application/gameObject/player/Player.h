#pragma once
#include "Object3d.h"
#include "FbxObject3d.h"
#include "CollisionPrimitive.h"
#include "input.h"
#include "Vector3.h"

class Player :
	public FbxObject3d {
public:
	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<Player> Create(FbxModel* fbxmodel);


public:

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	void Initialize();

	/// <summary>
	/// ���t���[������
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

	void SetIsReceivedDamage(bool isReceivedDamage) { this->isReceivedDamage = isReceivedDamage; }
	void SetIsHaveElement(bool isHaveElement) { this->isHaveElement = isHaveElement; }
	void SetIsMapEnd(bool isMapEnd) { this->isMapEnd = isMapEnd; }
	void SetIsNowCameraShake(bool isNowCameraShake) { this->isNowCameraShake = isNowCameraShake; }
private:
	Input* input = nullptr;
	XMFLOAT3 playerPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 savePos = { 0.0f,0.0f,0.0f };
	int HP = 3;
	const float chargeMoveAmount = 0.5f;
	const float defMoveAmount = 1.0f;
	const float dashMoveAmount = 3.0f;
	int dashTimer = 8;
	int damageTimer = 60;

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
};