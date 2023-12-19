#include "EnemyBoundBall.h"

EnemyBoundBall::~EnemyBoundBall()
{
	CollisionManager::GetInstance()->RemoveCollider(sphere.get());
}

void EnemyBoundBall::Initialize(Mesh* model, const Vector3& setPos, const Vector3& setRot)
{
	//ObjectInit
	model_ = model;
	object_ = Object3d::Create();
	object_->Initialize(true);
	object_->SetModel(model_);
	object_->SetPosition(setPos);
	object_->SetRotate(setRot);
	object_->SetScale({ 1,1,1 });
	object_->Update();

	sphere = std::make_unique<SphereCollider>();
	CollisionManager::GetInstance()->AddCollider(sphere.get());
	colPos_ = object_->worldTransform.matWorld_.GetWorldPos();
	sphere->SetAttribute(COLLISION_ATTR_ENEMIEBULLETS);
	sphere->SetBasisPos(&colPos_);
	sphere->SetRadius(object_->worldTransform.scale_.x);
	sphere->Update();
}

void EnemyBoundBall::Update()
{
	countSinceBirth_++;
	if (countSinceBirth_ >= maxCountSinceBirth_) {
		isDead_ = true;
	}

	if (object_->GetWorldTransformPtr()->translation_.y <= lowestPos_) {
		nowSpeed_ = offsetBoundSpeed_;
		boundCount_++;
	}else{
		nowSpeed_ -= gravityVel_;
	}

	if (boundCount_ > 0) {
		object_->GetWorldTransformPtr()->rotation_.z += rollSpeed_;
	}

	
	object_->GetWorldTransformPtr()->translation_.y += nowSpeed_;
	

	//更新
	object_->Update();

	sphere->SetBasisPos(&object_->worldTransform.translation_);
	sphere->Update();

}

void EnemyBoundBall::Draw(ID3D12GraphicsCommandList* cmdList)
{
	object_->Draw(cmdList);
}

void EnemyBoundBall::SetRadius(float rad)
{
	object_->worldTransform.scale_ = { rad,rad,rad };
	sphere->SetRadius(object_->worldTransform.scale_.x);
}

