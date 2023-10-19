/**
 * @file RaycastHit.h
 * @brief レイキャストの衝突した際に使う情報構造体
 */
#pragma once
#include "BaseCollider.h"

class Object3d;

//レイキャストによる情報を得るための構造体
struct RaycastHit
{
	//衝突相手のオブジェクト
	Object3d* object_ = nullptr;

	//衝突相手のコライダー
	BaseCollider* collider_ = nullptr;

	//衝突点
	Vector3 inter_;

	//衝突点までの距離
	float distance_ = 0.0f;

};