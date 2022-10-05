#pragma once

#include "WinApp.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "GameScene.h"
#include "FbxLoader.h"
#include "PostEffect.h"

class NamiGame {
public:
	//初期化
	void Initialize();
	//終了
	void Finalize();
	//毎フレーム更新
	void Update();
	//描画
	void Draw();
	//ゲッター
	bool GetIsEnd() { return isEnd_; }

private:
	bool isEnd_ = false;
	std::unique_ptr<WinApp> win;
	std::unique_ptr<DirectXCommon> dxCommon;
	std::unique_ptr<Input> input;
	std::unique_ptr<Audio> audio;
	std::unique_ptr<GameScene> gameScene;
	std::unique_ptr<PostEffect> postEffect;
};