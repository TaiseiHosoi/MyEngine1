#include"PostEffect.hlsli"


Texture2D<float4> tex0 : register(t0);  	// 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1);  	// 1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��

SamplerState smp : register(s0);      	// 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{

	float shift = 0.002f;
	float4 colortex0 = tex0.Sample(smp, input.uv);
	float4 colortex1 = tex0.Sample(smp, input.uv);

	float4 mainColor = colortex0;
	float colorR = tex0.Sample(smp, input.uv + float2(-shift,0)).x;
	float colorG = tex0.Sample(smp, input.uv + float2(0,0)).y;
	float colorB = tex0.Sample(smp, input.uv + float2(shift,0)).z;

	float4 col =  + float4(colorR,colorG,colorB,1);

	
	return col;

}
