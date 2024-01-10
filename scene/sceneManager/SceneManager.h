/**
 * @file SceneManager.h
 * @brief シーン管理クラス
 */
#pragma once
#include <memory>
#include <stack>


#include "FBXObject3d.h"
#include "fbx/FBXLoader.h"
#include "FBXModel.h"
#include "Audio.h"

#include "Mesh.h"
#include "Object3d.h"
#include "ParticleManager.h"
#include "GameObjManager.h"

#include "FbxPlayer.h"
#include "Ground.h"
#include "Sprite.h"
#include "SpriteCommon.h"

using namespace MyEngine;

class Input;
class IScene;
class MyEngine::DirectXCommon;
class GameCamera;



enum DisolveMode {
	gameClearMode = 23,
	gameOverMode = 22
};
// シーン管理クラス
class SceneManager
{
private:
	std::shared_ptr<IScene> _scene;
	DirectXCommon* _dxCommon;
	GameCamera* _camera;
public:



	SceneManager(DirectXCommon* dxCommon,GameCamera* camera);
	~SceneManager();	

	//オブジェクトのInitializeを呼び出す
	void ObjectInitialize();
	// 各シーンのInitializeを呼び出す
	void SceneInitialize();
	// 各シーンのUpdateを呼び出す
	void SceneUpdate(Input* input);
	// 各シーンのDrawを呼び出す
	void SceneDraw();
	void OffsetBackSceneDraw();

	// シーンを変更する
	void ChangeScene(IScene*);

	// ゲームのシーン設定をリセットする(敵の発生情報など)
	void ResetParameters();

	// jsonManager情報をセット
	void SetJsonManager(GameObjManager* jsonmanager) { gameObjectManager_ = jsonmanager; };

	//暗転
	void BlackDisolve();

	//暗転フラグ
	void SetIsBlackDisolve(bool isDisolve,int mode);
	bool GetIsTurnBackBlackDisolve();

	//シーンチェンジしたかどうかをGet,Set
	bool GetIsChangeScene_();
	void SetIsChangeScene_(bool arg);

	//コリジョンマネージャセット
	void SetCollisionManager(CollisionManager* collisionManager);

public:
	
	std::unique_ptr <Audio> audio;
	std::unique_ptr <SpriteCommon> spriteCommon_;
	CollisionManager* _collisionManager = nullptr;

	//暗転用
	std::unique_ptr <Sprite> blackSc_;
	float blackScAlpha_ = 0;
	const float maxBlackScAlpha_ = 4.0f;
	float alphaDisSpeed_ = 0.02f;
	bool oldIsBlackDisolve_ = false;
	bool isBlackDisolve_ = false;
	bool isTurnBackDis_ = false;
	int nowDisolveMode_ = 0;	//暗転画像切り替えの為
	int oldDisolveMode_ = 0;

	//背景スプライト
	std::unique_ptr <Sprite> backGroundSp_;
	std::unique_ptr <Sprite> outerFrameSp_;

	std::unique_ptr<ParticleManager> particleManager_;
	
	GameObjManager* gameObjectManager_ = nullptr;

	//FBXモデル
	std::unique_ptr<FBXModel> hitokunFbxM_;
	std::unique_ptr<FBXObject3d>hitokunFbxO_;
	std::unique_ptr<FbxPlayer> fbxPlayer_;
	
	std::unique_ptr<Field> field_;

	// 外部用変数
	bool isChangeScene_ = false;

	//スプライト位置保存
	//UI
	const Vector2 toHpSpritePos_ = { -300.f,500.f };
	const Vector2 offsetHpSpritePos_ = { 30.f,500.f };
	const Vector2 toExpPos_ = { 1500.f,100.f };
	const Vector2 offsetExpPos_ = { 1000.f,200.f };
	
};

//	参照元
//	https://qiita.com/tsuchinokoman/items/390a22a20abe2a6daa6d <-やり方わからん