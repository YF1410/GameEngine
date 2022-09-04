#pragma once
class BaseEnemy
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

