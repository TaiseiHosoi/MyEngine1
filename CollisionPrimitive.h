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

class Triangle {
public:
	Vector3 p0;
	Vector3 p1;
	Vector3 p2;

	Vector3 normal;

};

struct Ray
{
	Vector3 start = { 0,0,0 };

	Vector3 dir = { 1,0,0 };

};


namespace CollisionPrimitive
{

	//�e�ƕ��ʂ̓����蔻��
	bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);

	//�O�p�`�Ɠ_�̍ŋߐړ_
	void ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest);

	//�O�p�`�Ƌ�
	bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter = nullptr, Vector3* reject = nullptr);

	//���Ƌ�
	bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, Vector3* inter = nullptr, Vector3* reject = nullptr);

};

