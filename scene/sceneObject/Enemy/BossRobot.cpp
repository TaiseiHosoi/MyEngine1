#include "BossRobot.h"

void BossRobot::Initialize(FBXModel* fbxModel)
{
	fbxRoboModel_ = fbxModel;
	gameObject_ = FBXObject3d::Create();
	gameObject_->SetModel(fbxRoboModel_);
}

void BossRobot::Update()
{

	gameObject_->Update();
}

void BossRobot::Draw(ID3D12GraphicsCommandList* cmdList)
{
	gameObject_->Draw(cmdList);
}
