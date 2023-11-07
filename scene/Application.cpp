#include "Application.h"

#include<sstream>
#include<iomanip>
#include"imgui.h"




Application::Application()
{
}

Application::~Application()
{
}

void Application::Initialize(DirectXCommon* dxcomon)
{
	assert(dxcomon);
	dxCommon_ = dxcomon;

	collisionManager_ = CollisionManager::GetInstance();

	//FBX	(テスト)
	// デバイスをセット
	FBXObject3d::SetDevice(dxCommon_->GetDevice());
	// グラフィックスパイプライン生成
	FBXObject3d::CreateGraphicsPipeline();

	//敵マネージャ初期化
	JsonManager_ = std::make_unique<GameObjManager>();
	JsonManager_->StaticInit();

	//かめら初期化
	gameCamera_ = std::make_unique<GameCamera>(WinApp::window_width, WinApp::window_height, input_);
	assert(gameCamera_);
	//カメラインスタンスにjson情報セット
	gameCamera_->SetJsonObj(JsonManager_->GetCamObjsPtr());
	gameCamera_->Initialize();
	//カメラのポインタをセット
	//カメラ位置セット
	gameCamera_->SetTarget({ 0 , 0 , 0 });

	Object3d::SetCamera(gameCamera_.get());
	FBXObject3d::SetCamera(gameCamera_.get());
	ParticleManager::SetCamera(gameCamera_.get());

	

	sceneManager_ = std::make_unique<SceneManager>(dxCommon_, gameCamera_.get());
	sceneManager_->SetJsonManager(JsonManager_.get());
	sceneManager_->ObjectInitialize();
	sceneManager_->SceneInitialize();
	sceneManager_->SetCollisionManager(collisionManager_);

	
	


	hitStopManager_ = HitStopManager::GetInstance();
	hitStopManager_->SetGameCamera(gameCamera_.get());
}

void Application::Update()
{

	collisionManager_->CheckAllCollisions();

	sceneManager_->SceneUpdate(input_);


	hitStopManager_->Update();

	//カメラのアップデート
	gameCamera_->Update();



}

void Application::Draw()
{
	sceneManager_->SceneDraw();
}

