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
	object3d_->SetPosition( object3d_->GetPosition());
	nowPhase_ = MOVE_PHASE::forward;	//自機の挙動が何から始まるか
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
	// レールカメラ情報から現在の進行度を求める
	if (railCameraInfo_ != nullptr) {
		primaryPos_ = MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate);	
		
	}
	
	object3d_->worldTransform.translation_ = MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate + advancedValue_);

	// パターン行動
	if (oldFlamePhase_ != nowPhase_) {
		moveCount_ = 0;
	}
	oldFlamePhase_ = nowPhase_;

	if (nowPhase_ == MOVE_PHASE::none) {

	}
	else if (nowPhase_ == MOVE_PHASE::forward) {
		Forward();
		
	}
	else if (nowPhase_ == MOVE_PHASE::turn) {
		Turn();
	}

	// 回転角,位置計算
	Vector3 nowOffset;
	if (railCameraInfo_ != nullptr) {
		//進行上の向いている方向(顔の向きではない)
		directionLoot_ = MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate + offsetBattlePosTimeRate_)
			- MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate);
		directionLoot_.nomalize();

		//自身のtranslation計算
		

		//ワールド上の自機の回転量yを求める
		float dirAngle = atan2(directionLoot_.x, directionLoot_.z);
		nowOffset = offsetPos_;
		nowOffset = MathFunc::RotateVecAngleY(nowOffset,dirAngle);
		nowOffset.y = 0;	// yの値は別計算
		object3d_->worldTransform.translation_ += nowOffset;
		

		//y軸回転で前を向く
		object3d_->worldTransform.rotation_.y = adjustFAngle_ + dirAngle + MathFunc::PI;
	

	}

	// 当たり判定計算
	if (sphere_->GetIsHit() == true) {
		if (sphere_->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_ALLIESBULLETS) {
			state_.hp_--;
		}
	}

	if (state_.hp_ <= 0) {
		state_.isDead_ = true;
	}
	
	// コライダー更新
	colliderPos_ = object3d_->worldTransform.matWorld_.GetWorldPos();
	sphere_->Update();

	//object3d更新
	object3d_->Update();
	
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
	int maxTime = maxFowardTime_;
	if (moveCount_ < maxTime) {
		moveCount_++;
	}
	else {
		moveCount_ = 0;
		nowPhase_ = MOVE_PHASE::turn;
	}
	
	
	advancedValue_ = Ease::LinearEaseOutEasing(spownBattlePosTimeRate_,offsetBattlePosTimeRate_,moveCount_,maxTime, forwardEaseStrength);
	object3d_->worldTransform.translation_.y = Ease::LinierEaseInOutEasing(apparancePosY_,offsetBattlePosY_,moveCount_, maxTime, forwardEaseStrength);
	//ImGui::Begin("enemy");
	//ImGui::InputInt("count",&moveCount_);
	//ImGui::InputFloat("y", &object3d_->worldTransform.translation_.y);
	//ImGui::End();

}

void WalkingEnemy::Turn()
{
	int maxTime = maxFowardTime_;
	if (moveCount_ < maxTime) {
		moveCount_++;
	}
	else {
		moveCount_ = 0;
		nowPhase_ = MOVE_PHASE::none;
	}

	adjustFAngle_ = Ease::LinierEaseInOutEasing(minAdjustFAngle_, maxAdjustFAngle_, moveCount_, maxTime, turnEaseStrength) * MathFunc::PI / 180.f;
}

bool WalkingEnemy::compultionTrue()
{
	return true;
}


