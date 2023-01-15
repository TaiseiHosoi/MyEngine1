#pragma once
#include "Object3d.h"
#include "Mesh.h"

class Goal
{
public:
	/// <summary>
	/// ������,�X�V,�`��
	/// </summary>
	void Initialize(Mesh* model);
	void Update();
	void Draw();

	Vector3 GetWorldPosition();	//���[���h���W�擾
	
	void OnCollision();	//�G�ꂽ�Ƃ�
	void MaterDown(bool isMaterDown);
	
	int bulletHit_[3] = {};
private:
	Object3d worldTransform_;
	Mesh* model_ = nullptr;

	float modelVelocityAngle_ = 0; //digrees

	uint32_t textureHandle_ = 0;

	int materDownTime_ = 0;//���[�^�[�_�E���̃f�B���C�J�E���g
	
};


