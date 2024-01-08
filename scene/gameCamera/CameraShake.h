#pragma once
#include"Vector3.h"
#include"WorldTransform.h"

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
	void SetCameraDirAngle(float angle);


private:// 変数

	Vector3 magnitudeVec_ = {};	//方向を考慮する計算後の変数
	Vector3 nowMoveVec_ = {};
	bool isMagnitude_ = false;
	const float rotateShakeThetaSpeed_ = 0.1f;
	float rotateShakeTheta_ = 0;
	
	const float maxMagnitudeLen_ = 2.f;
	float nowMagnitudeLen_ = 0;

	float cameraDirAngle_ = 0;


};