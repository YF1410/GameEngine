﻿#pragma once

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "ParticleManager.h"
#include "DebugText.h"
#include "Audio.h"
#include "DebugCamera.h"
#include "Object3d.h"
#include "FbxObject3d.h"
#include "CollisionPrimitive.h"

#include "BaseEnemy.h"
#include "ElementObject.h"
#include "Player.h"
#include "SceneInterface.h"
#include "BaseCollider.h"

#include <vector>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public SceneInterface {
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 静的メンバ変数
	static const int debugTextTexNumber = 0;
public: // メンバ関数

	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	void Finalize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	//void DamageShake(bool isDamageShake,BaseEnemy* enemy);

private: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText* debugText = nullptr;

	bool isNowTitle = true;
	bool isNowGame = false;
	bool isNowEnd = false;


	/// <summary>
	/// ゲームシーン用
	/// </summary>
	std::unique_ptr <Camera> cameraObject;
	ParticleManager* particleMan;

	std::unique_ptr<FbxModel> playerModel;
	std::unique_ptr<Player> player;

	std::unique_ptr<FbxModel> enemyModel;
	std::list<std::unique_ptr<BaseEnemy>> enemy;

	std::unique_ptr<FbxModel> elementModel;
	std::list<std::unique_ptr<ElementObject>> element;

	std::unique_ptr<Model> groundModel;
	std::unique_ptr<Object3d> groundObj;

	std::unique_ptr<Model> skydomeModel;
	std::unique_ptr<Object3d> skydomeObj;
	std::unique_ptr<LightGroup> lightGroup;

	float cameraEye[3] = { 0.0f,20.0f,-50.0f };
	XMFLOAT3 cameraTarget = { 0.0f,0.0f,0.0f };
	Sphere cameraCollider;
	Sphere skydomeCollider;
};