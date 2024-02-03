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

	// 遠くのレティクルゲッタ
	Object3d* GetFarReticleObjPtr() { return farReticleO_.get(); }
	

private:
	/// <summary>
	/// オブジェクト
	/// </summary>

	//近距離レティクル
	std::unique_ptr<Object3d> nierReticleO_;
	std::unique_ptr<Mesh> nierReticleM_;

	//中距離レティクル
	std::unique_ptr<Object3d> mediumRangeReticleO_;
	std::unique_ptr<Mesh> mediumRangeReticleM_;

	//遠距離レティクル
	std::unique_ptr<Object3d> farReticleO_;
	std::unique_ptr<Mesh> farReticleM_;

	const float offsetRotY_ = 0.5f * 3.14f;
	
	const float offsetNierPosLength_ = 10.f;
	const float offsetMediumRangePos_ = 50.f;
	const float offsetFarPosLength_ = 90.f;
	const Vector3 nierReticleScale_ = { 0.1f,1.f,1.f };
	const Vector3 mediumRangeReticleScale_ = { 0.1f,1.f,1.f };
	const Vector3 farReticleScale_ = { 0.1f,1.f,1.f };

	



};

