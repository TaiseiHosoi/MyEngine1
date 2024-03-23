#include "PostEffectManager.h"

void PostEffectManager::Initialize(DirectXCommon* dxCommon)
{
	normalTex_ = std::make_unique<NormalTex>();
	postEffect_ = std::make_unique<PostEffect>();
	highLumi_ = std::make_unique<HighLumi>();
	multiTex_ = std::make_unique<MultiTex>();
	sCDistort_ = std::make_unique<SCDistort>();

	normalTex_->Initialize(dxCommon);
	postEffect_->Initialize(dxCommon);
	highLumi_->Initialize(dxCommon);
	multiTex_->Initialize(dxCommon);
	sCDistort_->Initialize(dxCommon);

}

void PostEffectManager::Update()
{
	highLumi_->Update();
}

void PostEffectManager::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	normalTex_->PreDrawScene(cmdList);

}

void PostEffectManager::PostDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	normalTex_->PostDrawScene();

	highLumi_->PreDrawScene(cmdList);

	normalTex_->Draw(cmdList);

	highLumi_->PostDrawScene();

	postEffect_->PreDrawScene(cmdList);

	normalTex_->Draw(cmdList);

	postEffect_->PostDrawScene();


	///テクスチャ合体
	//tex0
	multiTex_->PreDrawScene(cmdList, 0);

	postEffect_->Draw(cmdList);

	multiTex_->PostDrawScene(0);

	//tex1
	multiTex_->PreDrawScene(cmdList, 1);

	highLumi_->Draw(cmdList);

	multiTex_->PostDrawScene(1);

	//画面ゆがみ処理
	sCDistort_->PreDrawScene(cmdList);

	multiTex_->Draw(cmdList);

	sCDistort_->PostDrawScene();
}

void PostEffectManager::Draw(ID3D12GraphicsCommandList* cmdList)
{
	sCDistort_->Draw(cmdList);
}
