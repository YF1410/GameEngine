#include "Player.h"
#include "Input.h"
#include "DebugText.h"
#include "SphereCollider.h"
#include "ParticleManager.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

using namespace DirectX;

Player* Player::Create(Model* model) {
	//3Dオブジェクトのインスタンスを生成
	Player* instance = new Player();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	//モデルのセット
	if (model) {
		instance->SetModel(model);
	}
	return instance;
}

bool Player::Initialize() {
	if (!Object3d::Initialize()) {
		return false;
	}
	
	//コライダーの追加
	float radius = 0.6f;
	//半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	return true;
}

void Player::Update() {
	//A,Dで旋回
	if (input->PushKey(DIK_A)) {
		objRotation.y -= 2.0f;
	}
	else if (input->PushKey(DIK_D)) {
		objRotation.y += 2.0f;
	}

	//移動ベクトルをY軸周りの角度で回転
	XMVECTOR move = { 0,0,0.1f,0 };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(objRotation.y));
	move = XMVector3TransformNormal(move, matRot);

	//向いている方向に移動
	if (input->PushKey(DIK_S)) {
		objPosition.x -= move.m128_f32[0];
		objPosition.y -= move.m128_f32[1];
		objPosition.z -= move.m128_f32[2];
	}
	else if (input->PushKey(DIK_W)) {
		objPosition.x += move.m128_f32[0];
		objPosition.y += move.m128_f32[1];
		objPosition.z += move.m128_f32[2];
	}


	// 落下処理
	if (!onGround) {
		// 下向き加速度
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// 加速
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
		// 移動
		objPosition.x += fallV.m128_f32[0];
		objPosition.y += fallV.m128_f32[1];
		objPosition.z += fallV.m128_f32[2];
	}
	// ジャンプ操作
	else if (input->TriggerKey(DIK_SPACE)) {
		onGround = false;
		const float jumpVYFist = 0.2f;
		fallV = { 0, jumpVYFist, 0, 0 };
	}

	//行列の更新など
	Object3d::Update();

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	// 球の上端から球の下端までのレイキャスト
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;

	// 接地状態
	if (onGround) {
		// スムーズに坂を下る為の吸着距離
		const float adsDistance = 0.2f;
		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
			onGround = true;
			objPosition.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			// 行列の更新など
			Object3d::Update();
		}
		// 地面がないので落下
		else {
			onGround = false;
			fallV = {};
		}
	}
	// 落下状態
	else if (fallV.m128_f32[1] <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f)) {
			// 着地
			onGround = true;
			objPosition.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			// 行列の更新など
			Object3d::Update();
		}
	}
}

void Player::OnCollision(const CollisionInfo& info) {
	//デバッグできスト表示
	DebugText::GetInstance()->Printf("Collision detected.");

	//衝突点にパーティクルを表示させる
	for (int i = 0; i < 1; ++i) {
		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		ParticleManager::GetInstance()->Add(10, XMFLOAT3(info.inter.m128_f32), vel, XMFLOAT3(), 0.0f, 1.0f);
	}
}