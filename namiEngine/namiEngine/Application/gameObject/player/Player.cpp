#include "Player.h"

using namespace DirectX;

std::unique_ptr<Player> Player::Create(std::list<std::unique_ptr<BaseEnemy>>* enemy) {
	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	std::unique_ptr<Player> player = std::make_unique<Player>();

	player->Initialize(enemy);

	return player;
}

void Player::Initialize(std::list<std::unique_ptr<BaseEnemy>>* enemy) {
	//������
	this->enemy = enemy;
	FbxObject3d::Initialize();
	input = Input::GetInstance();
	//�����蔻��̏�����
	receiveDamageCollision.center = XMLoadFloat3(&position);
	receiveDamageCollision.radius = 3.0f;
	inflictDamageCollision.center = XMLoadFloat3(&position);
	inflictDamageCollision.radius = 15.0f;

	//�_���[�W��^������͈͂̉����I�u�W�F�N�g�̏�����
	receiveDamageColliderVisualizationModel = Model::CreateFromObject("SphereCollider");
	receiveDamageColliderVisualizationObject = Object3d::Create(receiveDamageColliderVisualizationModel.get());
	receiveDamageColliderVisualizationObject->SetPosition(position);
	receiveDamageColliderVisualizationObject->SetScale(receiveDamageCollision.radius);
	receiveDamageColliderVisualizationObject->SetColor({ 1,0,0,0.3f });

	//�_���[�W���󂯂�͈͂̉����I�u�W�F�N�g�̏�����
	inflictDamageColliderVisualizationModel = Model::CreateFromObject("SphereCollider");
	inflictDamageColliderVisualizationObject = Object3d::Create(inflictDamageColliderVisualizationModel.get());
	inflictDamageColliderVisualizationObject->SetPosition(position);
	inflictDamageColliderVisualizationObject->SetScale(inflictDamageCollision.radius);
	inflictDamageColliderVisualizationObject->SetColor({ 0,1,0,0.1f });

	//�v���C���[�̃��f�����蓖��
	attackModel = FbxLoader::GetInstance()->LoadModelFromFile("JumpAttack");
	moveModel = FbxLoader::GetInstance()->LoadModelFromFile("Running");
	rollModel = FbxLoader::GetInstance()->LoadModelFromFile("Rolling");
	idleModel = FbxLoader::GetInstance()->LoadModelFromFile("Idling");
	SetModel(idleModel.get());
}

void Player::Update() {

	Combo();
	ChangeMotion();
	Attack();

	//�_�b�V����Ԃ̎�
	if (isDash) {
		dashTimer--;	//�^�C�}�[�����炷
		if (dashTimer <= 0) {	//0�ȉ��ɂȂ�����
			//�^�C�}�[���������ăt���O��false��
			dashTimer = 30;
			isDash = false;
		}
	}

	//�_���[�W���󂯂���
	if (isReceivedDamage) {
		//�F��ς���
		SetColor({ 1,0,0,1 });
		damageTimer--;
		if (damageTimer <= 0) {	//�^�C�}�[��0�ȉ��ɂȂ�����
			//�^�C�}�[���������ĐF��߂��t���O��false��
			damageTimer = 60;
			SetColor(defColor);
			isReceivedDamage = false;
		}
	}

	//HP��0�����ɂȂ��ă_���[�W�󂯂����ɔ�������J�����V�F�C�N�������܂�����
	if (HP <= 0 && !isNowCameraShake) {
		//�����Ă����Ԃ�false�ɂ���HP��-1�ɂȂ��Ă��������Ȃ�Ȃ��悤0�ɌŒ�
		isActive = false;
		HP = 0;
	}

	CollisionUpdate();
	FbxObject3d::Update();
}

void Player::CollisionUpdate()
{
	//�_���[�W�֌W�̓����蔻��X�V
	receiveDamageCollision.center = XMLoadFloat3(&position);
	inflictDamageCollision.center = XMLoadFloat3(&position);
	receiveDamageColliderVisualizationObject->SetPosition(position);
	receiveDamageColliderVisualizationObject->SetScale(receiveDamageCollision.radius);
	receiveDamageColliderVisualizationObject->Update();
	inflictDamageColliderVisualizationObject->SetPosition(position);
	inflictDamageColliderVisualizationObject->SetScale(inflictDamageCollision.radius);
	inflictDamageColliderVisualizationObject->Update();
}

void Player::Combo()
{
	if (isAttack) {
		attackTimer--;
		if (attackTimer <= 0) {
			isAttack = false;
			isRigor = true;
			StopAnimation();
		}
	}

	if (isRigor) {
		rigorTimer--;
		if (rigorTimer <= 0) {
			isRigor = false;
			isNowCombo = true;
			comboCount++;
		}
	}

	if (isNowCombo) {
		comboTimer--;
		if (comboTimer <= 0) {
			isNowCombo = false;
			comboCount = 0;
		}
	}
}

