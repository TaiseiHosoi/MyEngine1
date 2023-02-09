#include "CollisionPrimitive.h"

using namespace DirectX;

bool CollisionPrimitive::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter)
{
	//座標系の原点から級の中心ざひょうへの距離
	float distV = sphere.center.dot(plane.normal);

	//平面ｍｐ原点距離を減算することで、平面と級の中心との距離が出る
	float dist = distV - plane.distance;

	//距離の絶対値が半径より大きければ当たっていない
	if (fabs(dist) > sphere.radius) return false;

	//疑似交点
	if (inter) {
		//平面上の最近接点を疑似交点とする
		*inter = -dist * plane.normal + sphere.center;
	}

	return true;
}
