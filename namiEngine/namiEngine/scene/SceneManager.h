#pragma once

#include "TitleScene.h"
#include "GameScene.h"
#include "GameEndScene.h"

class SceneManager
{
public:
	SceneManager();

	void Initialize();

	void Finalize();

	void Update();

	void Draw();

	void ToTitleScene();

	void ToGameScene();

private:
	void changeScene(SceneInterface* scene);
private:
	SceneInterface* scene_;
};

