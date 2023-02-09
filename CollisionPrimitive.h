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


class CollisionPrimitive
{
public:
	//弾と平面の当たり判定
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);

};

