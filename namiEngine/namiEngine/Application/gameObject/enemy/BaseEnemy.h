#pragma once
#include "Object3d.h"
#include "FbxObject3d.h"
#include "CollisionPrimitive.h"
#include "Player.h"
#include "Collision.h"


class BaseEnemy : public FbxObject3d
{
protected: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	static std::unique_ptr<BaseEnemy> Create(FbxModel* fbxmodel, Player* player);
public:
	//�R���X�g���N�^
	BaseEnemy();
	//�f�X�g���N�^
	~BaseEnemy();
	//������
	void Initialize(Player*player);
	//�X�V
	virtual void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);
	/*void SelectAction();
	void Sweep();
	void Stab();
	void Tackle();*/
	//�Q�b�^�[
	bool GetIsActive() { return isActive; }
	Sphere GetCollision() { return collision; }
	bool GetHaveElement() { return haveElement; }
	//�Z�b�^�[
	void SetIsDamage(bool isDamage) { this->isDamage = isDamage; }
	//�_���[�W����
	virtual void Damage();

	void CheckCollisionToPlayer(Camera*camera);

	virtual void Move();
protected:
	Player* player;
	float HP = 3;
	XMFLOAT3 savePos;
	XMFLOAT4 defaultColor = { 1,1,1,1 };
	bool isDamage = false;
	bool isActive = true;
	bool haveElement = false;
	int damageShakeCount = 0;
	float shakeObjectPos[3];
	float moveX = 0;
	float moveZ = 0;
	XMFLOAT3 defaultPos = {0,-5.0f,20.0f};
	Sphere collision;
	std::unique_ptr<Model> colliderVisualizationModel;
	std::unique_ptr<Object3d> colliderVisualizationObject;
};

