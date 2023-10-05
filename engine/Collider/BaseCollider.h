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
	//仮想デストラクタ
	virtual ~BaseCollider() = default;

	inline void SetObject3d(Object3d* object)
	{
		this->object3d = object;
	}

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

	inline void OnCllision(const CollisionInfo& infoArg)
	{
		isHit = true;
		this->info = infoArg;
	}

	inline void SetAttribute(unsigned short attributeArg)
	{
		this->attribute = attributeArg;
	}

	unsigned short GetAttribute() {
		return this->attribute;
	}

	inline void AddAttribute(unsigned short attributeArg)
	{
		this->attribute |= attributeArg;
	}

	inline void RemoveAttribute(unsigned short attributeArg)
	{
		this->attribute &= !attributeArg;
	}

	inline bool GetIsHit()
	{
		return isHit;
	}

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