#pragma once
#include "FbxObject3d.h"

class BaseEnemy : public FbxObject3d
{
public:
	BaseEnemy();
	~BaseEnemy();
	void SelectAction();
	void Sweep();
	void Stab();
	void Tackle();
private:
	int HP;
	int Element;
};

