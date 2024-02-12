#pragma once
#include"FBXObject3d.h"
#include"FBXModel.h"
#include"FBXLoader.h"

class BossRobot {
public:
	BossRobot();
	~BossRobot();

	//初期化更新描画
	void Initialize(FBXModel* fbxModel);
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);

private:
	std::unique_ptr<FBXObject3d> gameObject_;
	FBXModel* fbxRoboModel_ = nullptr;



};