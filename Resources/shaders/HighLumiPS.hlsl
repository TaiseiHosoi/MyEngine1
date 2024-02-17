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





float GetGrayScale(float4 dCol){
	 float grayScale = dCol.r * 0.299 + dCol.g * 0.587 + dCol.b * 0.114;
	 return grayScale;
}

float4 GetHighLumi(float4 dCol ,float grayScale){
    //高輝度部を抽出

   
    float extract = smoothstep(0.6f, 0.8f, grayScale);
    float4 highLumi = dCol * extract;
    
    highLumi.a = 0.3;

    //高輝度部を抽出
    return highLumi;
}


// ガウシアンブラーの重み
static const float weights[9] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
static const float weightSum = 16;

// 閾値を超える輝度のピクセルにガウシアンブラーを適用
float4 ApplyGaussianBlurToHighLumi(float2 uv) {
    float2 offsets[9] = {
        {-1, -1}, {0, -1}, {1, -1},
        {-1,  0}, {0,  0}, {1,  0},
        {-1,  1}, {0,  1}, {1,  1}
    };

    float4 sum = float4(0, 0, 0, 0);
	float strong = 0.003;
    for (int i = 0; i < 9; ++i) {
        float4 texColor = tex0.Sample(smp, uv + offsets[i] * (strong)); // 仮のテクスチャサイズ
        float luminance = dot(texColor.rgb, float3(0.299, 0.587, 0.114));
        float threshold = 0.5; // 高輝度部分の閾値
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

	float2 uv = distort(input.uv, 0.2);
	// uvが範囲内出なければ黒く塗りつぶす
	if (uv.x < 0 || 1 < uv.x || uv.y < 0 || 1 < uv.y)
	{
		return float4(0,0,0,1);
	}





    float4 color = ApplyGaussianBlurToHighLumi(uv);
    return color;





}
