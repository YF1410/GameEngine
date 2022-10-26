#pragma once
#include "SceneInterface.h"

class SceneManager
{
public:
	SceneManager();

	void Initialize(SceneInterface* scene);

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

