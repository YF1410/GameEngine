#pragma once
#include "Object3d.h"
#include "FbxObject3d.h"
#include "CollisionPrimitive.h"
#include "Player.h"
#include "Collision.h"


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
	static std::unique_ptr<BaseEnemy> Create(FbxModel* fbxmodel, Player* player);
public:
	//コンストラクタ
	BaseEnemy();
	//デストラクタ
	~BaseEnemy();
	//初期化
	void Initialize(Player*player);
	//更新
	virtual void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);
	/*void SelectAction();
	void Sweep();
	void Stab();
	void Tackle();*/
	//ゲッター
	bool GetIsActive() { return isActive; }
	Sphere GetCollision() { return collision; }
	bool GetHaveElement() { return haveElement; }
	//セッター
	void SetIsDamage(bool isDamage) { this->isDamage = isDamage; }
	//ダメージ処理
	virtual void Damage();

	void CheckCollisionToPlayer(Camera*camera);

	virtual void Move();
protected:
	Player* player;
	float HP = 3;
	XMFLOAT3 savePos;
	XMFLOAT4 defaultColor = { 1,1,1,1 };
	bool isDamage = false;
	bool isActive = true;
	bool haveElement = false;
	int damageShakeCount = 0;
	float shakeObjectPos[3];
	float moveX = 0;
	float moveZ = 0;
	XMFLOAT3 defaultPos = {0,-5.0f,20.0f};
	Sphere collision;
	std::unique_ptr<Model> colliderVisualizationModel;
	std::unique_ptr<Object3d> colliderVisualizationObject;
};

