#pragma once
#include "GameScene.h"

class TutorialScene:public GameScene
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	void Finalize() override;

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;
private:
	int num = 1;
};

