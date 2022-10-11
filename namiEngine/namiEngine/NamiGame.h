#pragma once
#include "Framework.h"

#include "GameScene.h"
#include "PostEffect.h"

class NamiGame : public Framework {
public:
	//初期化
	void Initialize() override;
	//終了
	void Finalize() override;
	//毎フレーム更新
	void Update() override;
	//描画
	void Draw() override;
private:
	std::unique_ptr<GameScene> gameScene;
	std::unique_ptr<PostEffect> postEffect;
};