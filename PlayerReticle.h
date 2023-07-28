#pragma once
#include "Object3d.h"
class PlayerReticle
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WorldTransform pairWTF);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

public:
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


};

