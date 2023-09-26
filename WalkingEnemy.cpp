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

}

void WalkingEnemy::Update()
{
	if (railCameraInfo_ != nullptr) {
		primaryPos_ = MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate);	
		battleLinePos_ = MathFunc::TangentSplinePosition(railCameraInfo_->points,railCameraInfo_->startIndex, railCameraInfo_->timeRate + 0.02f);
	}
	
	if (oldFlamePhase_ != nowPhase_) {
		moveCount_ = 0;
	}
	oldFlamePhase_ = nowPhase_;

	if (nowPhase_ == 0) {

	}
	else if (nowPhase_ == 1) {
		Forward();
	}

	object3d_->worldTransform.rotation_.y = -playerWorldTransform->rotation_.y;
	object3d_->Update();

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
	
	Vector3 backVel = primaryPos_ - battleLinePos_;
	Vector3 backLinePos = MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate-0.05f);

	Vector3 nowPos = { Ease::LinearEasing(battleLinePos_.x,backLinePos.x,moveCount_,maxTime),
		Ease::LinearEasing(battleLinePos_.y,backLinePos.y,moveCount_,maxTime),
		Ease::LinearEasing(battleLinePos_.z,backLinePos.z,moveCount_,maxTime)
	};

	object3d_->worldTransform.translation_ = nowPos;
	//object3d_->worldTransform.translation_ = battleLinePos_;

	ImGui::Begin("WalkEnemy");
	ImGui::InputFloat3("Pos",&nowPos.x );
	ImGui::InputInt("moveCount", &moveCount_);
	ImGui::End();

}


