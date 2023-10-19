#include "SceneIntegrate.h"


GamePart2::GamePart2(SceneManager* controller) {
	_controller = controller;
}

GamePart2::~GamePart2() {

}

void GamePart2::Initialize(DirectXCommon* dxCommon, GameCamera* camera) {

	static_cast<void>(camera);
	static_cast<void>(dxCommon);

	//音の初期化と読み込み
	audio_ = std::make_unique<Audio>();
	audio_->Initialize();
	//audio_->LoadWave("newspaper.wav");

	title_ = std::make_unique<Sprite>();
	title_->Initialize(_controller->spriteCommon_.get(), 1);
	title2_ = std::make_unique<Sprite>();
	title2_->Initialize(_controller->spriteCommon_.get(), 2);

	title_->SetSize({ 256,128 });
	title2_->SetSize({ 256,128 });
	title_->SetPozition({ 200,100 });
	title2_->SetPozition({ 200,100 });
	isChangeScene_ = false;
	isSwapCamera_ = false;

}

void GamePart2::Update(Input* input, GameCamera* camera) {
	static_cast<void>(camera);

	//audio_->PlayWave();
	if (input->TriggerKey(DIK_SPACE)) {
		_controller->ChangeScene(new TitleScene(_controller));
	}// ここから下にコード書くとメモリ君がエラー吐く
}

void GamePart2::Draw(DirectXCommon* dxCommon) {
	static_cast<void>(dxCommon);

	_controller->spriteCommon_->SpritePreDraw();
	if (isSwapCamera_ == true) {
		title2_->Draw();
	}
	else {
		title_->Draw();
	}
	_controller->spriteCommon_->SpritePostDraw();
}