#include "SceneManager.h"
#include "Input.h"
#include <cassert>
#include "DirectXCommon.h"

// シーンのインクルード
#include "SceneIntegrate.h"


SceneManager::SceneManager(DirectXCommon* dxCommon, GameCamera* camera) {
	_dxCommon = dxCommon;
	_scene.reset(new GamePart1(&*this));
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

	audio = std::make_unique<Audio>();
	audio->Initialize();


	// fbx テスト
	{
		hitokunFbxM_.reset(FbxLoader::GetInstance()->LoadModelFromFile("lowpoliInukun",true));
		//Player
		fbxPlayer_ = std::make_unique<FbxPlayer>();
		fbxPlayer_.get()->Initialize(hitokunFbxM_.get());
		fbxPlayer_->GetObject3d()->wtf.translation_.z = 0;
		_camera->SetFollowerPos(fbxPlayer_.get()->GetObject3d()->GetWorldTransformPtr());

		//boss
		boss_ = std::make_unique<Boss>();
		boss_.get()->Initialize(_dxCommon);
		boss_.get()->SetPlayer(fbxPlayer_.get());
		boss_->GetObject3d()->wtf.translation_.z = 50;
		_camera->SetTargetPos(boss_.get()->GetObject3d()->GetWorldTransformPtr());
	}

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
}

void SceneManager::SceneDraw() {
	_scene.get()->Draw(_dxCommon);

}

void SceneManager::ChangeScene(IScene* scene) {
	_camera->ChangeFollowFlag(true);
	_scene.reset(scene);
	ResetParameters();
	SceneInitialize();

}


void SceneManager::ResetParameters() {
	boss_->SetHp(100);
	fbxPlayer_->SetHp(100);
	boss_->Reset();
	boss_->GetObject3d()->wtf.translation_ = { -50,0,0 };
	boss_->Update();
	fbxPlayer_->GetObject3d()->wtf.translation_ = { 0,10,0 };
	fbxPlayer_->GetPlayerActionManager()->SetNowActNum(ACTION_NUM::move);
	fbxPlayer_->GetObject3d()->Update();
}