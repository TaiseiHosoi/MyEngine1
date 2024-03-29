﻿#include "PlayerHomingBullet.h"
#include <random>

PlayerHomingBullet::PlayerHomingBullet()
{
}

PlayerHomingBullet::~PlayerHomingBullet()
{
	
}

void PlayerHomingBullet::Initialize(Mesh* model,Vector3 setPos,Vector3 setRot)
{
	
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 rndEngine(seed_gen());
	//乱数範囲の設定
	std::uniform_real_distribution<float> Dist(-1.0, 1.0);

	balletVel_ = { Dist(rndEngine),Dist(rndEngine) ,Dist(rndEngine) };
	balletVel_.nomalize();

	//ObjectInit
	model_ = model;
	object_ = Object3d::Create();
	object_->Initialize(true);
	object_->SetModel(model_);
	object_->SetPosition(setPos);
	object_->SetRotate(setRot);
	object_->SetScale({ 1,1,1 });
	object_->Update();

	
	angleVel_ = MathFunc::bVelocity(strVec, object_->worldTransform.matWorld_);

	
}

void PlayerHomingBullet::Update()
{
	if (homingTargetPtr_ != nullptr) {
		//敵へのベクトル計算
		Vector3 toTarget = homingTargetPtr_->translation_ - object_->worldTransform.translation_;
		toTarget.nomalize();

		//球面線形補間により、今の速度とターゲットへのベクトルを内挿し、新たな速度とする
		balletVel_ = MathFunc::slarp(balletVel_, toTarget, 0.1f) * 0.1f;

		//角度を合わせる
		object_->SetPosition(object_->GetPosition() + balletVel_ * balletSpeed_);
	}
	object_->Update();
	

}

void PlayerHomingBullet::Draw(ID3D12GraphicsCommandList* cmdList)
{
	object_->Draw(cmdList);
}
