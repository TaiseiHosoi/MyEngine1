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

	blackBackTitle_ = std::make_unique<Sprite>();
	blackBackTitle_->Initialize(_controller->spriteCommon_.get(), 15);
	blackBackTitle_->SetPozition({ -1280,0 });

	sceneChangeLeft_ = std::make_unique<Sprite>();
	sceneChangeLeft_->Initialize(_controller->spriteCommon_.get(), 16);
	sceneChangeLeft_->SetPozition({ static_cast<float>( - WinApp::GetInstance()->window_width),0});

	sceneChangeRight_ = std::make_unique<Sprite>();
	sceneChangeRight_->Initialize(_controller->spriteCommon_.get(), 17);
	sceneChangeRight_->SetPozition({ static_cast<float>(WinApp::GetInstance()->window_width),0 });

	leftMouseButtonStr_ = std::make_unique<Sprite>();
	leftMouseButtonStr_->Initialize(_controller->spriteCommon_.get(), 18);
	leftMouseButtonStr_->SetPozition({ 0,0 });

	exp_ = std::make_unique<Sprite>();
	exp_->Initialize(_controller->spriteCommon_.get(), 19);
	exp_->SetPozition({_controller->toExpPos_.x ,_controller->toExpPos_.y });

	hpBar_ = std::make_unique<Sprite>();
	hpBar_->Initialize(_controller->spriteCommon_.get(), 20);
	hpBar_->SetPozition({ _controller->toHpSpritePos_.x ,_controller->toHpSpritePos_.y });

	hpGage_ = std::make_unique<Sprite>();
	hpGage_->Initialize(_controller->spriteCommon_.get(), 21);
	hpGage_->SetPozition({ _controller->toHpSpritePos_.x ,_controller->toHpSpritePos_.y });



	title_->SetSize({ 256,128 });
	title2_->SetSize({ 470,200 });

	title_->SetPozition({ 200,100 });
	title2_->SetPozition({ 200,100 });


	//ゲームオブジェクトクラスに情報セット
	_controller->gameObjectManager_->SetRailCamInfo(camera->GetRailCameraInfo());
	_controller->gameObjectManager_->SetPlayerWorldTF(_controller->fbxPlayer_->GetObject3d()->GetWorldTransformPtr());

	//ポップコマンドの設定
	_controller->gameObjectManager_->SetIsEnemyPops(false);
}

void TitleScene::Update(Input* input, GameCamera* camera) {
	_controller->field_->Update();
	_controller->fbxPlayer_->Update();
	_controller->gameObjectManager_->UpdateAllObjects();

	//audio_->PlayWave();

	ChangeCamera(input, camera);


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
	_controller->gameObjectManager_->DrawAllObjs(dxCommon->GetCommandList());

	_controller->spriteCommon_->SpritePreDraw();
	if (isSwapCamera == true) {

	}
	else {
		title2_->Draw();
		leftMouseButtonStr_->Draw();
	}

	exp_->Draw();
	hpBar_->Draw();
	hpGage_->Draw();

	sceneChangeLeft_->Draw();
	sceneChangeRight_->Draw();
	blackBackTitle_->Draw();

	_controller->spriteCommon_->SpritePostDraw();
}

