#pragma once
#include "IScene.h"

class EndScene :
    public IScene
{
protected:
	SceneManager* _controller;

public:
	EndScene(SceneManager* controller);
	~EndScene() override;

	void Initialize(DirectXCommon* dxCommon, GameCamera* camera) override;
	void Update(Input* input, GameCamera* camera) override;
	void Draw(DirectXCommon* dxCommon) override;


private:
	std::unique_ptr <Sprite> end_;

	//���p�ϐ�
	std::unique_ptr<Audio> audio_;

public:
	
};