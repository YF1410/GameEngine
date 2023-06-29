#include "TutorialScene.h"
#include "SceneManager.h"

void TutorialScene::Initialize() {
	srand(static_cast<unsigned int>(time(NULL)));
	// nullptr�`�F�b�N
	dxCommon = DirectXCommon::GetInstance();
	input = Input::GetInstance();
	audio = Audio::GetInstance();

	// �J��������
	cameraObject = std::make_unique<Camera>(WinApp::window_width, WinApp::window_height);

	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
		return;
	}
	// �f�o�b�O�e�L�X�g������
	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextTexNumber);

	// �e�N�X�`���ǂݍ���

	//�X�v���C�g����
	fadeSprite = Sprite::Create(2, { 0.0f,0.0f }, fadeColor);
	// �w�i�X�v���C�g����

	// �p�[�e�B�N���}�l�[�W������
	particleMan = ParticleManager::GetInstance();
	particleMan->SetCamera(cameraObject.get());

	// �J���������_���Z�b�g
	cameraObject->SetTarget(cameraTarget);
	cameraObject->SetEye({ cameraEye[0],cameraEye[1],cameraEye[2] });

	// ���f�������w�肵�ăt�@�C���ǂݍ���
	playerModel = FbxLoader::GetInstance()->LoadModelFromFile("Walking");
	elementModel = FbxLoader::GetInstance()->LoadModelFromFile("windingObject");
	groundModel = Model::CreateFromObject("stage1");
	skydomeModel = Model::CreateFromObject("skydome");
	// ���C�g����
	lightGroup = LightGroup::Create();
	lightGroup->SetCircleShadowActive(0, true);
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLightGroup(lightGroup.get());
	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(cameraObject.get());

	// �J�������Z�b�g
	FbxObject3d::SetCamera(cameraObject.get());

	player = Player::Create(&enemy);

	/*for (int i = 0; i < 5; i++) {
		enemy.push_back(BaseEnemy::Create(player.get(), cameraObject.get()));
		enemy.push_back(BulletEnemy::Create(player.get(), cameraObject.get()));
	}

	for (int i = 0; i < 2; i++) {
		enemy.push_back(ElementEnemy::Create(player.get(), cameraObject.get()));
	}*/

	groundObj = Object3d::Create(groundModel.get());
	groundObj->SetScale(8.5f);
	skydomeObj = Object3d::Create(skydomeModel.get());
	skydomeObj->SetScale(5.0f);

	cameraCollider.center = XMLoadFloat3(&cameraObject->GetEye());
	cameraCollider.radius = 5.0f;

	skydomeCollider.center = { 0,0,0 };
	skydomeCollider.radius = 400.0f;

	HP[0] = Sprite::Create(5, { 0.0f,0.0f });
	HP[1] = Sprite::Create(6, { 0.0f,0.0f });
	HP[2] = Sprite::Create(7, { 0.0f,0.0f });
	HP[3] = Sprite::Create(8, { 0.0f,0.0f });

	cameraObject->Update();
	player->Update();
	for (std::unique_ptr<BaseEnemy>& enemyObj : enemy) {
		enemyObj->Update();
	}

	for (std::unique_ptr<ElementObject>& elementObj : element) {
		elementObj->Update();
	}
	groundObj->Update();
	skydomeObj->Update();
	particleMan->Update();
}

void TutorialScene::Finalize()
{
}

