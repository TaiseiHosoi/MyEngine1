/**
 * @file PlayerReticle.h
 * @brief 自機のレティクル
 */
#pragma once
#include "Object3d.h"
#include "CollisionManager.h"
class PlayerReticle
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WorldTransform* pairWTF);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

public://アクセッサ
	/// <summary>
	/// ロックオンフラグ
	/// </summary>
	/// <returns><isRockOn_>
	Object3d* GetFarReticleObjPtr() { return farReticleO_.get(); }
	

private:
	/// <summary>
	/// オブジェクト
	/// </summary>

	//近距離レティクル
	std::unique_ptr<Object3d> nierReticleO_;
	std::unique_ptr<Mesh> nierReticleM_;

	//遠距離レティクル
	std::unique_ptr<Object3d> farReticleO_;
	std::unique_ptr<Mesh> farReticleM_;


	
	float offsetNier = 10.f;
	float offsetFar = 30.f;

	//CollisionManager* collider_ = nullptr;
	//std::unique_ptr<RaycastHit> raycastHit;
	//bool isRockOn_ = false;

};

