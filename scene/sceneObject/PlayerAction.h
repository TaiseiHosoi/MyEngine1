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

	/// �V�[���̍X�V���s��
	virtual void Initialize(FBXObject3d* gameObject) = 0;

	/// �V�[���̍X�V���s��
	virtual void Update(Input* input) = 0;

	/// �V�[���̕`����s��
	virtual void Draw() = 0;




};

