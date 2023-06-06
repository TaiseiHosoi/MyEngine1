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
#include "SphereCollider.h"



class Input;
class PlayerAction;

typedef enum ACTION_NUM {
	move,	//0
	stop,	//1
	atk1,	//2
	guard,	//3
	counter,//4
};

class PlayerActionManager
{
private:
	std::shared_ptr<PlayerAction> _action;
public:
	//コンストラクタ
	PlayerActionManager();

	//デストラクタ
	~PlayerActionManager();

	

public:
	// 各行動のInitializeを呼び出す
	void PActionInitialize(FBXObject3d* gameObject);
	// 各行動のUpdateを呼び出す
	void ActionUpdate(Input* input);
	// 各行動のDrawを呼び出す
	void ActionDraw();
	// シーンを変更する
	void ChangeAction(PlayerAction* pAction);
	// コライダーの初期化
	void ColliderInitialize(std::vector<SphereCollider*>* sphere, const int sphereColNum);

	//常に行う処理
	void DamagePlayer();
	
	

	
public:
	//円コライダーptr取得
	std::vector<SphereCollider*>* GetSphere() { return sphere_; };
	int GetSphereColNum() { return SPHERE_COLISSION_NUM; }

	//現在のアクしょん取得
	int GetNowActNum() { return nowActNum; };
	void SetNowActNum(const int num) { nowActNum = num; };

private: // メンバ変数
	FBXObject3d* gameObject_;
	
	std::vector<SphereCollider*>* sphere_;
	std::vector<Vector3> spherePos = {};
	int nowActNum = 1;
	int SPHERE_COLISSION_NUM = 0;

};

