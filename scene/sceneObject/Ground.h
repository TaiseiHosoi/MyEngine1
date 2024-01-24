/**
 * @file Ground.h
 * @brief フィールドや背景オブジェクト
 */
#pragma once
#include "DirectXCommon.h"
#include "Vector3.h"
#include "input.h"
#include "Object3d.h"
#include"Mesh.h"
#include "Sprite.h"
#include "SpriteCommon.h"

class Field
{
	//コンストラクタ
public:
	Field();
	~Field();

public:
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw(DirectXCommon* dxcomon_);



	//メンバ変数
private:



	std::unique_ptr<Mesh> fieldM;
	const float groundSpeed_ = 0.2f;

	const float groundAdjustScale_ = 4000.f;
	const float backGroundAdjustPosY_ = 400.f;
	const float groundAdjustPosY_ = -10;

	std::unique_ptr<Object3d> groundObj1_;	//地面1
	std::unique_ptr<Object3d> continuousFloor_;	//地面2
	
	

	std::unique_ptr<Object3d> skydomeObj_;
	std::unique_ptr<Mesh> skydomeModel_;
	std::unique_ptr<Mesh> continuousFloorModel_;

	Mesh* fenceM = nullptr;

	static const int fieldR_ = 415;
	static const int R = fieldR_;
	static const int BR = fieldR_ - 7;
};
