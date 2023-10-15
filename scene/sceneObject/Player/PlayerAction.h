/**
 * @file PlayerAction.h
 * @brief 自機行動の基底クラス
 */
#pragma once
#include "Object3d.h"
#include "FBXObject3d.h"
#include "FBXModel.h"
#include "input.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"ParticleManager.h"
#include"PlayerActionManager.h"

class PlayerAction
{
protected:

	PlayerActionManager* PActManager;
public:


	PlayerAction();
	
	virtual ~PlayerAction();

	/// シーンの更新を行う
	virtual void Initialize(FBXObject3d* gameObject) = 0;

	/// シーンの更新を行う
	virtual void Update(Input* input) = 0;

	/// シーンの描画を行う
	virtual void Draw() = 0;




};

