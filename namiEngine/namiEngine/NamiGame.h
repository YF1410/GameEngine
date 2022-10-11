#pragma once
#include "Framework.h"

#include "GameScene.h"
#include "PostEffect.h"

class NamiGame : public Framework {
private:
	static const int debugTextTexNumber = 0;
public:
	//������
	void Initialize() override;
	//�I��
	void Finalize() override;
	//���t���[���X�V
	void Update() override;
	//�`��
	void Draw() override;
private:
	std::unique_ptr<GameScene> gameScene;
	std::unique_ptr<PostEffect> postEffect;
};