#include"SummarizeEngine.h"

void SummarizeEngine::Initialize() {
	winApp_ = WinApp::GetInstance();
	dxCommon_ = new DirectXCommon;
	audio_ = Audio::GetInstance();

	//winApp������
	winApp_->Initialize();

	// DirectX�����������@��������
	dxCommon_->Initialize(winApp_);

	audio_->Initialize();

	input_->Initialize(winApp_);

	// FBX�֘A�ÓI������
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());

	// 3D�I�u�W�F�N�g�ÓI������
	Object3d::StaticInitialize(dxCommon_->GetDevice(), WinApp::window_width, WinApp::window_height);

	//�p�[�e�B�N���ÓI������
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

	//�X�V
	input_->Update();
	imGuiManager_->Begin();
	gameScene_->Update();


};

void SummarizeEngine::Draw() {
	//�`��
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

	//if (SUCCEEDED(dxCommon_->GetDevice()->QueryInterface(&debugInterface))) {
	//	debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
	//	debugInterface->Release();
	//}
	//���f�[�^���
	//delete[] imageData;
	//���͊J��

	delete gameScene_;
	delete imGuiManager_;
	delete dxCommon_;
	delete winApp_;

};