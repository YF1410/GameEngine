#pragma once
#include "Framework.h"

#include "Object3d.h"
#include "FbxObject3d.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "PostEffect.h"

class NamiGame : public Framework {
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
	SceneManager* scene_;
	std::unique_ptr<PostEffect> postEffect;
};