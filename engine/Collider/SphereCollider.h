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
		offset_(offset) ,
		radius_(radius)
	{
		//球形状をセット
		shapeType_ = COLLISIONSHAPE_SPHERE;
	}

	// 更新
	void Update() override;

	// 半径セット
	inline void SetRadius(float radiusArg)
	{
		this->radius_ = radiusArg; 
	}
	
	// 半径ゲット
	float GetRadius()
	{
		return radius_; 
	}
	
	// 中心位置セット
	inline void SetBasisPos(Vector3* pos)
	{
		basisPos_ = pos;
	}

	// 中心位置ゲット
	Vector3 GetBasisPos()
	{
		return *basisPos_;
	}

private:
	//オブジェクト中心からのオフセット
	Vector3 offset_;

	//モデルがない場合の基準となるベクトル
	Vector3* basisPos_ = nullptr;

	//半径
	float radius_;

};

