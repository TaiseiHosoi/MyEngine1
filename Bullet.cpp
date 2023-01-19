#include "Bullet.h"
#include <cmath>
#include "math.h"
#define PI 3.141592653589

void Bullet::Initialize(Mesh* model, Vector3 vector3, float kBulSpeed)
{
	// NULLポインタチェック
	assert(model);
	model_ = model;
	worldTransform_.SetModel(model);
	//textureHandle_ = textureHandle;

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();

	//初期座標をセット
	worldTransform_.Initialize();
	worldTransform_.position = vector3;
	worldTransform_.scale = { 1,1,1 };
	worldTransform_.rotation = { 0,0,0 };

	//現在のXによってレーンを変更
	if (worldTransform_.position.x < 0)
	{
		lane_ = Left;
	}
	else if (worldTransform_.position.x == 0)
	{
		lane_ = Center;
	}
	else if (worldTransform_.position.x > 0)
	{
		lane_ = Right;
	}

	//デスフラグ
	bool isDead_ = false;




}

void Bullet::Update()
{

	

	//ノーツの速度の処理
	kBulletSpeedZ += kBulletSpeedAcc;
	Vector3 kBulletSpeed = { 0.0f,0.0f,-easeIn(kBulletSpeedZ)*2 };
	worldTransform_.position += kBulletSpeed;
	worldTransform_.Update();

}

void Bullet::Draw()
{
	//model_->Draw(worldTransform_, viewProjection, textureHandle_);
	worldTransform_.Draw();
}

float Bullet::easeIn(float x)
{
	return 1 - cos((x * PI) / 2);
}

Vector3 Bullet::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分
	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}

void Bullet::OnCollision()
{
	isDead_ = true;
}
