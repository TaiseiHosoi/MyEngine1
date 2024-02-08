/**
 * @file GamePart1.h
 * @brief ゲームシーン1 基底クラス:IScene
 */
#pragma once
#include "IScene.h"
#include "GameObjManager.h"

using namespace MyEngine;

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

	//hp変動
	void HpFlucture();


private:
	enum PauseMenu{
		RESUME,
		BACK_TO_TITLE,
		END_OF_OPTION,
	};

private:

	std::unique_ptr <Sprite> enemyHp_;
	std::unique_ptr <Sprite> enemyHpRed_;
	std::unique_ptr <Sprite> playerHp_;
	std::unique_ptr <Sprite> playerHpRed_;

	std::unique_ptr <Sprite> exp_;

	//hp
	std::unique_ptr <Sprite> hpBar_;
	std::unique_ptr <Sprite> hpGage_;
	

	bool isClickL, isClickR;

	//ポーズメニュー用変数
	bool isPause_;
	int pauseMenuOptions_;
	bool backToTitle_;

	//音用変数
	bool isSounds = false;
	std::unique_ptr<Audio> audio_;

	//画像用固定変数
	const Vector2 moveSprPos_ = { 100,550 };
	const Vector2 moveSprSize_ = { 128,80 };

	//現在のモード
	enum GAME_SCENE_MODE{
		inGame,
		gameOver,
		gameClear

	};
	int gameSceneMode_ = 0;
	int gameCount_ = 0;


};

