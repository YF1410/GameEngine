#pragma once
#include "BaseEnemy.h"
class ElementEnemy : public BaseEnemy
{
public:
	static std::unique_ptr<ElementEnemy> Create(Player* player,Camera*camera);
public:
	//�R���X�g���N�^
	ElementEnemy();
	//�f�X�g���N�^
	~ElementEnemy();
};

