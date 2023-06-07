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
	void Initialize();

	void Update();

	void Draw(DirectXCommon* dxcomon_);

	static bool PlayerOnGround(Vector3 wolPos, float Obj_R);
	static bool EnemyOnGround(Vector3 wolPos, float Obj_R);


	//メンバ変数
private:
	std::unique_ptr<Object3d> gameObject_;	//地面
	std::unique_ptr<Mesh> fieldM;

	std::unique_ptr<Object3d> skydomeObj_;
	std::unique_ptr<Mesh> skydomeModel_;
	const int fenceLen_ = 4;
	Object3d* fence_[4] = {};	//フェンス
	Mesh* fenceM = nullptr;

	static const int fieldR_ = 415;
	static const int R = fieldR_;

	static const int BR = fieldR_ - 7;
};
