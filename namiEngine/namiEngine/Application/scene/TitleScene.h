#pragma once

#include "Input.h"
#include "Sprite.h"
#include "SceneInterface.h"

#include <memory>
class TitleScene:public SceneInterface
{
public:
	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;
	void SetIsFadeIn(bool isFadeIn);
private:
	std::unique_ptr<Sprite> titleBG;
	std::unique_ptr<Sprite> tutorial[2];
	std::unique_ptr<Sprite> fadeSprite;
	XMFLOAT4 fadeColor = {1.0f,1.0f,1.0f,1.0f};

	bool isTutorial = false;
	unsigned int tutorialNum = 0;

	bool isFadeOut = false;
	bool isFadeIn = true;
};

