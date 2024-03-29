#pragma once
#include "GameScene.h"

class TutorialScene:public GameScene
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	void Finalize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
private:
	int num = 1;
};