void TitleScene::ChangeCamera(Input* input, GameCamera* camera) {

	static_cast<void>(camera);
	oldSSCount_ = sceneSwapPhase_;	//前フレーム処理


	if (input->TriggerMouseButton(MOUSE_KEY::LEFT)) {
		sceneSwapPhase_ = SceneSwapPhaseNum::START;
	}


	if (sceneSwapPhase_ == SceneSwapPhaseNum::START) {
		sceneSwapCount_ = 0;
		oldSSCount_ = 0;
		sceneSwapPhase_ = 0;
		sceneSwapPhase_ = SceneSwapPhaseNum::MOVE1;

	}else if (sceneSwapPhase_ == SceneSwapPhaseNum::MOVE1) {
		sceneSwapCount_++;
		easeStrength_ = 3.f;
		float nowScrollVec = Ease::LinearEaseOutEasing(static_cast<float>(WinApp::GetInstance()->window_width), 0, static_cast<int>(sceneSwapCount_), static_cast<int>(MAX_SWAP_COUNT),easeStrength_);
		sceneChangeLeft_->SetPozition({ -nowScrollVec ,0 });
		sceneChangeRight_->SetPozition({ nowScrollVec ,0 });

		if (sceneSwapCount_ >= MAX_SWAP_COUNT) {
			sceneSwapPhase_ = SceneSwapPhaseNum::DIRECTION1;
			sceneSwapCount_ = 0;
			isSwapCamera = true;
			camera->SetCamMode(CAM_MODE::startDirection);
			camera->ResetGameCam();
			
		}

	}
	else if (sceneSwapPhase_ == SceneSwapPhaseNum::DIRECTION1) {
		sceneSwapCount_++;

		easeStrength_ = offsetEaseStrength_;
		float nowEaseTransVecX = Ease::LinearEaseOutEasing(static_cast<float>(-WinApp::GetInstance()->window_width), 0, static_cast<int>(sceneSwapCount_), static_cast<int>(MAX_BACK_SWAP_COUNT),easeStrength_);
		blackBackTitle_->SetPozition({ nowEaseTransVecX ,0 });
		if (sceneSwapCount_ >= MAX_BACK_SWAP_COUNT) {
			sceneSwapPhase_ = SceneSwapPhaseNum::DIRECTION2;
			sceneSwapCount_ = 0;
		}

	}
	else if (sceneSwapPhase_ == SceneSwapPhaseNum::DIRECTION2) {
		sceneSwapCount_++;

		easeStrength_ = offsetEaseStrength_;
		float nowEaseTransVecX = Ease::LinearEasing(0,static_cast<float>(WinApp::GetInstance()->window_width), static_cast<int>(sceneSwapCount_), static_cast<int>(MAX_BACK_SWAP_COUNT),easeStrength_);
		blackBackTitle_->SetPozition({ nowEaseTransVecX ,0 });
		if (sceneSwapCount_ >= MAX_BACK_SWAP_COUNT) {
			sceneSwapPhase_ = SceneSwapPhaseNum::MOVE2;
			sceneSwapCount_ = 0;
			camera->SetIsCountInc(true);
		}

	}
	else if (sceneSwapPhase_ == SceneSwapPhaseNum::MOVE2) {
		sceneSwapCount_++;

		easeStrength_ = offsetEaseStrength_;
		float nowScrollVec = Ease::LinearEaseOutEasing(0, static_cast<float>(WinApp::GetInstance()->window_height), static_cast<int>(sceneSwapCount_), static_cast<int>(MAX_SWAP_COUNT),easeStrength_);
		float hpSpPosX = Ease::LinearEaseOutEasing(_controller->toHpSpritePos_.x, _controller->offsetHpSpritePos_.x, static_cast<int>(sceneSwapCount_), static_cast<int>(MAX_SWAP_COUNT), easeStrength_);
		float expSpPosX = Ease::LinearEaseOutEasing(_controller->toExpPos_.x, _controller->offsetExpPos_.x, static_cast<int>(sceneSwapCount_), static_cast<int>(MAX_SWAP_COUNT), easeStrength_);

		sceneChangeLeft_->SetPozition({ 0 ,-nowScrollVec });
		sceneChangeRight_->SetPozition({ 0 ,nowScrollVec });

		hpBar_->SetPozition({ hpSpPosX ,_controller->offsetHpSpritePos_.y });
		hpGage_->SetPozition({ hpSpPosX ,_controller->offsetHpSpritePos_.y });
		exp_->SetPozition({ expSpPosX ,_controller->offsetExpPos_.y });

		if (sceneSwapCount_ >= MAX_SWAP_COUNT) {
			sceneSwapPhase_ = SceneSwapPhaseNum::END;
			sceneSwapCount_ = 0;
		}

	}else if (sceneSwapPhase_ == SceneSwapPhaseNum::END) {
		isSwapCamera = true;
		isChangeScene = true;
		

	}

	nowCount++; // 1フレーム当たりに増えるcount


}