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
private:
	std::unique_ptr<Sprite> spriteBG1;
};

