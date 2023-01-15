#pragma once
#include "Object3d.h"
#include "Mesh.h"

class Goal
{
public:
	/// <summary>
	/// 初期化,更新,描画
	/// </summary>
	void Initialize(Mesh* model);
	void Update();
	void Draw();

	Vector3 GetWorldPosition();	//ワールド座標取得
	
	void OnCollision();	//触れたとき
	void MaterDown(bool isMaterDown);
	
	int bulletHit_[3] = {};
private:
	Object3d worldTransform_;
	Mesh* model_ = nullptr;

	float modelVelocityAngle_ = 0; //digrees

	uint32_t textureHandle_ = 0;

	int materDownTime_ = 0;//メーターダウンのディレイカウント
	
};


