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

	// 自機が接地しているか
	static bool PlayerOnGround(Vector3 wolPos, float Obj_R);

	// 敵が接地しているか
	static bool EnemyOnGround(Vector3 wolPos, float Obj_R);


	//メンバ変数
private:

	std::unique_ptr<Mesh> fieldM;
	const float groundSpeed_ = 0.2f;
	const float groundAdjustScale_ = 100.f;
	const float groundAdjustPosY_ = -5000.f;
	std::unique_ptr<Object3d> groundObj1_;	//地面1
	std::unique_ptr<Object3d> groundObj2_;	//地面2
	

	std::unique_ptr<Object3d> skydomeObj_;
	std::unique_ptr<Mesh> skydomeModel_;

	Mesh* fenceM = nullptr;

	static const int fieldR_ = 415;
	static const int R = fieldR_;
	static const int BR = fieldR_ - 7;
};
