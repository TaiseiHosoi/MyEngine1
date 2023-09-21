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

	Matrix4 Initialize();
	Matrix4 Scale(Vector3 scale);
	Matrix4 Rotation(Vector3 rotation, int X_1_Y_2_Z_3_XYZ_6);
	Matrix4 Move(Vector3 move);
	//void  affin(WorldTransform& affin);

	Vector3 MatVector(const Vector3 vector3, const Matrix4 matrix4);

	Vector3 GetWorldtransform(const Matrix4 matrix4);

	Vector3 AddVector(const Vector3 v1, const Vector3 v2);

	Vector3 bVelocity(Vector3& velocity, Matrix4& mat);


	//座標変換
	Vector3 wDivision(const Vector3& v, const Matrix4& m);

		
	float FieldOfViewY(float focalLengs, float sensor);

	Matrix4 ConvertXMMATtoMat4(DirectX::XMMATRIX XMMatrix);
	DirectX::XMMATRIX ConvertMat4toXMMat(Matrix4 m);

	Matrix4 MakeInverse(const Matrix4* mat);

	Vector3 lerp(const Vector3& start, const Vector3& end, const float t);
	Vector3 slarp(const Vector3& v1, const Vector3& v2, float t);

	Vector3 InterpolateBetweenPoints(const std::vector<Vector3>& points, float& totalTime);	//直前と直後のポイントを考慮している&距離を考慮してスピードを一定に

	Vector3 TangentSplinePosition(const std::vector<Vector3>& points, size_t startIndex, float t);

	Vector3 CalculateTangent(const Vector3& prevPoint, const Vector3& nextPoint);

	float CalculateDistance(const Vector3& a, const Vector3& b);	//2点間の距離
};

