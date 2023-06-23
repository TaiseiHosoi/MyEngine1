#pragma once
#include "IScene.h"
#include "SceneManager.h"

// ボス 戦闘1
class GamePart2 :
    public IScene
{
protected:
	SceneManager* _controller;

public:
	GamePart2(SceneManager* controller);
	~GamePart2() override;

	void Initialize(DirectXCommon* dxCommon, GameCamera* camera) override;
	void Update(Input* input, GameCamera* camera) override;
	void Draw(DirectXCommon* dxCommon) override;

private:
	std::unique_ptr <Sprite> title_;
	std::unique_ptr <Sprite> title2_;

	std::unique_ptr<Audio>audio_;

	bool isChangeScene = false;
	bool isSwapCamera = false;

public:

};

