/**
 * @file EndScene.h
 * @brief 終了シーン 基底クラス:IScene
 */
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

	// 初期化
	void Initialize(DirectXCommon* dxCommon, GameCamera* camera) override;

	// 更新
	void Update(Input* input, GameCamera* camera) override;

	// 描画
	void Draw(DirectXCommon* dxCommon) override;


private:
	std::unique_ptr <Sprite> end_;

	//音用変数
	std::unique_ptr<Audio> audio_;

public:
	
};