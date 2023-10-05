#include "SceneIntegrate.h"

TitleScene::TitleScene(SceneManager* controller) {
	_controller = controller;
}
TitleScene::~TitleScene() {

}

void TitleScene::Initialize(DirectXCommon* dxCommon, GameCamera* camera) {

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

	titleCamerapos[0].initialize();
	titleCamerapos[0].translation_ = _controller->fbxPlayer_->GetObject3d()->GetWorldTransform().matWorld_.GetWorldPos();
	titleCamerapos[0].UpdateMatWorld();
	titleCamerapos[1].initialize();
	titleCamerapos[1].translation_ = Vector3(50, 10, 50);
	titleCamerapos[1].parent_ = &titleCamerapos[0];
	titleCamerapos[1].UpdateMatWorld();

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

	camera->SetEyePos(&titleCamerapos[1]);
	camera->SetTargetPos(&titleCamerapos[0]);

	titleCamerapos[0].rotation_.y += 0.001f;

	titleCamerapos[0].UpdateMatWorld();
	titleCamerapos[1].UpdateMatWorld();

	if (input->TriggerMouseButton(0)) {
		isSwapCamera = true;
		startCount = nowCount;
	}

	nowCount++; // 1フレーム当たりに増えるcount

	elapsedCount = nowCount - startCount;
	float elapsedTime = static_cast<float> (elapsedCount) / 1'000'000.0f;

	timeRate = elapsedTime / maxTime;

	if (isSwapCamera == true) {

		Vector3 tarVec, eyeVec, tarAns, eyeAns;
		Vector3 tar_a, tar_b, eye_c, eye_d;

		tar_a = titleCamerapos[0].matWorld_.GetWorldPos();
		tar_b = _controller->fbxPlayer_->GetObject3d()->GetWorldTransform().matWorld_.GetWorldPos();

		eye_c = titleCamerapos[1].matWorld_.GetWorldPos();
		eye_d = camera->swap_[0].translation_;

		tarVec = tar_b - tar_a;
		eyeVec = eye_d - eye_c;
		tarAns = tarVec.nomalize();
		eyeAns = eyeVec.nomalize();

		const float change = 4.5f;
		float ease = change;
		ease = static_cast<float>(Ease::InOutQuad(change, 0, 50, static_cast<int>(elapsedCount)));

		target.translation_ += tarAns * ease;
		eye.translation_ += eyeAns * ease;

		target.UpdateMatWorld();
		eye.UpdateMatWorld();
		camera->SetTargetPos(&target);
		camera->SetEyePos(&eye);

		if (ease >= change) {
			ease = change;
			isChangeScene = true;
		}
	}
}