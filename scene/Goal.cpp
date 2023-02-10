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

	worldTransform_.Initialize(true);

	worldTransform_.position = { 0,2,5 };
	worldTransform_.scale = { 1,1,1 };
	worldTransform_.rotation = { 0,0,0 };

	worldTransform_.Update();

	for (int i = 0; i < 3; i++) {
		bulletHit_[i] = 0;
	}

	hp = 10;

}

void Goal::Update(Vector3 pos)
{
	worldTransform_.position = {pos.x,pos.y,-25.0f};

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
	hp--;
	if (hp <= 0) {
		isDead_ = true;
	}
}

void Goal::MaterDown(bool isMaterDown)
{
	if (isMaterDown == true) {
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
