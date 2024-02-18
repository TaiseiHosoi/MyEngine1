#include "PlayerMachineGun.h"

PlayerMachinGun::PlayerMachinGun()
{
}

PlayerMachinGun::~PlayerMachinGun()
{
}

void PlayerMachinGun::Initialize(Mesh* model, WorldTransform* playerWorldTF)
{
	model_ = model;
	playerWorldTF_ = playerWorldTF;


	//右砲
	gameObjectRight_ = Object3d::Create();
	gameObjectRight_->Initialize(true);
	gameObjectRight_->SetModel(model_);
	gameObjectRight_->SetScale({0.5,0.5,0.5});
	gameObjectRight_->worldTransform.parent_ = playerWorldTF;
	gameObjectRight_->worldTransform.translation_.y += kAdjustPosY_;
	gameObjectRight_->worldTransform.translation_.x += kAdjustPosX_;

	//左砲
	gameObjectLeft_ = Object3d::Create();
	gameObjectLeft_->Initialize(true);
	gameObjectLeft_->SetModel(model_);
	gameObjectLeft_->SetScale({ 0.5,0.5,0.5 });
	gameObjectLeft_->worldTransform.parent_ = playerWorldTF;
	gameObjectLeft_->worldTransform.translation_.y += kAdjustPosY_;
	gameObjectLeft_->worldTransform.translation_.x -= kAdjustPosX_;

}

void PlayerMachinGun::Update()
{
	//右砲
	
	gameObjectRight_->Update();
	

	//左砲
	
	gameObjectLeft_->Update();
}

void PlayerMachinGun::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//描画
	gameObjectRight_->Draw(cmdList);
	gameObjectLeft_->Draw(cmdList);

}

Object3d* PlayerMachinGun::GetMuzzleRightPtr()
{
	return gameObjectRight_.get();
}

Object3d* PlayerMachinGun::GetMuzzleLeftPtr()
{
	return gameObjectLeft_.get();
}

