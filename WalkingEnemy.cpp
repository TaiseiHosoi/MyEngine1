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
	object3d_->SetScale({ 4,4,4});
	object3d_->SetPosition({ object3d_->GetPosition().x,4,object3d_->GetPosition().z });
	nowPhase_ = 1;	//自機の挙動が何から始まるか
	advancedValue_ = 0.0f;
	

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
		directionLoot_ = MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate + 0.01f)
			- MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate);
		directionLoot_.nomalize();

		float dirAngle = MathFunc::angleYAxis(directionLoot_);
		nowOffset = offsetPos_;
		nowOffset = MathFunc::RotateVecAngleY(nowOffset,dirAngle);
		object3d_->worldTransform.translation_ += nowOffset;


	}



	object3d_->worldTransform.rotation_.y = -playerWorldTransform->rotation_.y;
	object3d_->Update();

	ImGui::Begin("WalkEnemy");
	ImGui::InputFloat3("nowOffset",&nowOffset.x);
	ImGui::End();

}

void WalkingEnemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	object3d_->Draw(cmdList);
}

void WalkingEnemy::Forward()
{
	int maxTime = 120;
	if (moveCount_ < maxTime) {
		moveCount_++;
	}
	
	advancedValue_ = Ease::LinearEaseOutEasing(-0.002f,0.01f,moveCount_,maxTime);

	

}


