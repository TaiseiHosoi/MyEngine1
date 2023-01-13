#include "Goal.h"
#include "Matrix4.h"
#include <cassert>
#include "MathFunc.h"
#define PI 3.141592653589

void Goal::Initialize(Mesh* model, uint32_t textureHandle)
{
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	for (int i = 0; i < 4; i++) {
		worldTransform_[i].Create();
		worldTransform_[i].position = {0,0,0};
		worldTransform_[i].scale = {0.5,0.2,0.2};
	}
	worldTransform_[0].rotation = { 0,0,0 };
	worldTransform_[1].rotation = { 0,0,PI* 3/2 };
	worldTransform_[2].rotation = { 0,0,PI };
	worldTransform_[3].rotation = { 0,0,PI / 2 };
	
	
	for (int i = 0; i < 4; i++) {
		worldTransform_[i].Update();
	}

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

	for (int i = 0; i < 4; i++) {
		
		Vector3 kVec = { 0,sinf(modelVelocityAngle_*PI /180.0f)+2.4f,0};
		kVec =  MathFunc::bVelocity(kVec, worldTransform_[i]);
		worldTransform_[i].position = { kVec.x,kVec.y,-38 };
		worldTransform_[i].Update();

	}

}

void Goal::Draw()
{
	for (int i = 0; i < 4; i++) {
		//model_->Draw(worldTransform_[i], viewProjection,textureHandle_);
	}

}

Vector3 Goal::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;

	//ワールド行列の平行移動成分
	worldPos.x = worldTransform_[0].matWorld.m[3][0];
	worldPos.y = worldTransform_[0].matWorld.m[3][1];
	worldPos.z = worldTransform_[0].matWorld.m[3][2];

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
