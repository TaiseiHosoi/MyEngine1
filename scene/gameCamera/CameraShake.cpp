#include "CameraShake.h"
#include "ImGui.h"


CameraShake::CameraShake()
{
}

CameraShake::~CameraShake()
{
}

void CameraShake::Initialize()
{
	rotateShakeTheta_ = 0;
	isMagnitude_ = false;
	magnitudeVec_ = {};
	nowMagnitudeLen_ = 0;
}

void CameraShake::Update()
{


	if (oldShakeCount_ == 0 && shakeCount_ != 0) {
		isRotShake_ = true;
	}
	else {
		
	}

	if (shakeCount_ > 0) {
		shakeCount_--;
	}


	if (isRotShake_ == true) {
		//ゆっくり振動量が上がってその後下がっていく処理
		if (isMinusMag_ == false) {

			if (nowMagnitudeLen_ < kMaxMagnitudeLen_) {
				nowMagnitudeLen_ += kMagnitudeIncVal_;
			}


			//shakeCountが0になったら小さくなっていく
			if (shakeCount_ <= 0) {
				isMinusMag_ = true;
			}

		}
		else if (isMinusMag_ == true) {

			if (nowMagnitudeLen_ >= 0) {
				nowMagnitudeLen_ -= kMagnitudeIncVal_;
			}
			else {
				isMinusMag_ = false;
				nowMagnitudeLen_ = 0;
				isRotShake_ = false;
			}
		}
	}
	else {
		
	}

	if (nowMagnitudeLen_ < 0) {	//nowMagLenがマイナスにならぬようにする
		nowMagnitudeLen_ = 0;
	}
	


	//回転振動の回転計さん
	rotateShakeTheta_ += kRotateShakeThetaSpeed_;
	if (rotateShakeTheta_ > kMaxRad_) {
		rotateShakeTheta_ -= kMaxRad_;
	}

	

	//方向適用前計算
	nowMoveVec_.x = cosf(rotateShakeTheta_) * nowMagnitudeLen_ / 2.f;
	nowMoveVec_.y = sinf(rotateShakeTheta_) * nowMagnitudeLen_;

	//カメラに適用させる為の計算
	if (nowMagnitudeLen_ <= 0) {	//振動幅が0であるなら計算せず0
		magnitudeVec_ = {};
	}
	else {
		magnitudeVec_ = MathFunc::RotateVecAngleY(nowMoveVec_, *cameraDirAngle_);
	}

	//前フレーム処理
	oldShakeCount_ = shakeCount_;

}

void CameraShake::SetCameraDirAngle(float* angle)
{

	cameraDirAngle_ = angle;

}

void CameraShake::ShakeRotateCamera(int flameCount)
{
	shakeCount_ = flameCount;
	isMinusMag_ = false;
	
	//シェイクがオフだった場合初期化
	//if (isRotShake_ == false) {
	//	
	//	nowMagnitudeLen_ = 0;
	//}
}

Vector3* CameraShake::GetShakeVec()
{
	return &magnitudeVec_;
}


