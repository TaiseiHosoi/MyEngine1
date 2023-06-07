#pragma once
#include "IScene.h"

class TitleScene :public IScene
{
protected:
	SceneManager* _controller;

public:
	
	TitleScene(SceneManager* controller);
	~TitleScene() override;

	void Initialize(DirectXCommon* dxCommon, GameCamera* camera) override;
	void Update(Input* input, GameCamera* camera) override;
	void Draw(DirectXCommon* dxCommon) override;
public:

	void ChangeCamera(Input* input,GameCamera* camera);

private:
	
	Input* input_ = Input::GetInstance();

private://リソース
	std::unique_ptr <Sprite> title_;
	std::unique_ptr <Sprite> title2_;
	std::unique_ptr <Sprite> mouse_;
private:
	//1->eye ,0-> target 
	WorldTransform titleCamerapos[2];
	WorldTransform swapCamerapos[2];

	WorldTransform target;
	WorldTransform eye;

	int fbxTimer_ = 0;

	bool isChangeScene = false;
	bool isSwapCamera = false;
	// 時間計測に必要なデータ
	size_t startCount = 0;
	size_t nowCount = 0;
	size_t elapsedCount = 0;
	float maxTime = 5.0f;
	float timeRate;

	//音用変数
	std::unique_ptr<Audio> audio_;

};