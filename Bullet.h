#pragma once

#include "Input.h"
#include <Mesh.h>
#include <Object3d.h>
#include <list>
#include <cassert>
#include"Matrix4.h"
#include"Field.h"

class GameScene;

class Bullet
{
public:
	void Initialize(Mesh* model, Vector3 vector3, float kBulSpeed);
	void Update();
	void Draw();

	bool IsDead() const { return isDead_; }	//死亡時
	int GetId() { return bulletId_; }
	void SetID(int ID) { bulletId_ = ID; }
	void SetFieldLane(int lane) { lane_ = lane; }
	int GetFieldLane() { return lane_; };


	//弾のイージング
	float easeIn(float x);

	////ワールド座標を取得
	Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

private:
	//ワールド変換データ
	Object3d worldTransform_;

	//モデル
	Mesh* model_ = nullptr;

	//その他
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	

	//固有ID
	int bulletId_ = 0;

	//デスフラグ
	bool isDead_ = false;

	float depth = 40.0f;	//奥行
	float xDifference = 10.0f;	//左右差

	//ノーツの速度
	float kBulletSpeedZ = 0.4;
	//ノーツの加速度
	float kBulletSpeedAcc = 0.003;

	//現在のレーン
	int lane_;

};

