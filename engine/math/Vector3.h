/**
 * @file Vector3.h
 * @brief ベクター3クラス
 */
#pragma once

class Vector3
{
public:


	float x;
	float y;
	float z;

public:

	// コンストラクタ
	Vector3();
	Vector3(float x, float y, float z);

	//　距離
	float length() const;
	// 法線処理
	Vector3& nomalize();
	// 内積
	float dot(const Vector3& v) const;
	// 外積
	Vector3 cross(const Vector3& v) const;
	// 引き算
	Vector3 sub(const Vector3& v)const;
	// ラープ
	Vector3 lerp(const Vector3& start, const Vector3& end, const float t);
	// スプラインラープ
	Vector3 slarp(const Vector3& v1, const Vector3& v2, float t);
	
	Vector3 operator+() const;
	Vector3 operator-() const;

	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);

};

const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);