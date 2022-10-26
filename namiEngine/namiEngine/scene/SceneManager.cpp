#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "GameEndScene.h"

SceneManager::SceneManager()
{
}

void SceneManager::Initialize(SceneInterface* scene)
{
	changeScene(scene);
	scene_->Initialize();
}

void SceneManager::Update()
{
	scene_->Update();
}

void SceneManager::Draw()
{
	scene_->Draw();
}

void SceneManager::ToTitleScene()
{

}

void SceneManager::ToGameScene()
{
	changeScene(new GameEndScene);
}

void SceneManager::changeScene(SceneInterface* scene)
{
	if (!scene_) {
		delete scene_;
	}
	scene_ = scene;
}
