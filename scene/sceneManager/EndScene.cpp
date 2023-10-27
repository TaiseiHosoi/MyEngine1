#include "EndScene.h"
#include "SceneManager.h"
#include "WinApp.h"
#include "TitleScene.h"


EndScene::EndScene(SceneManager* controller) {
	_controller = controller;
}
EndScene::~EndScene() {

}

void EndScene::Initialize( DirectXCommon* dxCommon,GameCamera* camera) {

	//音の初期化と読み込み
	audio_ = std::make_unique<Audio>();
	audio_->Initialize();
	//audio_->LoadWave("newspaper.wav");
	
	end_ = std::make_unique<Sprite>();
	end_->Initialize(_controller->spriteCommon_.get(), 3);
	end_->SetSize({ WinApp::GetInstance()->window_width ,WinApp::GetInstance()->window_height });
	end_->SetPozition({ 0,0 });

	

	static_cast< void >(dxCommon);
	static_cast<void>(camera);
}

void EndScene::Update(Input* input, GameCamera* camera) {
	static_cast< void >(camera);

	//audio_->PlayWave();
	if (input->TriggerKey(DIK_SPACE)) {
		_controller->ChangeScene(new TitleScene(_controller));
	}// ここから下にコード書くとメモリ君がエラー吐く
}

void EndScene::Draw(DirectXCommon* dxCommon) {
	static_cast<void>(dxCommon);

	_controller->spriteCommon_->SpritePreDraw();
	end_->Draw();
	_controller->spriteCommon_->SpritePostDraw();


}