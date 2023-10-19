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
		this->object3d_ = object;
	}

	// オブジェクトゲット
	inline Object3d* GetObject3d()
	{
		return object3d_;
	}

	//更新
	virtual void Update() = 0;
	//形状タイプ取得
	inline CollisionShapeType GetShapeType()
	{
		return shapeType_;
	}

	// ヒット時情報セット
	inline void OnCllision(const CollisionInfo& infoArg)
	{
		isHit_ = true;
		this->info = infoArg;
	}

	// 属性セット
	inline void SetAttribute(unsigned short attributeArg)
	{
		this->attribute_ = attributeArg;
	}

	// 属性ゲット
	unsigned short GetAttribute() {
		return this->attribute_;
	}

	// 属性追加
	inline void AddAttribute(unsigned short attributeArg)
	{
		this->attribute_ |= attributeArg;
	}

	// 属性削除
	inline void RemoveAttribute(unsigned short attributeArg)
	{
		this->attribute_ &= !attributeArg;
	}

	// isHitゲッタ
	inline bool GetIsHit()
	{
		return isHit_;
	}

	// 衝突相手の情報ゲッタ
	inline CollisionInfo GetCollisionInfo()
	{
		return info;
	}

protected:
	Object3d* object3d_ = nullptr;
	
	bool isHit_ = false;

	CollisionInfo info = {
		nullptr ,
		nullptr ,
		{0 , 0 , 0}
	};
	
	//形状タイプ
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;
	//当たり判定属性
	unsigned short attribute_ = 0b1111111111111111;
};