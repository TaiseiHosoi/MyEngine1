/**
 * @file PlayerRapidBullet.h
 * @brief ���@�̑��˒e
 */
#pragma once
#include"Object3d.h"
#include"MathFunc.h"
#include"CollisionAttribute.h"
#include"CollisionManager.h"
#include"SphereCollider.h"

class PlayerRapidBullet
{
public:
	// ������
	void Initialize(Mesh* model, Vector3 setPos, Vector3 setRot);

	// �X�V
	void Update();

	// �`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

public:

	// ���R���C�_�[�Q�b�^
	SphereCollider* GetSphereCollider() { return sphere; };

	// ���S���t���O�Q�b�^
	bool ReturnIsDead() { return isDead_; };

	// ���S���t���O�Z�b�^
	void SetIsDead(bool isDead) { isDead_ = isDead; };

private:
	std::unique_ptr<Object3d> object_;
	Mesh* model_ = nullptr;
	float bulletSpeed_ = 8.f;
	CollisionManager* collider = nullptr;
	SphereCollider* sphere = nullptr;
	Vector3 pos;
	
	bool isDead_ = false;
	std::unique_ptr<Object3d> testObject_;

};

