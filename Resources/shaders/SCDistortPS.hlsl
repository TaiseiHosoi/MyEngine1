#include"SCDistort.hlsli"


Texture2D<float4> tex0 : register(t0);  	// 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      	// 0番スロットに設定されたサンプラー

// 画面が出っ張っているようにゆがませる
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
	// uvが範囲内出なければ黒く塗りつぶす
    if (uv.x < 0 || 1 < uv.x || uv.y < 0 || 1 < uv.y)
    {
        return float4(0, 0, 0, 1);
    }

    float4 colortex0 = tex0.Sample(smp, uv);

    return colortex0;

}
