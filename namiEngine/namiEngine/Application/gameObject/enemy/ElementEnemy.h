#pragma once
#include "BaseEnemy.h"
class ElementEnemy : public BaseEnemy
{
public:
	static std::unique_ptr<ElementEnemy> Create(FbxModel* fbxmodel, Player* player);
public:
	//コンストラクタ
	ElementEnemy();
	//デストラクタ
	~ElementEnemy();
};

