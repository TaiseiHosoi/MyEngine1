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
#include "SpriteCommon.h"
#include "Mesh.h"
#include "Object3d.h"
#include "ParticleManager.h"
#include "GameObjManager.h"

#include "FbxPlayer.h"
#include "Ground.h"


class Input;
class IScene;
class DirectXCommon;
class GameCamera;

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

	// シーンを変更する
	void ChangeScene(IScene*);

	// ゲームのシーン設定をリセットする(敵の発生情報など)
	void ResetParameters();

	// jsonManager情報をセット
	void SetJsonManager(GameObjManager* jsonmanager) { gameObjectManager_ = jsonmanager; };

public:
	
	std::unique_ptr <Audio> audio;
	std::unique_ptr <SpriteCommon> spriteCommon_;

	std::unique_ptr<ParticleManager> particleManager_;
	
	GameObjManager* gameObjectManager_ = nullptr;

	//FBXモデル
	std::unique_ptr<FBXModel> hitokunFbxM_;
	std::unique_ptr<FBXObject3d>hitokunFbxO_;
	std::unique_ptr<FbxPlayer> fbxPlayer_;
	
	std::unique_ptr<Field> field_;


};

//	参照元
//	https://qiita.com/tsuchinokoman/items/390a22a20abe2a6daa6d <-やり方わからん