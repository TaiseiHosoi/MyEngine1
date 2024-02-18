//自機の機銃
#pragma once
#include"Mesh.h"
#include"Object3d.h"

class PlayerMachinGun {
public:
	// コンストラクタデストラクタ
	PlayerMachinGun();
	~PlayerMachinGun();

	// 関数
	// 初期化
	void Initialize(Mesh* model,WorldTransform* playerWorldTF);

	// 更新
	void Update();

	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

public:
	//アクセサ

	//マズルオブジェクトゲッタ(right)
	Object3d* GetMuzzleRightPtr();
	
	//マズルオブジェクトゲッタ(left)
	Object3d* GetMuzzleLeftPtr();

private:
	//メンバ変数

	std::unique_ptr<Object3d> gameObjectRight_;
	std::unique_ptr<Object3d> gameObjectLeft_;
	Mesh* model_ = nullptr;
	WorldTransform* playerWorldTF_ = nullptr;
	Vector3* targetPos_ = nullptr;
	const float kAdjustPosY_ = 2.f;
	const float kAdjustPosX_ = 2.f;
	

};
