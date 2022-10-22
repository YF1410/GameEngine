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
	//コンストラクタ
	BaseEnemy();
	//デストラクタ
	~BaseEnemy();
	//初期化
	void Initialize();
	//再初期化
	void RestartInitialize();
	//更新
	void Update();
	/*void SelectAction();
	void Sweep();
	void Stab();
	void Tackle();*/
	//ゲッター
	bool GetIsActive() { return isActive; }
	Sphere GetCollision() { return collision; }
	//セッター
	void SetIsDamage(bool isDamage) { this->isDamage = isDamage; }
	//ダメージ処理
	void Damage(XMFLOAT3 pos,int DamageQuantity);
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

