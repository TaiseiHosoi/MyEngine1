#pragma once
#include"Vector3.h"
#include"WorldTransform.h"
enum CAMERA_SHAKE_TIMERATE {
	LONG_TIME = 60,
	MEDIUM_TIME = 40,
	SHORT_TIME = 20
};

class CameraShake {
public:	// コンストラクタデストラクタ

	CameraShake();
	~CameraShake();

public:// 関数

	//初期化
	void Initialize();

	//更新
	void Update();

public://アクセッサ
	void SetCameraDirAngle(float* angle);

	void ShakeRotateCamera(int flameCount);

	Vector3* GetShakeVec();


private:// 変数

	Vector3 magnitudeVec_ = {};	//方向を考慮する計算後の変数
	Vector3 nowMoveVec_ = {};
	bool isMagnitude_ = false;
	const float kRotateShakeThetaSpeed_ = 0.5f;
	float rotateShakeTheta_ = 0;
	
	const float kMaxMagnitudeLen_ = 1.0f;	//振動の強さ
	const float kMagnitudeIncVal_ = 0.1f;	//振動の強さが増える値
	const float kMaxRad_ = 2.f;

	float nowMagnitudeLen_ = 0;
	float* cameraDirAngle_ = 0;	//カメラの向いているアングル
	
	bool isMinusMag_ = false;	//振動量を引き算するかのフラグ
	bool isRotShake_ = false;	//シェイクを呼び出す

	int shakeCount_ = 0;	//振動のカウントフレーム
	int oldShakeCount_ = 0;
	

};