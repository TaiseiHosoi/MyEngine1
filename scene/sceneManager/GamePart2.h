/**
 * @file GamePart2.h
 * @brief ゲームシーン2 基底クラス:IScene
 */
#pragma once
#include "IScene.h"
#include "SceneManager.h"
using namespace MyEngine;

// ボス 戦闘1
class GamePart2 :
    public IScene
{
protected:
	SceneManager* _controller;

public:
	GamePart2(SceneManager* controller);
	~GamePart2() override;

	// 初期化
	void Initialize(DirectXCommon* dxCommon, GameCamera* camera) override;
	// 更新
	void Update(Input* input, GameCamera* camera) override;
	//描画
	void Draw(DirectXCommon* dxCommon) override;

private:
	std::unique_ptr <Sprite> title_;
	std::unique_ptr <Sprite> title2_;

	std::unique_ptr<Audio>audio_;

	bool isChangeScene = false;
	bool isSwapCamera = false;

public:

};

