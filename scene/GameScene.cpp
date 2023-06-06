#include "GameScene.h"

#include<sstream>
#include<iomanip>
#include"imgui.h"




GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize(DirectXCommon* dxcomon)
{
	assert(dxcomon);
	dxCommon_ = dxcomon;

	collisionManager_ = CollisionManager::GetInstance();

	//FBX	(テスト)
	// デバイスをセット
	FBXObject3d::SetDevice(dxCommon_->GetDevice());
	// グラフィックスパイプライン生成
	FBXObject3d::CreateGraphicsPipeline();

	//かめら初期化
	gameCamera_ = std::make_unique<GameCamera>(WinApp::window_width, WinApp::window_height, input_);
	assert(gameCamera_);

	//カメラのポインタをセット
	//カメラ位置セット
	//gameCamera_->SetTarget(hitokunFbxO_.get()->wtf.translation_);
	gameCamera_->SetEye({ 0 , 5 , -20 });
	gameCamera_->SetTarget({ 0 , 0 , 0 });

	Object3d::SetCamera(gameCamera_.get());
	FBXObject3d::SetCamera(gameCamera_.get());
	ParticleManager::SetCamera(gameCamera_.get());

	sceneManager_ = std::make_unique<SceneManager>(dxCommon_, gameCamera_.get());
	sceneManager_->ObjectInitialize();
	sceneManager_->SceneInitialize();

	hitStopManager_ = HitStopManager::GetInstance();
	hitStopManager_->SetGameCamera(gameCamera_.get());
}

void GameScene::Update()
{
	//gameCamera_->Update();

	collisionManager_->CheckAllCollisions();

	sceneManager_->SceneUpdate(input_);

	hitStopManager_->Update();

	//カメラのアップデート
	gameCamera_->Update();

}

void GameScene::Draw()
{
	sceneManager_->SceneDraw();
}