void Player::ChangeMotion()
{
	//�s���ɉ����ă��[�V�����̕ω�
	if (!isPlay) {
		isAttack = false;
		inflictDamageCollision.radius = 15.0f;
		for (std::unique_ptr<BaseEnemy>& enemyObj : *enemy) {
			enemyObj->SetIsFirstDamage(false);
		}
		if (isMove) {
			SetModel(moveModel.get());
			ResetAnimationTime();
		}
		else if (isIdle) {
			SetModel(idleModel.get());
			ResetAnimationTime();
		}

		if (!isLoop) {
			LoopAnimation();
		}
	}

	//���t���[���ҋ@��Ԃ�
	isIdle = true;
	isMove = false;
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList) {
	FbxObject3d::Draw(cmdList);
	Object3d::PreDraw(cmdList);

	//�����蔻��`��
	receiveDamageColliderVisualizationObject->Draw();
	if (!isPlay) {
	inflictDamageColliderVisualizationObject->Draw();
	}

	Object3d::PostDraw();
}

void Player::Attack()
{
	if (comboCount <= 2 && !isRigor) {
		//�ʏ�U��
		if ((input->TriggerKey(DIK_1) || input->TriggerMouse(MouseButton::LeftButton)) && !isPlay) {
			inflictDamageCollision.radius = 15.0f;
			attackPowor = 1;
			if (comboCount == 2) {
				attackPowor = 2;
			}
			isAttack = true;
			isRigor = false;
			isNowCombo = false;
			attackCount++;
			attackTimer = 15;
			rigorTimer = 30;
			comboTimer = 60;
			SetModel(attackModel.get());
			PlayAnimation(true);
		}

		//���U��
		if ((input->TriggerKey(DIK_2) || input->TriggerMouse(MouseButton::RightButton)) && !isPlay) {
			isHardAttack = true;
			attackCount++;
			SetModel(attackModel.get());
			PlayAnimation(true);
		}

		if (isHardAttack && !isPlay) {
			inflictDamageCollision.radius = 20.0f;
			attackTimer = 5;
			rigorTimer = 60;
			comboTimer = 120;
			attackPowor = 3;
			if (comboCount == 2) {
				attackPowor = 5;
			}
			isHardAttack = false;
			isAttack = true;
		}
	}
}

void Player::Move(Vector3 vec) {
	Vector3 moveVec = XMVector3Normalize(vec);
	float roteteY = XMConvertToDegrees(atan2(vec.z, vec.x));
	if (!isDash) {
		moveAmount = defMoveAmount;
	}
	else if (isDash) {
		moveAmount = dashMoveAmount;
	}

	if (!isHardAttack) {
		if (!isNowCameraShake) {
			if (input->PushKey(DIK_W)) {
				playerPos.x += moveVec.x * moveAmount;
				playerPos.z += moveVec.z * moveAmount;
				rotation.y = -roteteY + 90.0f;
				isIdle = false;
				isMove = true;
				if (input->PushKey(DIK_A)) {
					playerPos.x -= moveVec.z * moveAmount;
					playerPos.z += moveVec.x * moveAmount;
					rotation.y = -roteteY + 45.0f;
				}
				else if (input->PushKey(DIK_D)) {
					playerPos.x += moveVec.z * moveAmount;
					playerPos.z -= moveVec.x * moveAmount;
					rotation.y = -roteteY + 135.0f;
				}
			}
			else if (input->PushKey(DIK_S)) {
				playerPos.x -= moveVec.x * moveAmount;
				playerPos.z -= moveVec.z * moveAmount;
				rotation.y = -roteteY - 90.0f;
				isIdle = false;
				isMove = true;
				if (input->PushKey(DIK_A)) {
					playerPos.x -= moveVec.z * moveAmount;
					playerPos.z += moveVec.x * moveAmount;
					rotation.y = -roteteY - 45.0f;
				}
				if (input->PushKey(DIK_D)) {
					playerPos.x += moveVec.z * moveAmount;
					playerPos.z -= moveVec.x * moveAmount;
					rotation.y = -roteteY - 135.0f;
				}
			}
			else if (input->PushKey(DIK_A)) {
				playerPos.x -= moveVec.z * moveAmount;
				playerPos.z += moveVec.x * moveAmount;
				rotation.y = -roteteY;
				isIdle = false;
				isMove = true;
			}
			else if (input->PushKey(DIK_D)) {
				playerPos.x += moveVec.z * moveAmount;
				playerPos.z -= moveVec.x * moveAmount;
				rotation.y = -roteteY + 180.0f;
				isIdle = false;
				isMove = true;
			}
		}

		if (input->TriggerKey(DIK_SPACE) && (input->PushKey(DIK_W) || input->PushKey(DIK_A) || input->PushKey(DIK_S) || input->PushKey(DIK_D)) && !isDash) {
			isDash = true;
			isAttack = false;
			isHardAttack = false;
			dashCount++;
			SetModel(rollModel.get());
			PlayAnimation(true);
		}

		SetPosition(playerPos);
	}
}