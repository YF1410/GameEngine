#pragma once

#include "Input.h"
#include "Sprite.h"
#include "SceneInterface.h"
#include "FbxObject3d.h"

#include <memory>

class GameOverScene : public SceneInterface
{
public:
	void Initialize();
	void Finalize();
	void Update();
	void Draw();
private:
	std::unique_ptr<Sprite> retryFromGameOverBG;
	std::unique_ptr<Sprite> titleFromGameOverBG;
	std::unique_ptr<Sprite> fadeSprite;
	XMFLOAT4 fadeColor = { 1.0f,1.0f,1.0f,1.0f };
	bool isFadeOut = false;
	bool isFadeIn = true;
	bool isRetry = true;
	std::unique_ptr<FbxModel> dieModel;
	std::unique_ptr<FbxObject3d> dieObject;
	std::unique_ptr<Camera> cameraObject;
	float cameraEye[3] = { 0.0f,0.0f,-10.0f };
	XMFLOAT3 cameraTarget = { 0.0f,0.0f,0.0f };
};

