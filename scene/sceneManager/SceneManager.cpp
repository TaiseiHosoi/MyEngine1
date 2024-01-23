#include "SceneManager.h"
#include "Input.h"
#include <cassert>
#include "DirectXCommon.h"

// シーンのインクルード
#include "SceneIntegrate.h"


SceneManager::SceneManager(DirectXCommon* dxCommon, GameCamera* camera) {
	_dxCommon = dxCommon;
	
	_scene.reset(new TitleScene(&*this));
	_camera = camera;
	

	

}
SceneManager::~SceneManager() {

}
void SceneManager::ObjectInitialize() {

	//スプライト初期化
	spriteCommon_ = std::make_unique<SpriteCommon>();
	spriteCommon_->Initialize(_dxCommon);



	// TITLE
	spriteCommon_->LoadTexture("title1", "title.png");
	spriteCommon_->LoadTexture("title2", "title2.dds");
	spriteCommon_->LoadTexture("endScene", "end.png");

	// UI

	spriteCommon_->LoadTexture("sceneChangeTitle", "kinmiraTitle1.png");
	spriteCommon_->LoadTexture("leftMouseButton", "LeftMouseButton.png");
	spriteCommon_->LoadTexture("exp", "exp.png");
	spriteCommon_->LoadTexture("hpBar", "HpBar-sheet.png");
	spriteCommon_->LoadTexture("hpGage", "HpGage-sheet.png");
	spriteCommon_->LoadTexture("gameOverScene", "gameOver.png");
	spriteCommon_->LoadTexture("gameClearScene", "gameClear.png");
	spriteCommon_->LoadTexture("background", "background.png");
	spriteCommon_->LoadTexture("outFlame", "sotowaku2.png");

	spriteCommon_->LoadTexture("sceneCLeft", "sceneCLeft.png");
	spriteCommon_->LoadTexture("sceneCRight", "sceneCRight.png");

	audio = std::make_unique<Audio>();
	audio->Initialize();


	// fbx テスト
	{
		hitokunFbxM_.reset(FbxLoader::GetInstance()->LoadModelFromFile("Cube",true));
		//Player
		fbxPlayer_ = std::make_unique<FbxPlayer>();
		fbxPlayer_.get()->Initialize(hitokunFbxM_.get());
		fbxPlayer_->SetRailCameraInfo(_camera->GetRailCameraInfo());
		_camera->SetFollowerPos(fbxPlayer_.get()->GetObject3d()->GetWorldTransformPtr());

	}

	//暗転画像初期化
	blackSc_ = std::make_unique<Sprite>();
	blackSc_->Initialize(spriteCommon_.get(), "gameOverScene");
	blackSc_->SetPozition({ 0,0 });
	blackSc_->SetColor({ 1,1,1,blackScAlpha_ });

	//背景スプライト
	backGroundSp_ = std::make_unique<Sprite>();
	backGroundSp_->Initialize(spriteCommon_.get(), "background");
	backGroundSp_->SetPozition({ 0,0 });
	backGroundSp_->SetSize({WinApp::window_width,WinApp::window_height });

	//外枠スプライト
	outerFrameSp_ = std::make_unique<Sprite>();
	outerFrameSp_->Initialize(spriteCommon_.get(), "outFlame");
	outerFrameSp_->SetPozition({ 0,0 });
	outerFrameSp_->SetSize({ WinApp::window_width,WinApp::window_height });

	//パーティクルのセット
	particleManager_ = std::make_unique<ParticleManager>();
	particleManager_.get()->Initialize();
	//パーティクル用素材
	particleManager_->LoadTexture("effect.png");
	particleManager_->Update();

	//field
	field_ = std::make_unique<Field>();
	field_.get()->Initialize();

	ResetParameters();
}

void SceneManager::SceneInitialize() {
	_scene.get()->Initialize(_dxCommon,_camera);
	
}

void SceneManager::SceneUpdate(Input* input) {


	_scene.get()->Update(input,_camera);	
	BlackDisolve();
}

void SceneManager::SceneDraw() {
	
	spriteCommon_->SpritePreDraw();
	backGroundSp_->Draw();	//背景スプライト
	spriteCommon_->SpritePostDraw();

	_scene.get()->Draw(_dxCommon);
	
	if (isBlackDisolve_ == true) {
		blackSc_->Draw();
	}


}

void SceneManager::FrontSpriteSceneDraw()
{
	spriteCommon_->SpritePreDraw();
	outerFrameSp_->Draw();	//背景スプライト
	spriteCommon_->SpritePostDraw();
}

void SceneManager::ChangeScene(IScene* scene) {
	
	_camera->ChangeFollowFlag(true);
	_scene.reset(scene);
	ResetParameters();
	SceneInitialize();
	
	

}


void SceneManager::ResetParameters() {
	//boss_->SetHp(100);
	fbxPlayer_->SetHp(100);

	fbxPlayer_->GetObject3d()->Update();
}

void SceneManager::BlackDisolve()
{
	if (oldDisolveMode_ != nowDisolveMode_) {
		if (nowDisolveMode_ == DisolveMode::gameClearMode) {
			blackSc_->SetTextureByName("gameClearScene");
		}
		else if (nowDisolveMode_ == DisolveMode::gameClearMode) {
			blackSc_->SetTextureByName("gameOverScene");
		}
	}

	if (oldIsBlackDisolve_ == false && isBlackDisolve_ == true) {
		Input::GetInstance()->SetIsDontInput(true);
	}

	if (isBlackDisolve_ == true && isTurnBackDis_ == false) {
		blackScAlpha_ += 0.02f;
		if (blackScAlpha_ > maxBlackScAlpha_) {
			isTurnBackDis_ = true;
		}
		
	}
	else if (isBlackDisolve_ == true && isTurnBackDis_ == true) {
		blackScAlpha_ -= 0.02f;
		if (blackScAlpha_ < 0) {
			isTurnBackDis_ = false;
			isBlackDisolve_ = false;
			Input::GetInstance()->SetIsDontInput(false);
		}
		
	}
	
	//入力受付セット
	if (isBlackDisolve_ == true) {
		Input::GetInstance()->SetIsDontInput(true);
	}


	//α値設定
	blackSc_->SetColor({ 1.f,1.f,1.f,blackScAlpha_ });

	//前フレーム処理
	oldDisolveMode_ = nowDisolveMode_;
	oldIsBlackDisolve_ = isBlackDisolve_;
}

void SceneManager::SetIsBlackDisolve(bool isDisolve, int mode)
{
	isBlackDisolve_ = isDisolve;
	nowDisolveMode_ = mode;
}

bool SceneManager::GetIsTurnBackBlackDisolve()
{
	return isTurnBackDis_;
}

bool SceneManager::GetIsChangeScene_()
{
	return isChangeScene_;
}

void SceneManager::SetIsChangeScene_(bool arg)
{
	isChangeScene_ = arg;
}

void SceneManager::SetCollisionManager(CollisionManager* collisionManager)
{
	_collisionManager = collisionManager;
}
