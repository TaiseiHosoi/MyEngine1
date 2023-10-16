/**
 * @file SphereCollider.h
 * @brief 球のコライダー
 */
#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include "MathFunc.h"

class SphereCollider : public BaseCollider , public Sphere
{
public:
	SphereCollider(Vector3 offset = {0 , 0 , 0} , float radius = 1.0f) :
		offset(offset) ,
		radius(radius)
	{
		//球形状をセット
		shapeType = COLLISIONSHAPE_SPHERE;
	}

	// 更新
	void Update() override;

	// 半径セット
	inline void SetRadius(float radiusArg)
	{
		this->radius = radiusArg; 
	}
	
	// 半径ゲット
	float GetRadius()
	{
		return radius; 
	}
	
	// 中心位置セット
	inline void SetBasisPos(Vector3* pos)
	{
		basisPos = pos;
	}

	// 中心位置ゲット
	Vector3 GetBasisPos()
	{
		return *basisPos;
	}

private:
	//オブジェクト中心からのオフセット
	Vector3 offset;

	//モデルがない場合の基準となるベクトル
	Vector3* basisPos = nullptr;

	//半径
	float radius;

};

