#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Object3d.h"
#include "Collision.h"
#include "BaseEnemy.h"
#include "Player.h"

class BaseEnemy;
class Player;

class Bullet : public Object3d
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	static std::unique_ptr<Bullet> Create(XMFLOAT3 startPos, Player* player);
	static std::unique_ptr<Bullet> Create(XMFLOAT3 startPos, BaseEnemy*enemy);
public:
	bool Initialize(XMFLOAT3 startPos, Player* player);
	bool Initialize(XMFLOAT3 startPos, BaseEnemy*enemy);
	void PlayerBulletUpdate();
	void EnemyBulletUpdate(Camera*camera);
	void Draw();
	bool GetIsActive() { return isActive; }
private:
	BaseEnemy* enemy;
	Player* player;
	bool isActive;
	Sphere collision;
	Sphere targetCollision;
	XMFLOAT3 startPosition;
	XMFLOAT3 endPosition;
	std::unique_ptr<Object3d> bulletObject;
private:
	static std::unique_ptr<Model> bulletModel;
};

