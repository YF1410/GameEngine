#pragma once
#include "Object3d.h"
#include "FbxObject3d.h"
#include "CollisionPrimitive.h"
#include "input.h"
#include "Vector3.h"
#include "BaseEnemy.h"

class BaseEnemy;
class GameScene;

class Player :
	public FbxObject3d {
public:
	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<Player> Create(std::list<std::unique_ptr<BaseEnemy>>* enemy);

public:

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	void Initialize(std::list<std::unique_ptr<BaseEnemy>>* enemy);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void CollisionUpdate();

	/// <summary>
	/// �R���{����
	/// </summary>
	void Combo();

	/// <summary>
	/// ���[�V�����ύX
	/// </summary>
	void ChangeMotion();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �U������
	/// </summary>
	void Attack();

	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move(Vector3 vec);

	/// <summary>
	/// �_���[�W����
	/// </summary>
	/// <param name="damageQuantity"></param>
	void Damage(int damageQuantity) { HP -= damageQuantity; }

	//�Q�b�^�[
	float GetAttackPowor() { return attackPowor; }
	XMFLOAT3 GetPlayerPos() { return playerPos; }
	Sphere GetReceiveDamageCollision() { return receiveDamageCollision; }
	Sphere GetInflictDamageCollision() { return inflictDamageCollision; }
	bool GetIsAttack() { return isAttack; }
	bool GetIsHardAttack() { return isHardAttack; }
	bool GetIsDash() { return isDash; }
	bool GetIsReceivedDamage() { return isReceivedDamage; }
	bool GetIsActive() { return isActive; }
	int GetHP() { return HP; }
	int GetDashCount() { return dashCount; }
	int GetAttackCount() { return attackCount; }
	bool GetIsHaveElement() { return isHaveElement; }
	float GetMoveAmount() { return moveAmount; }

	//�Z�b�^�[
	void SetIsReceivedDamage(bool isReceivedDamage) { this->isReceivedDamage = isReceivedDamage; }
	void SetIsHaveElement(bool isHaveElement) { this->isHaveElement = isHaveElement; }
	void SetIsNowCameraShake(bool isNowCameraShake) { this->isNowCameraShake = isNowCameraShake; }
	void SetPlayerPos(XMFLOAT3 playerPos) { this->playerPos = playerPos; }
	void SetDefColor(XMFLOAT4 color) { defColor = color; }
private:
	std::list<std::unique_ptr<BaseEnemy>>* enemy;	//�G�l�~�[�z��

	Input* input = nullptr;	//����
	XMFLOAT3 playerPos = { 0.0f,0.0f,0.0f };	//�ړ����Ɏg�p
	XMFLOAT3 savePos = { 0.0f,0.0f,0.0f };		//�|�W�V�����̕ۑ��p
	int HP = 3;
	const float defMoveAmount = 1.0f;			//��{�̈ړ���
	const float dashMoveAmount = 1.2f;			//�_�b�V�����̈ړ���
	int dashTimer = 30;							//�_�b�V���̎�������
	int damageTimer = 60;						//�_���[�W����������̖��G����

	int dashCount = 0;							//�`���[�g���A���Ń_�b�V���񐔂𐔂��邽�߂Ɏg�p
	int attackCount = 0;						//�`���[�g���A���ōU���񐔂𐔂��邽�߂Ɏg�p

	XMFLOAT4 defColor = { 1,1,1,1 };			//��{�̐F

	float attackPowor = 1;						//�U���������ɗ^����_���[�W
	int attackTimer = 15;						//�U���̎�������
	bool isNowCombo = false;					//�R���{�����ǂ���
	int comboCount = 0;							//�����R���{�ڂ��̃J�E���g
	int comboTimer = 60;						//�R���{���؂��܂ł̎���
	bool isRigor = false;						//���d�����Ă��鎞�Ԃ��ǂ���
	int rigorTimer = 15;						//�d�����؂��܂ł̎���
	bool isActive = true;						//�����Ă��邩�ǂ���
	bool isHaveElement = false;					//�����������Ă��邩�ǂ���
	bool isDash = false;						//�_�b�V�����Ă��邩�ǂ���
	bool isAttack = false;					//��U�����Ă��邩�ǂ���
	bool isHardAttack = false;					//���U�����Ă��邩�ǂ���
	bool isReceivedDamage = false;				//�U�����󂯂Ă��邩�ǂ���
	bool isNowCameraShake = false;				//�J�����V�F�C�N�����Ă��邩�ǂ���
	bool isMove = false;						//�����Ă��邩�ǂ���
	bool isIdle = false;						//�ҋ@�����ǂ���
	Sphere receiveDamageCollision;				//�_���[�W���󂯂铖���蔻��
	Sphere inflictDamageCollision;				//�_���[�W��^�����铖���蔻��
	float moveAmount;							//�ړ���
	std::unique_ptr<Model> receiveDamageColliderVisualizationModel;			//�G���G���ƃ_���[�W���󂯂锻�����������p�̃��f��
	std::unique_ptr<Object3d> receiveDamageColliderVisualizationObject;		//�G���G���ƃ_���[�W���󂯂锻�����������p�̃I�u�W�F�N�g
	std::unique_ptr<Model> inflictDamageColliderVisualizationModel;			//�U�����ɔ͈͓��ɓG������ƃ_���[�W��^�����锻�����������p�̃��f��
	std::unique_ptr<Object3d> inflictDamageColliderVisualizationObject;		//�U�����ɔ͈͓��ɓG������ƃ_���[�W��^�����锻�����������p�̃I�u�W�F�N�g
	std::unique_ptr<FbxModel> attackModel;		//�U�����̃��f��
	std::unique_ptr<FbxModel> moveModel;		//�ړ����̃��f��
	std::unique_ptr<FbxModel> rollModel;		//�_�b�V�����̃��f��
	std::unique_ptr<FbxModel> idleModel;		//���삵�Ă��Ȃ����̃��f��
	//Mediator�p�^�[��

	//�~�܂��Ă��鎞�ɍs���Q�[�W�`���[�W
	//��������U���Ō����Ă���
};