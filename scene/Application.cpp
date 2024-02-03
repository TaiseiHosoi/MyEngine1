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



	//かめら初期化
	gameCamera_ = std::make_unique<GameCamera>(WinApp::window_width, WinApp::window_height, input_);
	assert(gameCamera_);

	//カメラのポインタをセット
	//カメラ位置セット
	gameCamera_->SetTarget({ 0 , 0 , 0 });

	Object3d::SetCamera(gameCamera_.get());
	FBXObject3d::SetCamera(gameCamera_.get());
	ParticleManager::SetCamera(gameCamera_.get());

	//敵マネージャ初期化
	gameObjManager_ = std::make_unique<GameObjManager>();
	gameObjManager_->StaticInit();

	//カメラインスタンスにjson情報セット
	gameCamera_->SetJsonObj(gameObjManager_->GetCamObjsPtr());
	gameCamera_->GameInfoInitialize();
	gameObjManager_->RailCameraInit(gameCamera_->GetRailCameraInfo());	//

	//ゲームオブジェクト管理クラスにカメラセット
	gameObjManager_->SetGameCamPtr(gameCamera_.get());
	gameObjManager_->GameObjInitialize();	//カメラ情報が必要なため隔離されている

	//その他シーンマネージャの初期化
	sceneManager_ = std::make_unique<SceneManager>(dxCommon_, gameCamera_.get());
	sceneManager_->SetJsonManager(gameObjManager_.get());
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

void Application::PostEffectDraw()
{
	sceneManager_->SceneDraw();
}

void Application::OutFlameDraw()
{
	sceneManager_->FrontSpriteSceneDraw();
}

