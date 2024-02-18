#include"HighLumi.hlsli"


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


// 3x3�̃K�E�V�A���t�B���^��������
half4 gaussian_sample(float2 uv, float2 dx, float2 dy)
{
	half4 col = 0;
	//col = tex0.Sample(smp, input.uv);
	col += tex0.Sample(smp, uv- dx - dy) * 1 / 16;
	col += tex0.Sample(smp, uv- dx) * 2 / 16;
	col += tex0.Sample(smp, uv- dx + dy) * 1 / 16;
	col += tex0.Sample(smp, uv- dy) * 2 / 16;
	col += tex0.Sample(smp, uv) * 4 / 16;
	col += tex0.Sample(smp, uv+ dy) * 2 / 16;
	col += tex0.Sample(smp, uv+ dx - dy) * 1 / 16;
	col += tex0.Sample(smp, uv+ dx) * 2 / 16;
	col += tex0.Sample(smp, uv+ dx + dy) * 1 / 16;
	return col;
}



float GetGrayScale(float4 dCol){
	 float grayScale = dCol.r * 0.299 + dCol.g * 0.587 + dCol.b * 0.114;
	 return grayScale;
}

float4 GetHighLumi(float4 dCol ,float grayScale){
    //���P�x���𒊏o

   
    float extract = smoothstep(0.6f, 0.8f, grayScale);
    float4 highLumi = dCol * extract;
    
    highLumi.a = 0.3;

    //���P�x���𒊏o
    return highLumi;
}


// �K�E�V�A���u���[�̏d��
static const float weights[9] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
static const float weightSum = 16;

// 臒l�𒴂���P�x�̃s�N�Z���ɃK�E�V�A���u���[��K�p
float4 ApplyGaussianBlurToHighLumi(float2 uv) {
    //9����
    float2 offsets[9] = {
        {-1, -1}, {0, -1}, {1, -1},
        {-1,  0}, {0,  0}, {1,  0},
        {-1,  1}, {0,  1}, {1,  1}
    };

    float4 sum = float4(0, 0, 0, 0);
	float strong = 0.003;
    for (int i = 0; i < 9; ++i) {
        float4 texColor = tex0.Sample(smp, uv + offsets[i] * (strong)); // ���̃e�N�X�`���T�C�Y
        float luminance = dot(texColor.rgb, float3(0.7, 0.587, 0.114));
        float threshold = 0.5; // ���P�x������臒l
        if (luminance > threshold) {
		texColor.a = 0.1;
            sum += texColor * weights[i];
        }
    }
    return sum / weightSum;
}

float4 main(VSOutput input) : SV_TARGET
{

	float4 colortex0 = tex0.Sample(smp, input.uv);

	float4 mainColor = colortex0;

    float4 color = ApplyGaussianBlurToHighLumi(input.uv);
    return color;

}
