#include "SceneIntegrate.h"

TitleScene::TitleScene(SceneManager* controller) {
	_controller = controller;
}
TitleScene::~TitleScene() {

}

void TitleScene::Initialize(DirectXCommon* dxCommon, GameCamera* camera) {
	static_cast<void>(dxCommon);

	//音の初期化と読み込み
		//音の初期化と読み込み
	audio_ = std::make_unique<Audio>();
	audio_->Initialize();
	//audio_->LoadWave("newspaper.wav");

	title_ = std::make_unique<Sprite>();
	title_->Initialize(_controller->spriteCommon_.get(), 1);
	title2_ = std::make_unique<Sprite>();
	title2_->Initialize(_controller->spriteCommon_.get(), 2);
	mouse_ = std::make_unique<Sprite>();
	mouse_->Initialize(_controller->spriteCommon_.get(), 11);

	title_->SetSize({ 256,128 });
	title2_->SetSize({ 256,128 });
	title_->SetPozition({ 200,100 });
	title2_->SetPozition({ 200,100 });
	mouse_->SetSize({ 80,80 });
	mouse_->SetPozition({ 1100,600 });


	/*target.initialize();
	target = titleCamerapos[0];
	target.UpdateMatWorld();

	eye.initialize();
	eye = titleCamerapos[1];
	eye.UpdateMatWorld();*/

	//_controller->fbxPlayer_->Update();
	/*camera->Update();
	camera->SetEyePos(&titleCamerapos[1]);
	camera->SetTargetPos(&titleCamerapos[0]);
	camera->ChangeFollowFlag(false);*/

	/*_controller->boss_->SetHp(100);*/

	//ゲームオブジェクトクラスに情報セット
	_controller->gameObjectManager_->SetRailCamInfo(camera->GetRailCameraInfo());
	_controller->gameObjectManager_->SetPlayerWorldTF(_controller->fbxPlayer_->GetObject3d()->GetWorldTransformPtr());
}

void TitleScene::Update(Input* input, GameCamera* camera) {
	_controller->field_->Update();
	_controller->fbxPlayer_->Update();
	
	//audio_->PlayWave();

	ChangeCamera(input,camera);

	

	if (isChangeScene == true) {
		camera->SetFollowerPos(_controller->fbxPlayer_.get()->GetObject3d()->GetWorldTransformPtr());
		//camera->SetTargetPos(_controller->boss_.get()->GetObject3d()->GetWorldTransformPtr());
		// UPDATE の一番下に
		camera->SetCamMode(1);
		_controller->ChangeScene(new GamePart1(_controller));
		
	}// ここから下にコード書くとメモリ君がエラー吐く
}

void TitleScene::Draw(DirectXCommon* dxCommon) {

	_controller->fbxPlayer_->Draw(dxCommon->GetCommandList());
	_controller->field_->Draw(dxCommon);

	_controller->spriteCommon_->SpritePreDraw();
	if (isSwapCamera == true) {
		title_->Draw();
	}
	else {
		title2_->Draw();
		mouse_->Draw();
	}
	_controller->spriteCommon_->SpritePostDraw();
}

void TitleScene::ChangeCamera(Input* input, GameCamera* camera) {

	static_cast<void>(camera);

	if (input->TriggerMouseButton(0)) {
		isSwapCamera = true;
		startCount = nowCount;
		isChangeScene = true;
	}

	nowCount++; // 1フレーム当たりに増えるcount

	
}