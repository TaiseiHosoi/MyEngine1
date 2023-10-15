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
	// 初期化
	void Initialize(Mesh* model, Vector3 setPos, Vector3 setRot);

	// 更新
	void Update();

	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

public:

	// 球コライダーゲッタ
	SphereCollider* GetSphereCollider() { return sphere; };

	// 死亡時フラグゲッタ
	bool ReturnIsDead() { return isDead_; };

	// 死亡時フラグセッタ
	void SetIsDead(bool isDead) { isDead_ = isDead; };

private:
	std::unique_ptr<Object3d> object_;
	Mesh* model_ = nullptr;
	float bulletSpeed_ = 8.f;
	CollisionManager* collider = nullptr;
	SphereCollider* sphere = nullptr;
	Vector3 pos;
	
	bool isDead_ = false;
	std::unique_ptr<Object3d> testObject_;

};