void TutorialScene::Update() {
	if (isFadeIn) {
		fadeColor.w -= 0.01f;
		fadeSprite->SetColor(fadeColor);
		if (fadeColor.w <= 0.0f) {
			isFadeIn = false;
		}
	}
	else if (!isFadeIn && !isFadeOut) {
		XMFLOAT3 pPos = player->GetPosition();
		XMFLOAT3 cPos = cameraObject->GetEye();
		XMFLOAT3 colliderCenter = { (pPos.x + cPos.x) / 2,(pPos.y + cPos.y) / 2 ,(pPos.z + cPos.z) / 2 };
		cameraCollider.center = XMLoadFloat3(&colliderCenter);

		if (Collision::CheckSphereInside2Sphere(cameraCollider, skydomeCollider)) {
			
			cameraObject->SetEye({ cameraEye[0] + player->GetPlayerPos().x, cameraEye[1],cameraEye[2] + player->GetPlayerPos().z });
			cameraObject->SetTarget({ cameraTarget.x + player->GetPlayerPos().x, cameraTarget.y,cameraTarget.z + player->GetPlayerPos().z });
		}
		else {

		}

		Vector3 vec = { pPos.x - cPos.x, pPos.y - cPos.y,pPos.z - cPos.z };

		switch (num) {
		case 1:
			player->Move(vec);
			if (player->GetDashCount() >= 5) {
				num++;
			}
			//���ʓ����A���n�񂵂���num++;
			break;
		case 2:
			player->Attack();
			player->Move(vec);
			if (player->GetAttackCount() >= 3) {
				num++;
				enemy.push_back(BaseEnemy::Create(player.get(), cameraObject.get()));
			}
			//�U��n�񂵂���num++�Abaseenemy.push_back();
			break;
		case 3:
			player->Attack();
			player->Move(vec);
			for (std::unique_ptr<BaseEnemy>& enemyObj : enemy) {
				enemyObj->Move();
				enemyObj->Damage();
			}
			if (enemy.empty() && !player->GetIsAttack() && !player->GetIsHardAttack()) {
				num++;
				enemy.push_back(ElementEnemy::Create(player.get(), cameraObject.get()));
			}
			//�G�l�~�[�|������num++�AelementEnemy.push_back();
			break;
		case 4:
			player->Attack();
			player->Move(vec);
			for (std::unique_ptr<BaseEnemy>& enemyObj : enemy) {
				enemyObj->Move();
				enemyObj->Damage();
				if (!enemyObj->GetIsActive() && enemyObj->GetHaveElement()) {
					element.push_back(ElementObject::Create(elementModel.get(), enemyObj->GetPosition()));
				}
			}

			if (player->GetIsHaveElement()) {
				isFadeOut = true;
				num++;
			}
			//���������������num++�A���U���̐�����
			break;
		case 5:
			player->Attack();
			player->Move(vec);
			for (std::unique_ptr<BaseEnemy>& enemyObj : enemy) {
				enemyObj->Move();
				enemyObj->Damage();
				if (!enemyObj->GetIsActive() && enemyObj->GetHaveElement()) {
					element.push_back(ElementObject::Create(elementModel.get(), enemyObj->GetPosition()));
				}
			}
			//���U������ׂ̃G�l�~�[�z�u�����Ĉꌂ�œ|������num++;
			break;
		case 6:
			player->Attack();
			player->Move(vec);
			for (std::unique_ptr<BaseEnemy>& enemyObj : enemy) {
				enemyObj->Move();
				enemyObj->Damage();
				if (!enemyObj->GetIsActive() && enemyObj->GetHaveElement()) {
					element.push_back(ElementObject::Create(elementModel.get(), enemyObj->GetPosition()));
				}
			}
			//�e�������Ă���G�̐�����������num++;
			break;
		}

		enemy.remove_if([](std::unique_ptr<BaseEnemy>& enemyObj) {return !enemyObj->GetIsActive(); });
		element.remove_if([](std::unique_ptr<ElementObject>& elementObj) {return !elementObj->GetIsActive(); });

		for (std::unique_ptr<BaseEnemy>& enemyObj : enemy) {
			enemyObj->CheckCollisionToPlayer();
		}

		for (std::unique_ptr<ElementObject>& elementObj : element) {
			elementObj->GetIsActive();
			if (Collision::CheckSphere2Sphere(player->GetReceiveDamageCollision(), elementObj->GetCollision()) && elementObj->GetIsActive()) {
				elementObj->SetIsActive(false);

				player->SetDefColor({ 0,0,1,1 });
				player->SetColor({ 0,0,1,1, });
				player->SetIsHaveElement(true);
			}
		}

		lightGroup->SetCircleShadowDir(0, XMLoadFloat3(&circleShadowDir));
		lightGroup->SetCircleShadowCasterPos(0, player->GetPosition());
		lightGroup->SetCircleShadowDistanceCasterLight(0, 450.0f);
		lightGroup->SetCircleShadowAtten(0, circleShadowAtten);
		lightGroup->SetCircleShadowFactorAngle(0, circleShadowFactorAngle);

		if (cameraObject->GetShakeFlag()) {
			player->SetIsNowCameraShake(true);
		}
		else if (!cameraObject->GetShakeFlag()) {
			player->SetIsNowCameraShake(false);
		}

		cameraObject->CameraShake();

		if (!player->GetIsActive()) {
			isFadeOut = true;
		}

		cameraObject->Update();
		player->Update();
		for (std::unique_ptr<BaseEnemy>& enemyObj : enemy) {
			enemyObj->Update();
		}

		for (std::unique_ptr<ElementObject>& elementObj : element) {
			elementObj->Update();
		}

		groundObj->Update();
		skydomeObj->Update();
		//skydomeCollider.Update();
		particleMan->Update();
		lightGroup->Update();
	}
	else if (isFadeOut) {
		if (isFadeOut) {
			fadeColor.w += 0.02f;
			fadeSprite->SetColor(fadeColor);
			if (fadeColor.w >= 1.0f) {
				isFadeOut = false;
				//SceneManager::GetInstance()->ToGameScene();
			}
		}
	}
}

void TutorialScene::Draw() {
	/*object2Pos[0] = object2->GetPosition().x;
	object2Pos[1] = object2->GetPosition().y;
	object2Pos[2] = object2->GetPosition().z;*/

	//ImGui::Begin("window");
	//ImGui::SetWindowPos(ImVec2(0, 0));
	//ImGui::SetWindowSize(ImVec2(500, 200));
	////ImGui::InputFloat3("cubePos", object2Pos);
	////ImGui::SliderFloat3("object2Pos", object2Pos,-1000,1000);
	//ImGui::DragFloat3("object2Pos", object2Pos);
	//ImGui::DragFloat3("cameraEye", cameraEye);
	//ImGui::End();

	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �w�i�X�v���C�g�`��
	//spriteBG->Draw();

	/// <summary>
	/// �����ɔw�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D�`��
	// 3D�I�u�W�F�N�g�̕`��
	Object3d::PreDraw(cmdList);
	groundObj->Draw();
	skydomeObj->Draw();
	Object3d::PostDraw();

	for (std::unique_ptr<BaseEnemy>& enemyObj : enemy) {
		enemyObj->Draw(cmdList);
	}


	for (std::unique_ptr<ElementObject>& elementObj : element) {
		elementObj->Draw(cmdList);
	}

	player->Draw(cmdList);
	//lightGroup->Draw(cmdList, 3);

	// �p�[�e�B�N���̕`��
	particleMan->Draw(cmdList);
#pragma endregion

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// �����ɑO�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>

	HP[player->GetHP()]->Draw();


	fadeSprite->Draw();

	// �f�o�b�O�e�L�X�g�̕`��
	debugText->DrawAll();

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion
}