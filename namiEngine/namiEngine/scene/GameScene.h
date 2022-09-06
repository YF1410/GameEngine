#pragma once

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

	void DamageShake();

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
	Camera* cameraObject = nullptr;
	Sprite* spriteBG1 = nullptr;
	Sprite* spriteBG2 = nullptr;
	ParticleManager* particleMan = nullptr;

	FbxModel* model1 = nullptr;
	std::unique_ptr<FbxObject3d> object1;

	FbxModel* model2 = nullptr;
	std::unique_ptr<FbxObject3d> object2;

	FbxModel* model3 = nullptr;
	std::unique_ptr<FbxObject3d> object3;

	Model* groundModel = nullptr;
	Object3d* groundObj = nullptr;

	Model* skydomeModel = nullptr;
	Object3d* skydomeObj = nullptr;
	LightGroup* lightGroup = nullptr;

	XMFLOAT3 object1Pos = { 0.0f,0.0f,0.0f };
	float object2Pos[3] = { 0,-5.0f,20.0f };
	float object3Pos[3] = { 10.0f,0,20.0f };
	float cameraEye[3] = { 0.0f,20.0f,-50.0f };
	XMFLOAT3 cameraTarget = { 0.0f,0.0f,0.0f };
	float xMoveAmount = 0.0f;
	float zMoveAmount = 0.0f;

	Sphere object1Collision;
	Sphere object2Collision;
	Sphere object3Collision;

	bool isObject2Active = true;
	bool isObject3Active = false;
	bool isDash = false;
	float defMoveAmount = 1.0f;
	float dashMoveAmount = 3.0f;
	int dashTimer = 8;
	int enemyHP = 3;
	bool isDamage = false;
	bool isDamageShake = false;
	int damageShakeCount = 0;
	float shakeObjectPos[3] = {0.0f,0.0f,0.0f};
};