#pragma once
#include "BaseEnemy.h"
class BulletEnemy : public BaseEnemy
{
public:
	static std::unique_ptr<BulletEnemy> Create(FbxModel* fbxmodel, Player* player);
public:
	//�R���X�g���N�^
	BulletEnemy();
	//�f�X�g���N�^
	~BulletEnemy();

	void Update() override;

	void Move() override;

	void SetIsShotRange(bool isShotRange) { this->isShotRange = isShotRange; }
private:
	bool isShotRange = false;
	Sphere shotRange;
	std::unique_ptr<Model> bulletModel;
	std::unique_ptr<Object3d> bulletObject;
};

