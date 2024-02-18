#include"PostEffect.hlsli"


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

float rand(float2 co)
{
	return frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43756.5453);
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



// easeIn
float ease_in_out_cubic(const float x)
{
	return x < 0.5
		? 4 * x * x * x
		: 1 - pow(-2 * x + 2, 3) / 2;
}

// CRT��1��f�̏㉺�[���Â��Ȃ錻�ۂ��Č�����
float crt_ease(const float x, const float base, const float offset)
{
	float tmp = fmod(x + offset, 1);
	float xx = 1 - abs(tmp * 2 - 1);
	float ease = ease_in_out_cubic(xx);
	return ease * base + base * 0.8;
}

float GetGrayScale(float4 dCol){
	 float grayScale = dCol.r * 0.299 + dCol.g * 0.587 + dCol.b * 0.114;
	 return grayScale;
}

float4 GetHighLumi(float4 dCol ,float grayScale){
    //���P�x���𒊏o

   
    float extract = smoothstep(0.6f, 0.8f, grayScale);
    float4 highLumi = dCol * extract;
    
    highLumi.a = 1;

    //���P�x���𒊏o
    return highLumi;
}



// �u���[���p�̃K�E�V�A���u���[��K�p (������gaussian_sample�����p�܂��͊g��)
// �����ł́Agaussian_sample�֐������̂܂܎g�p���Ă��܂����A
// �u���[���G�t�F�N�g��p�̃p�����[�^�������s�����Ƃ𐄏����܂��B

// �ŏI�I�ȍ������s��
//float4 ApplyBloom(float4 sceneColor, float2 uv)
//{
//    float4 brightParts = ExtractBrightParts(sceneColor);
//    
//    // �K�E�V�A���u���[�𖾂邢�����ɓK�p
//    float4 blurredBrightParts = gaussian_sample(uv, float2(0.002, 0), float2(0, 0.002));
//
//}


float4 main(VSOutput input) : SV_TARGET
{

	float yShift = 0.0018f;

	float4 colortex0 = tex0.Sample(smp, input.uv);
	

	float4 mainColor = colortex0;

	float2 uv = distort(input.uv, 0.2);
	// uv���͈͓��o�Ȃ���΍����h��Ԃ�
	if (uv.x < 0 || 1 < uv.x || uv.y < 0 || 1 < uv.y)
	{
		return float4(0,0,0,1);
	}


	// �ׂ̃s�N�Z���܂ł�UV���W�ł̍����v�Z���Ă���
	const float2 dx = float2(1 / 1280, 0);
	const float2 dy = float2(0, 1 / 720);


	// �K�E�V�A���t�B���^�ɂ���āA���E���ڂ���
	half4 col;
	col.x = gaussian_sample(uv + float2(-yShift,0), dx, dy).x;
	col.y = gaussian_sample(uv, dx, dy).y;
	col.z = gaussian_sample(uv + float2(yShift,0), dx, dy).z;

	// �c�ɂ��炵�Ă���
	const float floor_y = fmod(uv.y * 720 / 6, 1);	
	const float ease_r = crt_ease(floor_y, col.r, rand(uv + float2(-yShift,0)) * 0.1);
	const float ease_g = crt_ease(floor_y, col.g, rand(uv) * 0.1);
	const float ease_b = crt_ease(floor_y, col.b, rand(uv + float2(yShift,0)) * 0.1);

	
	float4 rgba =  float4(ease_r,ease_g,ease_b,1);


	//�u���[��
	float grayS = GetGrayScale(tex0.Sample(smp, uv));

	float4 brightParts;

	brightParts = GetHighLumi(tex0.Sample(smp, uv),grayS);

	if(grayS > 0.6f){
	 brightParts =  gaussian_sample(uv, float2(0.002, 0), float2(0, 0.002));
	}

	
		
	
	//�܂Ƃ߂�

	float4 finalCol = brightParts;


	return rgba + finalCol;





}
