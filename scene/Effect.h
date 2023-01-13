#pragma once
#include "Input.h"
#include"Object3d.h"
#include <Mesh.h>
#include <list>
#include <cassert>
#include"Matrix4.h"

const int EFFECT_NUM = 40;

class Effect
{
public:
	void Initialize(Mesh* model, uint32_t textureHandle, Vector3 vec);
	void Update();
	void Draw();

	bool IsDead() const { return isDead_; }	//���S��




private:
	//���[���h�ϊ��f�[�^

	Object3d worldTransform_[EFFECT_NUM];

	//���f��
	Mesh* model_ = nullptr;

	//���̑�
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	Vector3 velocity_[EFFECT_NUM] = {};
	Vector3 rotVector_[EFFECT_NUM] = {};

	//�f�X�t���O
	bool isDead_ = false;
	int daedFlame_ = 0;

};

