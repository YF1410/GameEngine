#pragma once
#include "FbxObject3d.h"
#include "CollisionPrimitive.h"


class BaseEnemy : public FbxObject3d
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	static std::unique_ptr<BaseEnemy> Create(FbxModel* fbxmodel);
public:
	BaseEnemy();
	~BaseEnemy();
	void Initialize();
	void RestartInitialize();
	void Update();
	void SelectAction();
	void Sweep();
	void Stab();
	void Tackle();
	void SetSavePos() { savePos = position; }
	void SetShakePos() { position = savePos; }
	void SetDefaultPos() { position = defaultPos; }
	void SetIsDamage(bool isDamage) { this->isDamage = isDamage; }
	XMFLOAT3 GetSavePos() { return savePos; }
	bool GetIsActive() { return isActive; }
	Sphere GetCollision() { return collision; }
	void Damage(XMFLOAT3 pos,int DamageQuantity);
	void SetHP(int HP) { this->HP = HP; }
private:
	int HP = 3;
	XMFLOAT3 savePos;
	bool isDamage = false;
	bool isActive = true;
	int damageShakeCount = 0;
	float shakeObjectPos[3];
	float moveX = 0;
	float moveZ = 0;
	XMFLOAT3 defaultPos = {0,-5.0f,20.0f};
	Sphere collision;
};

