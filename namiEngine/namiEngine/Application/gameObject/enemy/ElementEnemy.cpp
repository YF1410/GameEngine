#include "ElementEnemy.h"

ElementEnemy::ElementEnemy()
{
	haveElement = true;
	defaultColor = { 0,0,1,1 };
	SetColor(defaultColor);
}

ElementEnemy::~ElementEnemy()
{
}

std::unique_ptr<ElementEnemy> ElementEnemy::Create(FbxModel* fbxmodel, Player* player) {
	std::unique_ptr<ElementEnemy> enemy = std::make_unique<ElementEnemy>();

	enemy->SetModel(fbxmodel);
	enemy->Initialize(player);

	return enemy;
}