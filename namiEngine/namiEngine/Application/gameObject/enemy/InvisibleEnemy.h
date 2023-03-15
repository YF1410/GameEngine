#pragma once
#include "BaseEnemy.h"

class InvisibleEnemy : public BaseEnemy
{
public:
	static std::unique_ptr<InvisibleEnemy> Create(Player* player, Camera* camera);
public:
	//�R���X�g���N�^
	InvisibleEnemy();
	//�f�X�g���N�^
	~InvisibleEnemy();

	void Update() override;

	void Draw(ID3D12GraphicsCommandList* cmdList) override;

	void Move() override;
private:
	int moveInterval = 60;
	int shotInterval = 30;
	int shotCount = 0;
	int invisibleTimer = 60;
	int randX = 0;
	int randZ = 0;
	bool isInvisible = false;
	bool isFirstMove = true;
	std::list<std::unique_ptr<Bullet>> bullet;
};