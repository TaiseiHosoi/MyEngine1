#pragma once
#include"IPostTexture.h"
#include"PostEffect.h"
#include"HighLumi.h"
#include"MultiTex.h"
#include"SCDistort.h"
#include"NormalTex.h"
#include"DirectXCommon.h"

class PostEffectManager {
public:
	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	// 更新
	void Update();

	// 描画前処理
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

private:
	std::unique_ptr<NormalTex> normalTex_;
	std::unique_ptr<PostEffect> postEffect_;
	std::unique_ptr<HighLumi> highLumi_;
	std::unique_ptr<MultiTex> multiTex_;
	std::unique_ptr<SCDistort> sCDistort_;


};