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
	//�R���X�g���N�^
	PlayerActionManager();

	//�f�X�g���N�^
	~PlayerActionManager();

	

public:
	// �e�s����Initialize���Ăяo��
	void PActionInitialize(FBXObject3d* gameObject);
	// �e�s����Update���Ăяo��
	void ActionUpdate(Input* input);
	// �e�s����Draw���Ăяo��
	void ActionDraw();
	// �V�[����ύX����
	void ChangeAction(PlayerAction* pAction);
	// �R���C�_�[�̏�����
	void ColliderInitialize(std::vector<SphereCollider*>* sphere, const int sphereColNum);

	//��ɍs������
	void DamagePlayer();
	
	

	
public:
	//�~�R���C�_�[ptr�擾
	std::vector<SphereCollider*>* GetSphere() { return sphere_; };
	int GetSphereColNum() { return SPHERE_COLISSION_NUM; }

	//���݂̃A�N�����擾
	int GetNowActNum() { return nowActNum; };
	void SetNowActNum(const int num) { nowActNum = num; };

private: // �����o�ϐ�
	FBXObject3d* gameObject_;
	
	std::vector<SphereCollider*>* sphere_;
	std::vector<Vector3> spherePos = {};
	int nowActNum = 1;
	int SPHERE_COLISSION_NUM = 0;

};

