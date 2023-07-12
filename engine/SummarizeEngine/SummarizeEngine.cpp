#include"SummarizeEngine.h"

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

	// FBX関連静的初期化
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());

	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxCommon_->GetDevice(), WinApp::window_width, WinApp::window_height);

	//パーティクル静的初期化
	ParticleManager::StaticInitialize(dxCommon_->GetDevice(), dxCommon_->GetCommandList());

	gameScene_ = new GameScene();
	gameScene_->Initialize(dxCommon_);

	imGuiManager_ = new ImGuiManager;
	imGuiManager_->Initialize(winApp_, dxCommon_);

	isRunningGame = true;
};

void SummarizeEngine::Update() {
	if (winApp_->ProcessMessage()) {
		isRunningGame = false;
	}

	//更新
	input_->Update();
	imGuiManager_->Begin();
	gameScene_->Update();


};

void SummarizeEngine::Draw() {
	//描画
	dxCommon_->PreDraw();


	gameScene_->Draw();

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

	delete gameScene_;
	delete imGuiManager_;
	delete dxCommon_;
	delete winApp_;

};