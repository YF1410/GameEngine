#pragma once
#include "BaseEnemy.h"
#include "Bullet.h"

class BulletEnemy : public BaseEnemy
{
public:
	static std::unique_ptr<BulletEnemy> Create(FbxModel* fbxmodel, Player* player,Camera*camera);
public:
	//コンストラクタ
	BulletEnemy();
	//デストラクタ
	~BulletEnemy();

	void Update() override;

	void Draw(ID3D12GraphicsCommandList* cmdList) override;

	void Move() override;

	void SetIsShotRange(bool isShotRange) { this->isShotRange = isShotRange; }
private:
	bool isShotRange = false;
	Sphere shotRange;
	int shotInterval = 60;
	std::list<std::unique_ptr<Bullet>> bullet;
};

