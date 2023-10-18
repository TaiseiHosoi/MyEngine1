#include "SceneIntegrate.h"



GamePart1::GamePart1(SceneManager* controller) {
	_controller = controller;

}

GamePart1::~GamePart1() {

}

void GamePart1::Initialize(DirectXCommon* dxCommon, GameCamera* camera) {

	static_cast<void>(dxCommon);

	move_ = std::make_unique<Sprite>();
	move_->Initialize(_controller->spriteCommon_.get(), 10);
	move_->SetSize({ 128,80 });
	move_->SetPozition({ 100,550 });

	////音の初期化と読み込み
	//audio_ = std::make_unique<Audio>();
	//audio_->Initialize();
	//audio_->LoadWave("newspaper.wav");



	isPause_ = false;
	pauseMenuOptions_ = 0;
	backToTitle_ = 0;

	//ゲームオブジェクトクラスに情報セット
	_controller->gameObjectManager_->SetRailCamInfo(camera->GetRailCameraInfo());
	_controller->gameObjectManager_->SetPlayerWorldTF(_controller->fbxPlayer_->GetObject3d()->GetWorldTransformPtr());
	
	//camera->ResetGameCam();

}

void GamePart1::Update(Input* input, GameCamera* camera) {

	//BGMを流す
	//PlaySounds();



	if (isPause_ == false) {
		//test-----------------------------//

		if (camera->GetRailCameraInfo()->nowCount == 200 || camera->GetRailCameraInfo()->nowCount == 800) {

			_controller->gameObjectManager_->AddEnemy(0,0,{0,0,0});
			_controller->gameObjectManager_->GetWalkingEnemies()->back()->SetRailCameraInfo(camera->GetRailCameraInfo());	//レールカメラ情報をセット
			_controller->gameObjectManager_->GetWalkingEnemies()->back()->SetPlayerWorldTransform(_controller->fbxPlayer_->GetObject3d()->GetWorldTransformPtr());	//
		}


		//test-----------------------------//


		_controller->field_->Update();
		_controller->fbxPlayer_->Update();
		_controller->gameObjectManager_->UpdateAllObjects();

		if (input->TriggerKey(DIK_ESCAPE)) {
			isPause_ = true;
			pauseMenuOptions_ = 0;
		}
		/*ImGui::Begin("Pause");
		ImGui::SetWindowPos({200 , 200});
		ImGui::InputInt("isPause" , &isPause_);
		ImGui::End();*/
	}
	else {
		Pause(input, camera);
	}



	//playerHp_->SetSize({ 300 * _controller->fbxPlayer_->GetHp() / 100.0f, 32 });
	//enemyHp_->SetSize({1280.0f * _controller->boss_->GetHp() / 100.0f , 32});


	// ここから下にコード書くとメモリ君がエラー吐く
}

void GamePart1::Draw(DirectXCommon* dxCommon) {

	_controller->field_->Draw(dxCommon);
	//_controller->boss_->Draw();
	_controller->fbxPlayer_->Draw(dxCommon->GetCommandList());

	_controller->gameObjectManager_->DrawAllEnemies(dxCommon->GetCommandList());

	_controller->spriteCommon_->SpritePreDraw();

	move_->Draw();


	_controller->spriteCommon_->SpritePostDraw();

}

void GamePart1::Pause(Input* input, GameCamera* camera)
{

	//ESCでポーズ解除
	if (input->TriggerKey(DIK_ESCAPE)) {
		isPause_ = false;
	}

	//矢印キー上下で選択肢の変更
	if (input->TriggerKey(DIK_DOWN)) {
		if (pauseMenuOptions_ < END_OF_OPTION - 1)
			pauseMenuOptions_++;
	}
	else if (input->TriggerKey(DIK_UP)) {
		if (0 < pauseMenuOptions_)
			pauseMenuOptions_--;
	}

	//エラー回避用
	if (pauseMenuOptions_ < 0) {
		pauseMenuOptions_ = 0;
	}
	if (END_OF_OPTION <= pauseMenuOptions_) {
		pauseMenuOptions_ = END_OF_OPTION - 1;
	}

	//エンターキーを押したとき
	if (input->TriggerKey(DIK_RETURN)) {

		switch (pauseMenuOptions_)
		{
			//再開する
		case GamePart1::RESUME:
			isPause_ = false;
			break;

			//タイトルに戻る
		case GamePart1::BACK_TO_TITLE:
			backToTitle_ = true;
			isPause_ = false;
			break;

		default:
			break;
		}
	}
	/*ImGui::Begin("Pause");
	ImGui::SetWindowPos({200 , 200});
	ImGui::InputInt("PauseMenu",&pauseMenuOptions_);
	ImGui::End();*/

	camera->SetIsPause(isPause_);
}

void GamePart1::PlaySounds()
{
	if (isSounds == false)
	{
		isSounds = true;
		audio_->PlayWave("newspaper.wav"); //ループ再生はしない
	}
}
