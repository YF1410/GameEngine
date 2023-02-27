#pragma once

#include "FbxModel.h"
#include "Camera.h"
#include "FbxLoader.h"
#include "DirectXCommon.h"
#include "LightGroup.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

class FbxObject3d {
protected: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://�萔
//�{�[���̍ő吔
	static const int MAX_BONES = 32;

public: // �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\���́i���W�ϊ��s��p�j
	struct ConstBufferDataTransform {
		XMMATRIX viewproj;    // �r���[�v���W�F�N�V�����s��
		XMMATRIX world; // ���[���h�s��
		XMFLOAT3 cameraPos; // �J�������W�i���[���h���W�j
		float pad;
		XMFLOAT4 color;
	};

	//�萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
	struct ConstBufferDataSkin {
		XMMATRIX bones[MAX_BONES];
	};

public: // �ÓI�����o�֐�
	/// <summary>
	/// �O���t�B�b�N�p�C�v���C���̐���
	/// </summary>
	static void CreateGraphicsPipeline();
	static void StaticFinalize();
	// setter
	static void SetCamera(Camera* cameraObject) { FbxObject3d::cameraObject = cameraObject; }

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ComPtr<ID3D12Device> device;
	// �J����
	static Camera* cameraObject;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;

public: // �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	void SetModel(FbxModel* fbxmodel) { this->fbxmodel = fbxmodel; }

	/// <summary>
	/// �A�j���[�V�����J�n
	/// </summary>
	void PlayAnimation(bool isResetTime);

	/// <summary>
	/// ���[�v�A�j���[�V�����J�n
	/// </summary>
	void LoopAnimation();

	/// <summary>
	/// �A�j���[�V������~
	/// </summary>
	void StopAnimation();

	//	���W�̎擾
	inline const XMFLOAT3& GetPosition() { return position; }
	// X,Y,Z�����̎擾
	inline const XMFLOAT3& GetRotation() { return rotation; }
	// �X�P�[���̎擾
	inline const XMFLOAT3& GetScale() { return scale; }
	// ���W�̐ݒ�
	inline void SetPosition(XMFLOAT3 position) { this->position = position; }
	// X,Y,Z�����̐ݒ�
	inline void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }
	// �X�P�[���̐ݒ�
	inline void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	void SetColor(XMFLOAT4 color) { this->color = color; }

	bool GetIsPlay() { return isPlay; }

	void ResetAnimationTime();

protected: // �����o�ϐ�
	// �萔�o�b�t�@�i���W�ϊ��s��p�j
	ComPtr<ID3D12Resource> constBuffTransform;
	//�萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12Resource> constBuffSkin;
	//���C�g�O���[�v
	std::unique_ptr<LightGroup> lightGroup;
	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	// ���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	// ���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	//�F
	XMFLOAT4 color = { 1,1,1,1 };
	// ���f��
	FbxModel* fbxmodel = nullptr;
	//1�t���[���̎���
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ���(�A�j���[�V����
	FbxTime currentTime;
	//���f��������(�A�j���[�V����
	FbxTime saveTime;
	//�A�j���[�V�����Đ���
	bool isPlay = false;
	//�A�j���[�V�������[�v�Đ���
	bool isLoop = false;

	XMFLOAT3 circleShadowDir = { 0,-1,0 };
	XMFLOAT3 circleShadowAtten = { 0.5f,0.6f,0.0f };
	XMFLOAT2 circleShadowFactorAngle = { 0.0f,0.5f };
};