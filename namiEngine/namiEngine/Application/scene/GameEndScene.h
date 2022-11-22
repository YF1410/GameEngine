#pragma once

#include "Input.h"
#include "Sprite.h"
#include "SceneInterface.h"

#include <memory>

class GameEndScene : public SceneInterface
{
public:
	void Initialize();
	void Finalize();
	void Update();
	void Draw();
private:
	std::unique_ptr<Sprite> spriteBG2;
};

