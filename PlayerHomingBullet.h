#pragma once
#include"Object3d.h"
#include"MathFunc.h"

class PlayerHomingBullet
{
public:
	PlayerHomingBullet();
	~PlayerHomingBullet();
	void Initialize(Mesh* model,Vector3 setPos,Vector3 setRot,WorldTransform* homingTargetPtr);
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);

private:
	std::unique_ptr<Object3d> object_;
	Mesh* model_ = nullptr;
	Vector3 angleVel_ = {};	//ワールド行列とvelをかけた値
	WorldTransform* homingTargetPtr_ = nullptr;
	Vector3 balletVel_ = {};	//弾のvel
	float balletSpeed_ = 0.1f;

};

