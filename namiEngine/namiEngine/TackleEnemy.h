#pragma once
#include "BaseEnemy.h"


class TackleEnemy : public BaseEnemy
{
public:
	static std::unique_ptr<TackleEnemy> Create(Player* player, Camera* camera);
public:
	//�R���X�g���N�^
	TackleEnemy();
	//�f�X�g���N�^
	~TackleEnemy();

	void Update() override;

	void Move() override;
private:
	bool isFirstMove = true;
	Sphere tackleRange;
	bool isTackleRange;
};
