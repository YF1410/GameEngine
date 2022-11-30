#include "ElementEnemy.h"

ElementEnemy::ElementEnemy()
{
	haveElement = true;
	defaultColor = { 1,0,0,1 };
	moveAmount = 0.3f;
	SetColor(defaultColor);
}

ElementEnemy::~ElementEnemy()
{
}

std::unique_ptr<ElementEnemy> ElementEnemy::Create(FbxModel* fbxmodel, Player* player,Camera*camera) {
	std::unique_ptr<ElementEnemy> enemy = std::make_unique<ElementEnemy>();

	enemy->SetModel(fbxmodel);
	enemy->Initialize(player,camera);

	return enemy;
}