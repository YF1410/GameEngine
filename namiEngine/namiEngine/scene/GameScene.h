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

#include <vector>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
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
public:
	struct PlayerStatus {
		int HP = 3;
		const float defMoveAmount = 1.0f;
		const float dashMoveAmount = 3.0f;
		int dashTimer = 8;
		int attackPowor = 1;
		bool isHaveElement = false;
		bool isDash = false;
		bool isAttack = false;
		bool isReceivedDamage = false;
	};
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
	void Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio);

	//タイトル
	void TitleUpdate();

	//ゲームシーン
	void GameUpdate();

	//終了画面
	void EndUpdate();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	//タイトル
	void TitleDraw();

	//ゲームシーン
	void GameDraw();

	//終了画面
	void EndDraw();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void Move(float moveAmount);

	void DamageShake(bool isDamageShake,FbxObject3d* enemy);

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
	std::unique_ptr<Sprite> spriteBG1;
	std::unique_ptr<Sprite> spriteBG2;
	ParticleManager* particleMan;

	std::unique_ptr<FbxModel> playerModel;
	std::unique_ptr<FbxObject3d> player;

	std::unique_ptr<FbxModel> enemyModel;
	std::unique_ptr<BaseEnemy> enemy;

	std::unique_ptr<FbxModel> elementModel;
	std::unique_ptr<FbxObject3d> element;

	std::unique_ptr<Model> groundModel;
	std::unique_ptr<Object3d> groundObj;

	std::unique_ptr<Model> skydomeModel;
	std::unique_ptr<Object3d> skydomeObj;
	std::unique_ptr<LightGroup> lightGroup;

	XMFLOAT3 playerPos = { 0.0f,0.0f,0.0f };
	float enemyPos[3] = { 0,-5.0f,20.0f };
	float elementPos[3] = { 10.0f,0,20.0f };
	float cameraEye[3] = { 0.0f,20.0f,-50.0f };
	XMFLOAT3 cameraTarget = { 0.0f,0.0f,0.0f };
	float xMoveAmount = 0.0f;
	float zMoveAmount = 0.0f;
	XMFLOAT3 savePos;

	Sphere playerCollision;
	Sphere enemyCollision;
	Sphere elementCollision;

	PlayerStatus playerStatus;

	bool isEnemeyActive = true;
	bool isElementActive = false;
	int enemyHP = 3;
	bool isDamageShake = false;
	int damageShakeCount = 0;
	float shakeObjectPos[3] = {0.0f,0.0f,0.0f};

	float enemyMoveX = 0;
	float enemyMoveZ = 0;
};