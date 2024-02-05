#include "CameraShake.h"
#include "ImGui.h"

void CameraShake::Initialize()
{
	rotateShakeTheta_ = 0;
	isMagnitude_ = false;
	magnitudeVec_ = {};
	nowMagnitudeLen_ = 0;
}

void CameraShake::Update()
{

	//回転振動の回転計さん
	rotateShakeTheta_ += rotateShakeThetaSpeed_;
	if (rotateShakeTheta_ > 2.f) {
		rotateShakeTheta_ -= 2.f;
	}

	

	//方向適用前計算
	nowMoveVec_.x = cosf(rotateShakeTheta_) * nowMagnitudeLen_;
	nowMoveVec_.y = sinf(rotateShakeTheta_) * nowMagnitudeLen_;

	//カメラに適用させる為の計算
	magnitudeVec_ = MathFunc::RotateVecAngleY(nowMoveVec_, cameraDirAngle_);

}

void CameraShake::SetCameraDirAngle(float angle)
{

	cameraDirAngle_ = angle;

}


