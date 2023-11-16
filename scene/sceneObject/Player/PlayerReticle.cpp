#include "PlayerReticle.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

void PlayerReticle::Initialize(WorldTransform* pairWTF)
{
	nierReticleM_ = Mesh::LoadFormOBJ("3DReticle", false);
	nierReticleO_ = Object3d::Create();
	nierReticleO_->SetModel(nierReticleM_.get());
	nierReticleO_->worldTransform.parent_ = pairWTF;
	nierReticleO_->worldTransform.translation_.z = offsetNierPos_;
	nierReticleO_->worldTransform.rotation_.y = offsetRotY_;
	nierReticleO_->worldTransform.scale_ = nierReticleScale_;

	mediumRangeReticleM_ = Mesh::LoadFormOBJ("3DReticle", false);
	mediumRangeReticleO_ = Object3d::Create();
	mediumRangeReticleO_->SetModel(mediumRangeReticleM_.get());
	mediumRangeReticleO_->worldTransform.parent_ = pairWTF;
	mediumRangeReticleO_->worldTransform.translation_.z = offsetMediumRangePos_;
	mediumRangeReticleO_->worldTransform.rotation_.y = offsetRotY_;
	mediumRangeReticleO_->worldTransform.scale_ = mediumRangeReticleScale_;

	farReticleM_ = Mesh::LoadFormOBJ("3DReticle", false);
	farReticleO_ = Object3d::Create();
	farReticleO_->SetModel(farReticleM_.get());
	farReticleO_->worldTransform.parent_ = pairWTF;
	farReticleO_->worldTransform.translation_.z = offsetFarPos_;
	farReticleO_->worldTransform.rotation_.y = offsetRotY_;
	farReticleO_->worldTransform.scale_ = farReticleScale_;

	nierReticleO_->Update();
	mediumRangeReticleO_->Update();
	farReticleO_->Update();

	//collider_ = CollisionManager::GetInstance();
	//isRockOn_ = false;

	//raycastHit = std::make_unique<RaycastHit>();
	
}

void PlayerReticle::Update()
{
	/*Vector3 forwardNorm = farReticleO_->worldTransform.matWorld_.GetWorldPos() - nierReticleO_->worldTransform.matWorld_.GetWorldPos();
	forwardNorm.nomalize();

	Ray ray;
	ray.start = nierReticleO_->worldTransform.parent_->translation_;
	ray.dir = forwardNorm;
	
	RaycastHit raycastTest;
	
	if (collider_->Raycast(ray, &raycastTest, 60.f) == true && isRockOn_ == false) {
		if (raycastTest.collider->GetAttribute() == COLLISION_ATTR_ENEMIES) {
			isRockOn_ = true;
		}
	}
	else {
		isRockOn_ = false;
	}*/
	
	
	
	nierReticleO_->Update();
	mediumRangeReticleO_->Update();
	farReticleO_->Update();

}

void PlayerReticle::Draw(ID3D12GraphicsCommandList* cmdList)
{
	nierReticleO_->Draw(cmdList);
	mediumRangeReticleO_->Draw(cmdList);
	farReticleO_->Draw(cmdList);
}
