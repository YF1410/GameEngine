#include "Material.h"
#include <DirectXTex.h>
#include <cassert>

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std;

// �ÓI�����o�ϐ��̎���
ComPtr<ID3D12Device> Material::device;

void Material::StaticInitialize(ComPtr<ID3D12Device> device) {
	Material::device = device;
	//device->SetName(L"3DObjMaterialDevice");
}

void Material::StaticFinalize() {
	device.Reset();
}

Material* Material::Create() {
	Material* instance = new Material;

	instance->Initialize();

	return instance;
}

void Material::Initialize() {
	// �萔�o�b�t�@�̐���
	CreateConstantBuffer();
}

void Material::CreateConstantBuffer() {
	HRESULT result;
	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);
	if (FAILED(result)) 	{
		assert(0);
	}
}

void Material::LoadTexture(const std::string& directoryPath, CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle) {
	// �e�N�X�`���Ȃ�
	if (textureFilename.size() == 0) 	{
		textureFilename = "white1x1.png";
	}

	cpuDescHandleSRV = cpuHandle;
	gpuDescHandleSRV = gpuHandle;

	HRESULT result = S_FALSE;

	// WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	// �t�@�C���p�X������
	string filepath = directoryPath + textureFilename;
	wchar_t wfilepath[128];

	// ���j�R�[�h������ɕϊ�
	MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	result = LoadFromWICFile
	(
		wfilepath, WIC_FLAGS_NONE,
		&metadata, scratchImg
	);
	if (FAILED(result)) 	{
		assert(0);
	}

	const Image* img = scratchImg.GetImage(0, 0, 0); // ���f�[�^���o

	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D
	(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	// �e�N�X�`���p�o�b�t�@�̐���
	result = device->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&texbuff)
	);
	if (FAILED(result)) 	{
		assert(0);
	}

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texbuff->WriteToSubresource
	(
		0,
		nullptr, // �S�̈�փR�s�[
		img->pixels, // ���f�[�^�A�h���X
		(UINT)img->rowPitch, // 1���C���T�C�Y
		(UINT)img->slicePitch // 1���T�C�Y
	);
	if (FAILED(result)) 	{
		assert(0);
	}

	// �V�F�[�_���\�[�X�r���[�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView
	(
		texbuff.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
		cpuDescHandleSRV
	);
}

void Material::Update() {
	HRESULT result;
	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataB1* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) 	{
		constMap->ambient = ambient;
		constMap->diffuse = diffuse;
		constMap->specular = specular;
		constMap->alpha = alpha;
		constBuff->Unmap(0, nullptr);
	}
}