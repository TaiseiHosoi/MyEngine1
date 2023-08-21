#pragma once
#include"Object3d.h"
#include"MathFunc.h"
#include"CollisionAttribute.h"
#include"CollisionManager.h"
#include"SphereCollider.h"

class PlayerRapidBullet
{
public:
	void Initialize(Mesh* model, Vector3 setPos, Vector3 setRot);
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);

public:
	SphereCollider* GetSphereCollider() { return sphere; };
	bool ReturnIsDead() { return isDead_; }
	bool isDead_ = false;

private:
	std::unique_ptr<Object3d> object_;
	Mesh* model_ = nullptr;
	float bulletSpeed_ = 4.f;
	CollisionManager* collider = nullptr;
	SphereCollider* sphere = nullptr;
	Vector3 pos;
	

	std::unique_ptr<Object3d> testObject_;

};

