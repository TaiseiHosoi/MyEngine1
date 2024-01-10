#include"PostEffect.hlsli"


Texture2D<float4> tex0 : register(t0);  	// 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);  	// 1番スロットに設定されたテクスチャ

SamplerState smp : register(s0);      	// 0番スロットに設定されたサンプラー

// 画面が出っ張っているようにゆがませる
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

// 3x3のガウシアンフィルタをかける
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

// CRTの1画素の上下端が暗くなる現象を再現する
float crt_ease(const float x, const float base, const float offset)
{
	float tmp = fmod(x + offset, 1);
	float xx = 1 - abs(tmp * 2 - 1);
	float ease = ease_in_out_cubic(xx);
	return ease * base + base * 0.8;
}

float4 main(VSOutput input) : SV_TARGET
{

	float shift = 0.0018f;
	float4 colortex0 = tex0.Sample(smp, input.uv);
	float4 colortex1 = tex0.Sample(smp, input.uv);

	float4 mainColor = colortex0;

	float2 uv = distort(input.uv, 0.2);
	// uvが範囲内出なければ黒く塗りつぶす
	if (uv.x < 0 || 1 < uv.x || uv.y < 0 || 1 < uv.y)
	{
		return float4(0,0,0,1);
	}


	// 隣のピクセルまでのUV座標での差を計算しておく
	const float2 dx = float2(1 / 1280, 0);
	const float2 dy = float2(0, 1 / 720);

	// RGBごとにUVをずらす
	uv += -1 * dy;
	uv += 0 * dy;
	uv += 1 * dy;



	// ガウシアンフィルタによって、境界をぼかす
	half4 col;
	col.x = gaussian_sample(uv + float2(-shift,0), dx, dy).x;
	col.y = gaussian_sample(uv, dx, dy).y;
	col.z = gaussian_sample(uv + float2(shift,0), dx, dy).z;


	const float floor_y = fmod(uv.y * 720 / 6, 1);
	const float ease_r = crt_ease(floor_y, col.r, rand(uv + float2(-shift,0)) * 0.1);
	const float ease_g = crt_ease(floor_y, col.g, rand(uv) * 0.1);
	const float ease_b = crt_ease(floor_y, col.b, rand(uv + float2(shift,0)) * 0.1);

	
	float3 rgb =  float3(ease_r,ease_g,ease_b);
	


	return half4(rgb, 1);





}
