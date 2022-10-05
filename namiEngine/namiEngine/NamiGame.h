#pragma once

#include "WinApp.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "GameScene.h"
#include "FbxLoader.h"
#include "PostEffect.h"

class NamiGame {
public:
	//������
	void Initialize();
	//�I��
	void Finalize();
	//���t���[���X�V
	void Update();
	//�`��
	void Draw();
	//�Q�b�^�[
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