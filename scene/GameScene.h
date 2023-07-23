#pragma once
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Audio.h"
#include "Mesh.h"
#include "sstream"
#include "memory"
#include "list"
#include "GameCamera.h"
#include"CollisionPrimitive.h"
#include "CollisionManager.h"
#include"GameCamera.h"
#include"FBXObject3d.h"
#include"fbx/FBXLoader.h"
#include"FBXModel.h"
#include"ParticleManager.h"
#include "SceneManager.h"
#include "HitStopManager.h"
#include "JsonManager.h"


#include "FbxPlayer.h"

#include"Ground.h"



class GameScene {
public:
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxcomon);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


	int gameLevel_ = 0;
	int levelMax_ = 8;
	int scene = 1;

private:
	DirectXCommon* dxCommon_ = nullptr;

	std::unique_ptr <GameCamera> gameCamera_;
	Input* input_ = Input::GetInstance();
	std::unique_ptr <SceneManager> sceneManager_;
	HitStopManager* hitStopManager_;
	CollisionManager* collisionManager_ = nullptr;
	//敵マネージャ
	std::unique_ptr<JsonManager> JsonManager_;

};