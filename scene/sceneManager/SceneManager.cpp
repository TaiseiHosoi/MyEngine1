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
	spriteCommon_->LoadTexture(1, "title.png");
	spriteCommon_->LoadTexture(2, "title2.png");
	spriteCommon_->LoadTexture(3, "end.png");

	// UI
	spriteCommon_->LoadTexture(6, "attack.png");
	spriteCommon_->LoadTexture(7, "attack2.png");
	spriteCommon_->LoadTexture(8, "guard.png");
	spriteCommon_->LoadTexture(9, "guard2.png");
	spriteCommon_->LoadTexture(10, "move.png");
	spriteCommon_->LoadTexture(11, "mouse.png");
	spriteCommon_->LoadTexture(12 , "purple.png");
	spriteCommon_->LoadTexture(13 , "red.png");
	spriteCommon_->LoadTexture(14 , "green.png");
	spriteCommon_->LoadTexture(15, "kinmiraTitle1.png");
	spriteCommon_->LoadTexture(16, "sceneCLeft.png");
	spriteCommon_->LoadTexture(17, "sceneCRight.png");
	spriteCommon_->LoadTexture(18, "LeftMouseButton.png");
	spriteCommon_->LoadTexture(19, "exp.png");
	spriteCommon_->LoadTexture(20, "HpBar-sheet.png");
	spriteCommon_->LoadTexture(21, "HpGage-sheet.png");
	spriteCommon_->LoadTexture(22, "gameOver.png");

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
	blackSc_->Initialize(spriteCommon_.get(), 22);
	blackSc_->SetPozition({ 0,0 });
	blackSc_->SetColor({ 1,1,1,blackScAlpha_ });

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
	/*ImGui::Begin("Info");
	ImGui::Text("E : particle");
	ImGui::Text("arrowkey : %f,%f,%f", fbxPlayer_->GetObject3d()->GetPosition().x, fbxPlayer_->GetObject3d()->GetPosition().y,fbxPlayer_->GetObject3d()->GetPosition().z);
	ImGui::End();*/

	_scene.get()->Update(input,_camera);	
	BlackDisolve();
}

void SceneManager::SceneDraw() {
	_scene.get()->Draw(_dxCommon);

	if (isBlackDisolve_ == true) {
		blackSc_->Draw();
	}


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
		}
		
	}
	blackSc_->SetColor({ 1.f,1.f,1.f,blackScAlpha_ });
}

void SceneManager::SetIsBlackDisolve(bool arg)
{
	isBlackDisolve_ = arg;
}

bool SceneManager::GetIsTurnBackBlackDisolve()
{
	return isTurnBackDis_;
}
