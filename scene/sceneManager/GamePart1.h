/**
 * @file GamePart1.h
 * @brief ゲームシーン1 基底クラス:IScene
 */
#pragma once
#include "IScene.h"
#include "GameObjManager.h"

// 追いかけパート
class GamePart1 :
    public IScene
{
protected:
	SceneManager* _controller;

public:
	GamePart1(SceneManager* controller);
	~GamePart1() override;

	// 初期化
	void Initialize(DirectXCommon* dxCommon, GameCamera* camera) override;

	// 更新
	void Update(Input* input, GameCamera* camera) override;

	// 描画
	void Draw(DirectXCommon* dxCommon) override;

	// ポーズシーン切り替え
	void Pause(Input* input, GameCamera* camera);

	// サウンド再生
	void PlaySounds();

private:
	enum PauseMenu{
		RESUME,
		BACK_TO_TITLE,
		END_OF_OPTION,
	};

private:
	std::unique_ptr <Sprite> attack_;
	std::unique_ptr <Sprite> attack2_;
	std::unique_ptr <Sprite> guard_;
	std::unique_ptr <Sprite> guard2_;
	std::unique_ptr <Sprite> move_;
	std::unique_ptr <Sprite> enemyHp_;
	std::unique_ptr <Sprite> enemyHpRed_;
	std::unique_ptr <Sprite> playerHp_;
	std::unique_ptr <Sprite> playerHpRed_;

	bool isClickL, isClickR;

	//ポーズメニュー用変数
	int isPause_;
	int pauseMenuOptions_;
	bool backToTitle_;

	//音用変数
	bool isSounds = false;
	std::unique_ptr<Audio> audio_;

	


};

