#pragma once

#include "Input.h"
#include <Mesh.h>
#include <Object3d.h>
#include <list>
#include <cassert>
#include"Matrix4.h"
#include"Field.h"

class GameScene;

class Bullet
{
public:
	void Initialize(Mesh* model, Vector3 vector3, float kBulSpeed);
	void Update();
	void Draw();

	bool IsDead() const { return isDead_; }	//���S��
	int GetId() { return bulletId_; }
	void SetID(int ID) { bulletId_ = ID; }
	void SetFieldLane(int lane) { lane_ = lane; }
	int GetFieldLane() { return lane_; };


	//�e�̃C�[�W���O
	float easeIn(float x);

	////���[���h���W���擾
	Vector3 GetWorldPosition();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

private:
	//���[���h�ϊ��f�[�^
	Object3d worldTransform_;

	//���f��
	Mesh* model_ = nullptr;

	//���̑�
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	

	//�ŗLID
	int bulletId_ = 0;

	//�f�X�t���O
	bool isDead_ = false;

	float depth = 40.0f;	//���s
	float xDifference = 10.0f;	//���E��

	//�m�[�c�̑��x
	float kBulletSpeedZ = 0.4;
	//�m�[�c�̉����x
	float kBulletSpeedAcc = 0.003;

	//���݂̃��[��
	int lane_;

};

