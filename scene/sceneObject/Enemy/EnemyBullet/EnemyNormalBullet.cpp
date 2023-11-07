#include "EnemyNormalBullet.h"

EnemyNormalBullet::~EnemyNormalBullet()
{
	CollisionManager::GetInstance()->RemoveCollider(sphere.get());
}

void EnemyNormalBullet::Initialize(Mesh* model, Vector3 setPos, Vector3 setRot)
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

void EnemyNormalBullet::Update()
{
	countSinceBirth_++;
	if (countSinceBirth_ >= maxCountSinceBirth_) {
		isDead_ = true;
	}

	//弾の方向ベクトル
	Vector3 bulletVec = { 0,0,bulletSpeed_ };
	bulletVec = MathFunc::MatVector(bulletVec, object_->worldTransform.matWorld_);

	object_->worldTransform.translation_ += bulletVec;

	//更新
	object_->Update();

	sphere->SetBasisPos(&object_->worldTransform.translation_);
	sphere->Update();

}

void EnemyNormalBullet::Draw(ID3D12GraphicsCommandList* cmdList)
{
	object_->Draw(cmdList);
}
