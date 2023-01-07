#pragma once

#include "Input.h"
#include "Sprite.h"
#include "SceneInterface.h"

#include <memory>

class GameOverScene : public SceneInterface
{
public:
	void Initialize();
	void Finalize();
	void Update();
	void Draw();
private:
	std::unique_ptr<Sprite> endBG;
	std::unique_ptr<Sprite> fadeSprite;
	XMFLOAT4 fadeColor = { 1.0f,1.0f,1.0f,1.0f };
	bool isFadeOut = false;
	bool isFadeIn = true;
};

