#include"SummarizeEngine.h"

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

	//ポストエフェクト初期化
	postEffectManager_ = std::make_unique<PostEffectManager>();
	postEffectManager_->Initialize(dxCommon_);

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
	postEffectManager_->Update();

};

void SummarizeEngine::Draw() {


	postEffectManager_->PreDrawScene(dxCommon_->GetCommandList());

	application_->PostEffectDraw();

	postEffectManager_->PostDrawScene(dxCommon_->GetCommandList());


	//Drawを重ねる
	dxCommon_->PreDraw();


	postEffectManager_->Draw(dxCommon_->GetCommandList());

	
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



	delete application_;
	delete imGuiManager_;
	delete dxCommon_;
	delete winApp_;

};