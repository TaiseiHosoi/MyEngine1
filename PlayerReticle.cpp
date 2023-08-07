#include "PlayerReticle.h"

void PlayerReticle::Initialize(WorldTransform pairWTF)
{
	nierReticleM_ = Mesh::LoadFormOBJ("3DReticle", false);
	nierReticleO_ = Object3d::Create();
	nierReticleO_->SetModel(nierReticleM_.get());
	nierReticleO_->worldTransform.parent_ = &pairWTF;
	nierReticleO_->worldTransform.translation_.z = offsetNier;
	nierReticleO_->worldTransform.rotation_.y = 0.5f * 3.14f;

	farReticleM_ = Mesh::LoadFormOBJ("3DReticle", false);
	farReticleO_ = Object3d::Create();
	farReticleO_->SetModel(nierReticleM_.get());
	farReticleO_->worldTransform.parent_ = &pairWTF;
	farReticleO_->worldTransform.translation_.z = offsetFar;
	farReticleO_->worldTransform.rotation_.y = 0.5f * 3.14f;


	nierReticleO_->Update();
	farReticleO_->Update();
	
}

void PlayerReticle::Update()
{
	Vector3 vel = farReticleO_.get()->worldTransform.matWorld_.GetWorldPos() - nierReticleO_.get()->worldTransform.matWorld_.GetWorldPos();
	vel.nomalize();

	vel *= rockLength_;
	

	

	nierReticleO_->Update();
	farReticleO_->Update();

}

void PlayerReticle::Draw(ID3D12GraphicsCommandList* cmdList)
{
	nierReticleO_->Draw(cmdList);
	farReticleO_->Draw(cmdList);
}
