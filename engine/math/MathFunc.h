/**
 * @file MathFunc.h
 * @brief 自作数学関数管理クラス
 */
#pragma once
#include <DirectXMath.h>
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"
#include <vector>


//#include "Object3d.h"
//#include <WorldTransform.h>


namespace MathFunc {

	//円周率
	const float PI = 3.141592654f;
	// 度数法->ラジアン変換
	float Dig2Rad(float value);
	// ラジアン->度数法変換 
	float Rad2Dig(float value);

	// 初期化
	Matrix4 Initialize();
	// スケーリング
	Matrix4 Scale(Vector3 scale);
	// ローテート
	Matrix4 Rotation(Vector3 rotation, int X_1_Y_2_Z_3_XYZ_6);
	// 移動
	Matrix4 Move(Vector3 move);	

	//ベクトルとマトリックスの掛け算
	Vector3 MatVector(const Vector3 vector3, const Matrix4 matrix4);

	// ワールドトランスフォームゲッタ
	Vector3 GetWorldtransform(const Matrix4 matrix4);

	// ベクトル足し算
	Vector3 AddVector(const Vector3 v1, const Vector3 v2);

	//matrix4とVec3のベロシティを求める
	Vector3 bVelocity(const Vector3 velocity, const Matrix4 mat);

	// Vector3を任意の角度で回転
	Vector3 RotateVecAngleY(Vector3 v, float angle);	

	// Y軸の回転をVec3のx,zから求める
	float angleYAxis(const Vector3 v);	//Y軸の角度を求める

	//座標変換
	Vector3 wDivision(const Vector3& v, const Matrix4& m);
	
	// focalLengsからYの角度が求まる
	float FieldOfViewY(float focalLengs, float sensor);

	// XMMat->Mat4変換
	Matrix4 ConvertXMMATtoMat4(DirectX::XMMATRIX XMMatrix);

	// Mat4変換->XMMat
	DirectX::XMMATRIX ConvertMat4toXMMat(Matrix4 m);

	// 逆行列を求める
	Matrix4 MakeInverse(const Matrix4* mat);

	// ラープ関数
	Vector3 lerp(const Vector3& start, const Vector3& end, const float t);

	// スプラインラープ関数
	Vector3 slarp(const Vector3& v1, const Vector3& v2, float t);

	// 直前と直後のポイントを考慮している&距離を考慮してスピードを一定にするTangentSplinePosition
	Vector3 InterpolateBetweenPoints(const std::vector<Vector3>& points, float& totalTime);	

	// タンジェント角を有効にしたスプライン曲線関数
	Vector3 TangentSplinePosition(const std::vector<Vector3>& points, size_t startIndex, float t);

	// タンジェント角計算
	Vector3 CalculateTangent(const Vector3& prevPoint, const Vector3& nextPoint);

	//Vec3の距離計算
	float CalculateDistance(const Vector3& a, const Vector3& b);	//2点間の距離
};

