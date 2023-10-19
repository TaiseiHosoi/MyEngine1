/**
 * @file CollisionInfo.h
 * @brief 衝突情報構造体
 */
#pragma once
#include "MathFunc.h"

class Object3d;
class BaseCollider;

//衝突情報
struct CollisionInfo
{
public:
	CollisionInfo(Object3d* object , BaseCollider* collider , const Vector3 inter)
	{
		this->object_ = object;
		this->collider_ = collider;
		this->inter_ = inter;
	}

	//衝突相手のオブジェクト
	Object3d* object_ = nullptr;
	//衝突相手のコライダー
	BaseCollider* collider_ = nullptr;
	//衝突店
	Vector3 inter_;

};