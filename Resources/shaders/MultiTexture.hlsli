cbuffer ConstBufferData : register(b0) {
	//float4 color;
 //   matrix mat;
}

//���_�V�F�[�_�̏o�͍\����
//�i���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����j
struct VSOutput {
	//�V�X�e���p���_���W
	float4 svpos : SV_POSITION;
	//uv���W
	float2 uv :TEXCOORD;
};

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
};