#include "Goal.h"
#include "Matrix4.h"
#include <cassert>
#include "MathFunc.h"
#define PI 3.141592653589

void Goal::Initialize(Mesh* model)
{

	assert(model);

	model_ = model;

	worldTransform_.SetModel(model_);
	
	worldTransform_.Initialize();

	worldTransform_.scale = { 1,1,1 };

	worldTransform_.rotation = { 1,1,1 };

	worldTransform_.Update();

	for (int i = 0; i < 3; i++) {
		bulletHit_[i] = 0;
	}

}

void Goal::Update()
{
	modelVelocityAngle_++;
	if (modelVelocityAngle_ > 360) {
		modelVelocityAngle_ = 0;
	}

	
		
		Vector3 kVec = { 0,sinf(modelVelocityAngle_*PI /180.0f)+2.4f,0};
		kVec =  MathFunc::bVelocity(kVec, worldTransform_);
		worldTransform_.position = { kVec.x,kVec.y,-38 };
		worldTransform_.Update();

	

}

void Goal::Draw()
{
	
	worldTransform_.Draw();
	

}

Vector3 Goal::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;

	//ワールド行列の平行移動成分
	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}

void Goal::OnCollision()
{

}

void Goal::MaterDown(bool isMaterDown)
{
	if (isMaterDown == true ) {
		materDownTime_++;

		if (materDownTime_ >= 7) {
			materDownTime_ = 0;
			for (int i = 0; i < 3; i++) {
				if (bulletHit_[i] > 0) {
					bulletHit_[i]--;
				}
			}

		}

	}
}
