#include"PostEffect.hlsli"


Texture2D<float4> tex0 : register(t0);  	// 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      	// 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[


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


float4 main(VSOutput input) : SV_TARGET
{

	float yShift = 0.00001f;


	// �ׂ̃s�N�Z���܂ł�UV���W�ł̍����v�Z���Ă���
	const float2 dx = float2(1 / 1280, 0);
	const float2 dy = float2(0, 1 / 720);


	// �K�E�V�A���t�B���^�ɂ���āA���E���ڂ���
	half4 col;
	col.x = gaussian_sample(input.uv + float2(-yShift,0), dx, dy).x;
    col.y = gaussian_sample(input.uv, dx, dy).y;
    col.z = gaussian_sample(input.uv + float2(yShift, 0), dx, dy).z;

	// �c�ɂ��炵�Ă���
    const float floor_y = fmod(input.uv.y * 720 / 6, 1);
    const float ease_r = crt_ease(floor_y, col.r, rand(input.uv + float2(-yShift, 0)) * 0.1);
    const float ease_g = crt_ease(floor_y, col.g, rand(input.uv) * 0.1);
    const float ease_b = crt_ease(floor_y, col.b, rand(input.uv + float2(yShift, 0)) * 0.1);
	float4 rgba =  float4(ease_r,ease_g,ease_b,1);

	return rgba;

}
