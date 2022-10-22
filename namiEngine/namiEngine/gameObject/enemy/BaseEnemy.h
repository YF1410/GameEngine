#pragma once
#include "FbxObject3d.h"
#include "CollisionPrimitive.h"


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
	static std::unique_ptr<BaseEnemy> Create(FbxModel* fbxmodel);
public:
	//�R���X�g���N�^
	BaseEnemy();
	//�f�X�g���N�^
	~BaseEnemy();
	//������
	void Initialize();
	//�ď�����
	void RestartInitialize();
	//�X�V
	void Update();
	/*void SelectAction();
	void Sweep();
	void Stab();
	void Tackle();*/
	//�Q�b�^�[
	bool GetIsActive() { return isActive; }
	Sphere GetCollision() { return collision; }
	//�Z�b�^�[
	void SetIsDamage(bool isDamage) { this->isDamage = isDamage; }
	//�_���[�W����
	void Damage(XMFLOAT3 pos,int DamageQuantity);
private:
	int HP = 3;
	XMFLOAT3 savePos;
	bool isDamage = false;
	bool isActive = true;
	int damageShakeCount = 0;
	float shakeObjectPos[3];
	float moveX = 0;
	float moveZ = 0;
	XMFLOAT3 defaultPos = {0,-5.0f,20.0f};
	Sphere collision;
};

