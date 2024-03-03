#include"HighLumi.hlsli"


Texture2D<float4> tex0 : register(t0);  	// 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      	// 0番スロットに設定されたサンプラー


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


//ガウシアンブラーの重み
static const float weights[9] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
static const float weightSum = 16;

//ガウシアンブラーを適用
float4 ApplyGaussianBlurToHighLumi(float2 uv) {
    //9分割(5枚目が中心でそこからずれてるイメージ)
    float2 offsets[9] = {
        {-1, -1}, {0, -1}, {1, -1},
        {-1,  0}, {0,  0}, {1,  0},
        {-1,  1}, {0,  1}, {1,  1}
    };

    float4 sum = float4(0, 0, 0, 0);
	float strong = 0.003;
    
    for (int i = 0; i < 9; ++i) {
        float4 texColor = tex0.Sample(smp, uv + offsets[i] * (strong)); //仮のテクスチャサイズ
        float luminance = dot(texColor.rgb, float3(0.7, 0.587, 0.114)); //赤を強く参照
        float threshold = 0.5; //高輝度部分の閾値
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
