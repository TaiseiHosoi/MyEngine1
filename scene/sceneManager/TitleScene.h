/**
 * @file TitleScene.h
 * @brief タイトルシーン 基底クラス:IScene
 */
#pragma once
#include "IScene.h"



class TitleScene :public IScene
{
protected:
	SceneManager* _controller;

public:
	
	TitleScene(SceneManager* controller);
	~TitleScene() override;

	// 初期化
	void Initialize(DirectXCommon* dxCommon, GameCamera* camera) override;
	// 更新
	void Update(Input* input, GameCamera* camera) override;
	// 描画
	void Draw(DirectXCommon* dxCommon) override;
public:

	// カメラのモード切り替え
	void ChangeCamera(Input* input,GameCamera* camera);

private:
	
	Input* input_ = Input::GetInstance();

private://リソース
	std::unique_ptr <Sprite> title_;
	std::unique_ptr <Sprite> title2_;
	std::unique_ptr <Sprite> mouse_;
	std::unique_ptr <Sprite> blackBackTitle_;
	std::unique_ptr <Sprite> sceneChangeLeft_;
	std::unique_ptr <Sprite> sceneChangeRight_;
	std::unique_ptr <Sprite> leftMouseButtonStr_;
	std::unique_ptr <Sprite> exp_;
	std::unique_ptr <Sprite> hpBar_;
	std::unique_ptr <Sprite> hpGage_;
private:

	enum SceneSwapPhaseNum {
		NONE,
		START,
		MOVE1,
		DIRECTION1,
		DIRECTION2,
		MOVE2,
		END
	};

	WorldTransform target;
	WorldTransform eye;

	int fbxTimer_ = 0;

	bool isChangeScene = false;
	bool isSwapCamera = false;
	// 時間計測に必要なデータ
	size_t startCount = 0;
	size_t nowCount = 0;
	float maxTime = 5.0f;
	float timeRate;

	// シーン切り替え演出用
	size_t sceneSwapCount_ = 0;
	const size_t MAX_SWAP_COUNT = 30;
	const size_t MAX_BACK_SWAP_COUNT = 45;
	size_t oldSSCount_ = 0;
	size_t sceneSwapPhase_ = 0;
	float easeStrength_ = 0;

	//音用変数
	std::unique_ptr<Audio> audio_;



};