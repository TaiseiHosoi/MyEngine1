/**
 * @file BaseCollider.h
 * @brief コライダー基底クラス
 */
#pragma once

#include "CollisionTypes.h"
#include "Object3d.h"
#include "CollisionInfo.h"


//コライダー基底クラス
class BaseCollider
{
public:
	friend class CollisionManager;

public:
	BaseCollider() = default;
	// 仮想デストラクタ
	virtual ~BaseCollider() = default;

	// オブジェクトセット
	inline void SetObject3d(Object3d* object)
	{
		this->object3d = object;
	}

	// オブジェクトゲット
	inline Object3d* GetObject3d()
	{
		return object3d;
	}

	//更新
	virtual void Update() = 0;
	//形状タイプ取得
	inline CollisionShapeType GetShapeType()
	{
		return shapeType;
	}

	// ヒット時情報セット
	inline void OnCllision(const CollisionInfo& infoArg)
	{
		isHit = true;
		this->info = infoArg;
	}

	// 属性セット
	inline void SetAttribute(unsigned short attributeArg)
	{
		this->attribute = attributeArg;
	}

	// 属性ゲット
	unsigned short GetAttribute() {
		return this->attribute;
	}

	// 属性追加
	inline void AddAttribute(unsigned short attributeArg)
	{
		this->attribute |= attributeArg;
	}

	// 属性削除
	inline void RemoveAttribute(unsigned short attributeArg)
	{
		this->attribute &= !attributeArg;
	}

	// isHitゲッタ
	inline bool GetIsHit()
	{
		return isHit;
	}

	// 衝突相手の情報ゲッタ
	inline CollisionInfo GetCollisionInfo()
	{
		return info;
	}

protected:
	Object3d* object3d = nullptr;
	
	bool isHit = false;

	CollisionInfo info = {
		nullptr ,
		nullptr ,
		{0 , 0 , 0}
	};
	
	//形状タイプ
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
	//当たり判定属性
	unsigned short attribute = 0b1111111111111111;
};