#include "WalkingEnemy.h"
#include "Ease.h"

WalkingEnemy::WalkingEnemy()
{
}

WalkingEnemy::~WalkingEnemy()
{
}

void WalkingEnemy::Initialize(Mesh* model)
{
	model_ = model;
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(true);
	object3d_->SetModel(model_);
	object3d_->SetScale({ 3,3,3});
	object3d_->SetPosition({ object3d_->GetPosition().x,1.5f,object3d_->GetPosition().z });
	nowPhase_ = 1;	//自機の挙動が何から始まるか
	advancedValue_ = 0.0f;

	//当たり判定初期化
	sphere_ = std::make_unique<SphereCollider>();
	CollisionManager::GetInstance()->AddCollider(sphere_.get());
	sphere_->SetAttribute(COLLISION_ATTR_ENEMIES);
	sphere_->SetBasisPos(&colliderPos_);
	sphere_->SetRadius(object3d_->worldTransform.scale_.x);
	sphere_->Update();

	collider_ = CollisionManager::GetInstance();
	
	//ステータス初期化
	state_.hp_ = 1;
	state_.isDead_ = false;
	state_.isAtk_ = false;

}

void WalkingEnemy::Update()
{
	
	if (railCameraInfo_ != nullptr) {
		primaryPos_ = MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate);	
		
	}
	
	if (oldFlamePhase_ != nowPhase_) {
		moveCount_ = 0;
	}
	oldFlamePhase_ = nowPhase_;

	if (nowPhase_ == 0) {

	}
	else if (nowPhase_ == 1) {
		Forward();
		object3d_->worldTransform.translation_ = MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate + advancedValue_);
	}
	Vector3 nowOffset;
	if (railCameraInfo_ != nullptr) {
		//進行上の向いている方向(顔の向きではない)
		directionLoot_ = MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate + 0.005f)
			- MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate);
		directionLoot_.nomalize();

		//ワールド上の自機の回転量yを求める
		float dirAngle = atan2(directionLoot_.x, directionLoot_.z);
		nowOffset = offsetPos_;
		nowOffset = MathFunc::RotateVecAngleY(nowOffset,dirAngle);
		object3d_->worldTransform.translation_ += nowOffset;

		object3d_->worldTransform.translation_.y = 1.5f;

		//y軸回転で前を向く
		object3d_->worldTransform.rotation_.y = dirAngle + 3.14f;


	}

	if (sphere_->GetIsHit() == true) {
		if (sphere_->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_ALLIESBULLETS) {
			state_.hp_--;
		}
	}

	if (state_.hp_ <= 0) {
		state_.isDead_ = true;
	}
	
	//object3d更新
	object3d_->Update();

	//コライダー更新
	colliderPos_ = object3d_->worldTransform.matWorld_.GetWorldPos();
	sphere_->Update();
	
	//ImGui::Begin("WalkingEnemy");
	//ImGui::InputFloat3("translation", &object3d_->worldTransform.translation_.x);
	//ImGui::End();


}

void WalkingEnemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	object3d_->Draw(cmdList);
}

EnemyState* WalkingEnemy::GetState()
{
	return &state_;
}

void WalkingEnemy::Forward()
{
	int maxTime = 120;
	if (moveCount_ < maxTime) {
		moveCount_++;
	}
	
	float easeStrength = 3.0f;
	advancedValue_ = Ease::LinearEaseOutEasing(-0.002f,0.006f,moveCount_,maxTime, easeStrength);

	

}

void WalkingEnemy::Deceleration()
{
}


