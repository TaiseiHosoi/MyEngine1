/**
 * @file PlayerRapidBullet.h
 * @brief 自機の速射弾
 */
#pragma once
#include"Object3d.h"
#include"MathFunc.h"
#include"CollisionAttribute.h"
#include"CollisionManager.h"
#include"SphereCollider.h"

class PlayerRapidBullet
{
public:
	~PlayerRapidBullet();
	// 初期化
	void Initialize(Mesh* model, Vector3 setPos, Vector3 setRot);

	// 更新
	void Update();

	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

public:

	// 球コライダーゲッタ
	SphereCollider* GetSphereCollider() { return sphere.get(); };

	// 死亡時フラグゲッタ
	bool ReturnIsDead() { return isDead_; };

	// 死亡時フラグセッタ
	void SetIsDead(bool isDead) { isDead_ = isDead; };

private:

	float bulletSpeed_ = 8.f;
	const int maxCountSinceBirth_ = 60;
	int countSinceBirth_ = 0;
	CollisionManager* collider = nullptr;
	std::unique_ptr<SphereCollider> sphere;
	std::unique_ptr<Object3d> object_;
	Mesh* model_ = nullptr;
	bool isDead_ = false;
	Vector3 pos;
	
	
	std::unique_ptr<Object3d> testObject_;

};

