#include"SummarizeEngine.h"
#include"PostEffect.h"
#include"HighLumi.h"
#include"SCDistort.h"
#include"MultiTex.h"
using namespace MyEngine;

void SummarizeEngine::Initialize() {
	winApp_ = WinApp::GetInstance();
	dxCommon_ = new DirectXCommon;
	audio_ = Audio::GetInstance();

	//winApp初期化
	winApp_->Initialize();

	// DirectX初期化処理　ここから
	dxCommon_->Initialize(winApp_);

	audio_->Initialize();

	input_->Initialize(winApp_);

	//FBX関連静的初期化
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());

	//3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxCommon_->GetDevice());

	//パーティクル静的初期化
	ParticleManager::StaticInitialize(dxCommon_->GetDevice(), dxCommon_->GetCommandList());

	application_ = new Application();
	application_->Initialize(dxCommon_);

	//ポストエフェクト
	PostEffect::Initialize(dxCommon_);	//ポストエフェクトのスタティックメンバ変数初期化
	HighLumi::Initialize(dxCommon_);
	SCDistort::Initialize(dxCommon_);
	MultiTex::Initialize(dxCommon_);

	//ImGui
	imGuiManager_ = new ImGuiManager;
	imGuiManager_->Initialize(winApp_, dxCommon_);

	//ゲームが動いているかの初期化
	isRunningGame = true;
};

void SummarizeEngine::Update() {
	if (winApp_->ProcessMessage()) {
		isRunningGame = false;
	}

	//更新
	input_->Update();
	imGuiManager_->Begin();
	application_->Update();


};

void SummarizeEngine::Draw() {


	PostEffect::PreDrawScene(dxCommon_->GetCommandList());

	application_->PostEffectDraw();

	PostEffect::PostDrawScene();

	//ブルーム処理
	HighLumi::PreDrawScene(dxCommon_->GetCommandList());

	application_->PostEffectDraw();

	HighLumi::PostDrawScene();

	///テクスチャ合体
	//tex0
	MultiTex::PreDrawScene(dxCommon_->GetCommandList(),0);

	PostEffect::Draw(dxCommon_->GetCommandList());

	MultiTex::PostDrawScene(0);

	//tex1
	MultiTex::PreDrawScene(dxCommon_->GetCommandList(), 1);

	HighLumi::Draw(dxCommon_->GetCommandList());

	MultiTex::PostDrawScene(1);

	//画面ゆがみ処理
	SCDistort::PreDrawScene(dxCommon_->GetCommandList());

	MultiTex::Draw(dxCommon_->GetCommandList());

	SCDistort::PostDrawScene();


	//Drawを重ねる
	dxCommon_->PreDraw();


	SCDistort::Draw(dxCommon_->GetCommandList());

	
#pragma region OutFlameDraw

	application_->OutFlameDraw();

#pragma endregion OutFlameDraw
	

	imGuiManager_->End();

	imGuiManager_->Draw();

	dxCommon_->PostDraw();
};

void SummarizeEngine::Finalize() {
	winApp_->Finalize();
	imGuiManager_->Finalize();
	audio_->Finalize();

	//ID3D12DebugDevice* debugInterface;
	//
	//if (SUCCEEDED(dxCommon_->GetDevice()->QueryInterface(&debugInterface))) {
	//	debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
	//	debugInterface->Release();
	//}
	//元データ解放
	//delete[] imageData;
	//入力開放

	delete application_;
	delete imGuiManager_;
	delete dxCommon_;
	delete winApp_;

};