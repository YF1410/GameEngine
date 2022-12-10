#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

struct PSOutput {
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};

PSOutput main(VSOutput input) : SV_TARGET
{
	PSOutput output;
	// �e�N�X�`���}�b�s���O
	float4 texcolor = tex.Sample(smp, input.uv) * color;
	// Lambert����
	float3 light = normalize(float3(1,-1,1)); // �E�����@�����̃��C�g
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);

	// �ۉe
	for (int i = 0; i < CIRCLESHADOW_NUM; i++) {
		if (circleShadows[i].lightActive) {
			// �I�u�W�F�N�g�\�ʂ���L���X�^�[�ւ̃x�N�g��
			float3 casterv = circleShadows[i].casterPos - input.worldpos.xyz;
			// ���������ł̋���
			float d = dot(casterv, circleShadows[i].dir);

			// ���������W��
			float lightAtten = saturate(1.0f / (circleShadows[i].lightAtten.x + circleShadows[i].lightAtten.y * d + circleShadows[i].lightAtten.z * d * d));
			// �������}�C�i�X�Ȃ�0�ɂ���
			lightAtten *= step(0, d);

			// ���C�g�̍��W
			float3 lightPos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
			//  �I�u�W�F�N�g�\�ʂ��烉�C�g�ւ̃x�N�g���i�P�ʃx�N�g���j
			float3 lightv = normalize(lightPos - input.worldpos.xyz);
			// �p�x����
			float cos = dot(lightv, circleShadows[i].dir);
			// �����J�n�p�x����A�����I���p�x�ɂ����Č���
			// �����J�n�p�x�̓�����1�{ �����I���p�x�̊O����0�{�̋P�x
			float angleatten = smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);
			// �p�x��������Z
			lightAtten *= angleatten;

			// �S�Č��Z����
			shadecolor.rgb -= lightAtten;
		}
	}

	// �A�e�ƃe�N�X�`���̐F������
	output.target0 = shadecolor * texcolor;
	output.target1 = float4(1 - (shadecolor * texcolor).rgb,1);
	return output;
}