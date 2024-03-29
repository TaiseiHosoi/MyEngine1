#include "SceneIntegrate.h"
using namespace MyEngine;


GamePart1::GamePart1(SceneManager* controller) {
	_controller = controller;

}

GamePart1::~GamePart1() {

}

void GamePart1::Initialize(DirectXCommon* dxCommon, GameCamera* camera) {

	static_cast<void>(dxCommon);

	//音の初期化と読み込み
	audio_ = std::make_unique<Audio>();
	audio_->Initialize();
	audio_->LoadWave("newspaper.wav");

	isPause_ = false;
	pauseMenuOptions_ = 0;
	backToTitle_ = 0;

	//ゲームオブジェクトクラスに情報セット
	_controller->gameObjectManager_->SetRailCamInfo(camera->GetRailCameraInfo());
	_controller->gameObjectManager_->SetPlayerWorldTF(_controller->gameObjectManager_->GetPlayerPtr()->GetObject3d()->GetWorldTransformPtr());
	exp_ = std::make_unique<Sprite>();
	exp_->Initialize(_controller->spriteCommon_.get(), "exp");
	exp_->SetPozition({ _controller->offsetExpPos_.x ,_controller->offsetExpPos_.y });

	hpBar_ = std::make_unique<Sprite>();
	hpBar_->Initialize(_controller->spriteCommon_.get(), "hpBar");
	hpBar_->SetPozition({ _controller->offsetHpSpritePos_.x ,_controller->offsetHpSpritePos_.y });
	hpBar_->SetAnchorPoint({0.5,1.f});
	hpBar_->Update();

	hpGage_ = std::make_unique<Sprite>();
	hpGage_->Initialize(_controller->spriteCommon_.get(), "hpGage");
	hpGage_->SetPozition({ _controller->offsetHpSpritePos_.x ,_controller->offsetHpSpritePos_.y });

	
	//camera->ResetGameCam();
	gameSceneMode_ = GAME_SCENE_MODE::inGame;

	//ポップコマンドモードセット
	_controller->gameObjectManager_->SetIsEnemyPops(true);
	_controller->gameObjectManager_->InitEnemyCommands();
	_controller->gameObjectManager_->InitOjamaFence();
	
	

}

void GamePart1::Update(Input* input, GameCamera* camera) {

	//BGMを流す
	PlaySounds();

	if (isPause_ == false) {

		_controller->field_->Update();
		_controller->gameObjectManager_->GetPlayerPtr()->Update();
		_controller->gameObjectManager_->UpdateAllObjects();
		HpFlucture();

		if (input->TriggerKey(DIK_ESCAPE)) {
			isPause_ = true;
			pauseMenuOptions_ = PauseMenu::RESUME;
		}

		if (_controller->gameObjectManager_->GetPlayerPtr()->GetIsDeadActNum() == DEAD_ACT_NUM::disappear) {
			_controller->SetIsBlackDisolve(true,DisolveMode::gameOverMode);
			if (_controller->GetIsTurnBackBlackDisolve() == true) {
				_controller->gameObjectManager_->GetPlayerPtr()->PlayerPalamReset();
				_controller->gameObjectManager_->DestroyAllEnemies();
				camera->SetCamMode(CAM_MODE::title);
				_controller->ChangeScene(new TitleScene(_controller));
			}
		}

		if (camera->GetIsGameClearDirectionEnd() == true) {	//クリア演出終了
			_controller->SetIsBlackDisolve(true,DisolveMode::gameClearMode);

			if (_controller->GetIsTurnBackBlackDisolve() == true) {
				_controller->gameObjectManager_->GetPlayerPtr()->PlayerPalamReset();
				_controller->gameObjectManager_->DestroyAllEnemies();
				camera->SetCamMode(CAM_MODE::title);
				_controller->ChangeScene(new TitleScene(_controller));
				camera->SetIsGameClearDirectionEnd(false);
			}
		}
		gameCount_++;
		//if (_controller->gameObjectManager_->GetPlayerPtr()->GetHp() <= 0) {
		//	camera->GoGameOver();
		//	_controller->gameObjectManager_->GetPlayerPtr()->GoGameOver();
		//	gameSceneMode_ = GAME_SCENE_MODE::gameOver;
		//	gameCount_ = 0;
		//
		//}
		if ( gameCount_ == 2000) {
			camera->GoGameClear();
			gameSceneMode_ = GAME_SCENE_MODE::gameClear;
			gameCount_ = 0;
		}
		else if (gameCount_ > 2000) {
			gameCount_ = 0;
		}
		


	}
	else {
		Pause(input, camera);
	}


	// ここから下にコード書くとメモリ君がエラー吐く
}

void GamePart1::Draw(DirectXCommon* dxCommon) {

	_controller->field_->Draw(dxCommon);
	//_controller->boss_->Draw();
	_controller->gameObjectManager_->GetPlayerPtr()->Draw(dxCommon->GetCommandList());

	_controller->gameObjectManager_->DrawAllObjs(dxCommon->GetCommandList());
	_controller->gameObjectManager_->GetPlayerPtr()->ParticleDraw(dxCommon->GetCommandList());

	_controller->spriteCommon_->SpritePreDraw();

	exp_->Draw();
	hpBar_->Draw();
	if (gameSceneMode_ != GAME_SCENE_MODE::gameOver) {
		hpGage_->Draw();
	}



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

void GamePart1::HpFlucture()
{
	float nowHp = static_cast<float>(_controller->gameObjectManager_->GetPlayerPtr()->GetHp());
	//10段階
	if (nowHp >= 0) {
		hpGage_->SetSize({ 80.f,nowHp / 100.f * 160.f });
	}
}
