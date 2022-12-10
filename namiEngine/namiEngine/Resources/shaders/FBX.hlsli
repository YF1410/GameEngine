//�{�[���̍ő吔
static const int MAX_BONES = 32;

cbuffer cbuff0 : register(b0) {
	matrix viewproj; // �r���[�v���W�F�N�V�����s��
	matrix world; // ���[���h�s��
	float3 cameraPos; // �J�������W�i���[���h���W�j
	float1 pad;
	float4 color;
};
// ���s�����̐�
static const int DIRLIGHT_NUM = 3;

struct DirLight {
	float3 lightv;    // ���C�g�ւ̕����̒P�ʃx�N�g��
	float3 lightColor;    // ���C�g�̐F(RGB)
	uint lightActive;
};

// �_�����̐�
static const int POINTLIGHT_NUM = 3;

struct PointLight {
	float3 lightPos;    // ���C�g���W
	float3 lightColor;  // ���C�g�̐F(RGB)
	float3 lightatten;	// ���C�g���������W��
	uint lightActive;
};

// �X�|�b�g���C�g�̐�
static const int SPOTLIGHT_NUM = 3;

struct SpotLight {
	float3 lightv;		// ���C�g�̌��������̋t�x�N�g���i�P�ʃx�N�g���j
	float3 lightPos;    // ���C�g���W
	float3 lightColor;  // ���C�g�̐F(RGB)
	float3 lightatten;	// ���C�g���������W��
	float2 lightfactoranglecos; // ���C�g�����p�x�̃R�T�C��
	uint lightActive;
};

// �ۉe�̐�
static const int CIRCLESHADOW_NUM = 1;

struct CircleShadow {
	float3 dir;		// ���e�����̋t�x�N�g���i�P�ʃx�N�g���j
	float3 casterPos;    // �L���X�^�[���W
	float  distanceCasterLight;	// �L���X�^�[�ƃ��C�g�̋���
	float3 lightAtten;	// ���������W��
	float2 factorAngleCos; // �����p�x�̃R�T�C��
	uint lightActive;
};

cbuffer cbuff3 : register(b3) {
	float3 ambientColor;
	DirLight dirLights[DIRLIGHT_NUM];
	PointLight pointLights[POINTLIGHT_NUM];
	SpotLight spotLights[SPOTLIGHT_NUM];
	CircleShadow circleShadows[CIRCLESHADOW_NUM];
}

cbuffer skinning : register(b2) {
	matrix matSkinning[MAX_BONES];
};

//�o�[�e�b�N�X�o�b�t�@�[�̓���
struct VSInput {
	float4 pos	: POSITION;				//�ʒu
	float3 normal : NORMAL;				//���_�@��
	float2 uv	: TEXCOORD;				//�e�N�X�`���[���W
	uint4 boneIndices : BONEINDICES;	//�{�[���̔ԍ�
	float4 boneWeights :BONEWEIGHTS;		//�{�[���̃X�L���E�F�C�g
};

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput {
	float4 svpos : SV_POSITION; // �V�X�e���p���_���W
	float4 worldpos : POSITION; // ���[���h���W
	float3 normal :NORMAL; // �@��
	float2 uv  :TEXCOORD; // uv�l
};
