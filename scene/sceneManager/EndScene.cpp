#include "EndScene.h"
#include "SceneManager.h"

#include "TitleScene.h"

EndScene::EndScene(SceneManager* controller) {
	_controller = controller;
}
EndScene::~EndScene() {

}

void EndScene::Initialize(DirectXCommon* dxCommon, GameCamera* camera) {

	//���̏������Ɠǂݍ���
	audio_ = std::make_unique<Audio>();
	audio_->Initialize();
	//audio_->LoadWave("newspaper.wav");

	end_ = std::make_unique<Sprite>();
	end_->Initialize(_controller->spriteCommon_.get(), 3);
	end_->SetSize({ 1280,720 });
	end_->SetPozition({ 0,0 });

	_controller->fbxPlayer_->GetObject3d()->SetPosition(Vector3(30, 0, 0));
}

void EndScene::Update(Input* input, GameCamera* camera) {

	//audio_->PlayWave();
	if (input->TriggerKey(DIK_SPACE)) {
		_controller->ChangeScene(new TitleScene(_controller));
	}// �������牺�ɃR�[�h�����ƃ������N���G���[�f��
}

void EndScene::Draw(DirectXCommon* dxCommon) {
	_controller->spriteCommon_->SpritePreDraw();
	end_->Draw();
	_controller->spriteCommon_->SpritePostDraw();


}