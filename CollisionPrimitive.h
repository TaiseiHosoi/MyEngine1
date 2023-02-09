#pragma once
#include"DirectXMath.h"
#include"Vector3.h"
#include"Vector4.h"

struct Sphere {
	//���S���W
	Vector3 center = { 0,0,0 };
	//���a
	float radius = 1.0f;
};

struct Plane {
	//�@���x�N�g��
	Vector3 normal = { 0,1,0 };
	//���_(0,0,0)����̋���
	float distance = 0.0f;
};


class CollisionPrimitive
{
public:
	//�e�ƕ��ʂ̓����蔻��
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);

};

