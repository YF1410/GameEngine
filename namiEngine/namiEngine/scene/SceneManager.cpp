#include "SceneManager.h"

SceneManager::SceneManager() :scene_(new GameScene)
{
}

void SceneManager::Initialize()
{
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
	delete this->scene_;
	scene_ = scene;
}
