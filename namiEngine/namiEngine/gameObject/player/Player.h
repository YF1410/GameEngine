#pragma once
#include "Object3d.h"
#include "Input.h"
class Player :
	public Object3d {
public:
	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static Player* Create(Model* model = nullptr);
public:

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize() override;

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update() override;

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	void OnCollision(const CollisionInfo& info) override;

private:
	Input* input = nullptr;

	bool onGround = true;
	// �����x�N�g��
	DirectX::XMVECTOR fallV;
};