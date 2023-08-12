﻿#include "PlayerReticle.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

void PlayerReticle::Initialize(WorldTransform* pairWTF)
{
	nierReticleM_ = Mesh::LoadFormOBJ("3DReticle", false);
	nierReticleO_ = Object3d::Create();
	nierReticleO_->SetModel(nierReticleM_.get());
	nierReticleO_->worldTransform.parent_ = pairWTF;
	nierReticleO_->worldTransform.translation_.z = offsetNier;
	nierReticleO_->worldTransform.rotation_.y = 0.5f * 3.14f;

	farReticleM_ = Mesh::LoadFormOBJ("3DReticle", false);
	farReticleO_ = Object3d::Create();
	farReticleO_->SetModel(nierReticleM_.get());
	farReticleO_->worldTransform.parent_ = pairWTF;
	farReticleO_->worldTransform.translation_.z = offsetFar;
	farReticleO_->worldTransform.rotation_.y = 0.5f * 3.14f;


	nierReticleO_->Update();
	farReticleO_->Update();

	collider_ = CollisionManager::GetInstance();
	isRockOn_ = false;
	
}

void PlayerReticle::Update()
{
	Vector3 forwardNorm = farReticleO_->worldTransform.matWorld_.GetWorldPos() - nierReticleO_->worldTransform.matWorld_.GetWorldPos();
	forwardNorm.nomalize();

	Ray ray;
	ray.start = nierReticleO_->worldTransform.parent_->translation_;
	ray.dir = forwardNorm;
	

	if (collider_->Raycast(ray, &raycastHit, 60.f) && isRockOn_ == false) {
		if (raycastHit.collider->GetAttribute() == COLLISION_ATTR_ENEMIES) {
			isRockOn_ = true;
		}
	}
	else {
		isRockOn_ = false;
	}
	
	
	
	nierReticleO_->Update();
	farReticleO_->Update();

}

void PlayerReticle::Draw(ID3D12GraphicsCommandList* cmdList)
{
	nierReticleO_->Draw(cmdList);
	farReticleO_->Draw(cmdList);
}
