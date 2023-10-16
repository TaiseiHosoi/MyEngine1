/**
 * @file PlayerHomingBullet.h
 * @brief 自機のhoming弾
 */
#pragma once
#include"Object3d.h"
#include"MathFunc.h"

class PlayerHomingBullet
{
public:
	PlayerHomingBullet();
	~PlayerHomingBullet();
	// 初期化
	void Initialize(Mesh* model,Vector3 setPos,Vector3 setRot);

	//更新
	void Update();
	
	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

public://アクセッサ
	// homing弾用のターゲットセッタ
	void SetTargerPtr(WorldTransform* target) { homingTargetPtr_ = target; };

private:
	std::unique_ptr<Object3d> object_;
	Mesh* model_ = nullptr;
	Vector3 angleVel_ = {};	//ワールド行列とvelをかけた値
	WorldTransform* homingTargetPtr_ = nullptr;
	Vector3 balletVel_ = {};	//弾のvel
	float balletSpeed_ = 0.4f;

};

