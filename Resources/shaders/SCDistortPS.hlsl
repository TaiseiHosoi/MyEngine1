#include"SCDistort.hlsli"


Texture2D<float4> tex0 : register(t0);  	// 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      	// 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

// ��ʂ��o�������Ă���悤�ɂ䂪�܂���
float2 distort(float2 uv, float rate)
{
	uv -= 0.5;
	uv /= 1 - length(uv) * rate;
	uv += 0.5;
	return uv;
}


float4 main(VSOutput input) : SV_TARGET
{
    float2 uv = distort(input.uv, 0.2);
	// uv���͈͓��o�Ȃ���΍����h��Ԃ�
    if (uv.x < 0 || 1 < uv.x || uv.y < 0 || 1 < uv.y)
    {
        return float4(0, 0, 0, 1);
    }

    float4 colortex0 = tex0.Sample(smp, uv);

    return colortex0;

}
