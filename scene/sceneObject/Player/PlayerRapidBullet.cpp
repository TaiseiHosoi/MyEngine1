#include "PlayerRapidBullet.h"
#include <imgui.h>



void PlayerRapidBullet::Initialize(Mesh* model, Vector3 setPos, Vector3 setRot)
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
	pos = object_->worldTransform.matWorld_.GetWorldPos();
	sphere->SetAttribute(COLLISION_ATTR_ALLIESBULLETS);
	sphere->SetBasisPos(&pos);
	sphere->SetRadius(1.0f);
	sphere->Update();

	testObject_ = Object3d::Create();
	testObject_->Initialize(true);
	testObject_->SetModel(model_);
	testObject_->SetPosition({0,0,0});
	testObject_->SetRotate({ 0,0,0 });
	testObject_->SetScale({ 1,1,1 });
	testObject_->Update();
	
}

void PlayerRapidBullet::Update()
{
	

	//弾の方向ベクトル
	Vector3 bulletVec = { 0,0,bulletSpeed_ };
	bulletVec = MathFunc::MatVector(bulletVec, object_->worldTransform.matWorld_);

	object_->worldTransform.translation_ += bulletVec;

	//更新
	object_->Update();

	testObject_->worldTransform.translation_ = object_->worldTransform.translation_;
	sphere->SetBasisPos(&object_->worldTransform.translation_);
	sphere->Update();

	testObject_->Update();


}

void PlayerRapidBullet::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//object_->Draw(cmdList);
	testObject_->Draw(cmdList);
}
