#pragma once
#include"DirectXMath.h"
#include"Vector3.h"
#include"Vector4.h"

struct Sphere {
	//中心座標
	Vector3 center = { 0,0,0 };
	//半径
	float radius = 1.0f;
};

struct Plane {
	//法線ベクトル
	Vector3 normal = { 0,1,0 };
	//原点(0,0,0)からの距離
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

	//弾と平面の当たり判定
	bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);

	//三角形と点の最近接点
	void ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest);

	//三角形と球
	bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter = nullptr, Vector3* reject = nullptr);

	//球と球
	bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, Vector3* inter = nullptr, Vector3* reject = nullptr);

};

