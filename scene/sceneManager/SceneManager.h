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
#include"JsonManager.h"

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
	void ResetParameters();

	void SetJsonManager(GameObjManager* jsonmanager) { jsonManager_ = jsonmanager; };
	//ID3D12GraphicsCommandList* GetCommandList() { return _dxCommon->GetCommandList(); };
public:
	
	std::unique_ptr <Audio> audio;
	std::unique_ptr <SpriteCommon> spriteCommon_;

	std::unique_ptr<ParticleManager> particleManager_;
	
	GameObjManager* jsonManager_ = nullptr;
	//std::unique_ptr <Object3d> object3d;
	//std::unique_ptr <Mesh> model;

	//FBXモデル
	std::unique_ptr<FBXModel> hitokunFbxM_;
	std::unique_ptr<FBXObject3d>hitokunFbxO_;
	//FBXモデル
	//std::unique_ptr<FBXModel> bossFbxM_;
	//std::unique_ptr<FBXObject3d> bossFbxO_;

	std::unique_ptr<FbxPlayer> fbxPlayer_;
	

	std::unique_ptr<Field> field_;


};

//	参照元
//	https://qiita.com/tsuchinokoman/items/390a22a20abe2a6daa6d <-やり方わからん