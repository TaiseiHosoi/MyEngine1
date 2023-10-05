#include "Quaternion.h"
#include "Matrix4.h"
#include"MathFunc.h"
#include <math.h>

Quaternion Quaternion::Multiply(const Quaternion& q1, const Quaternion& q2) {
	return Quaternion(
		q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
		q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
		q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
		q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w
	);
}

Quaternion Quaternion::IdentityQuaternion()
{
	return *this = { 0.0f,0.0f,0.0f,1.0f };
}

Quaternion Quaternion::Conjugate()
{
	Quaternion result;

	result.w = 1;
	result.x = 0;
	result.y = 0;
	result.z = 0;

	return result;
}

float Quaternion::Norm(const Quaternion& quaternion)
{
	float norm = sqrtf(
		quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z +
		quaternion.w * quaternion.w);

	return norm;
}

Quaternion Quaternion::Normalize(const Quaternion& quaternion)
{
	Quaternion q = quaternion;

	float len = Norm(q);
	if (len != 0) {
		return *this = q / len;
	}

	return *this = q;
}

Quaternion Quaternion::Inverse(const Quaternion& quaternion)
{
	Quaternion result = quaternion;

	float norm = Norm(result);
	result = Conjugate();
	result /= (norm * norm);

	return result;
}

Matrix4 Quaternion::Quaternion2Matrix() const
{
	Matrix4 mat;
	mat = mat.identity();

	mat.m[0][0] = (w * w) + (x * x) - (y * y) - (z * z);
	mat.m[0][1] = (x * y * 2) + (z * w * 2);
	mat.m[0][2] = (x * z * 2) - (y * w * 2);

	mat.m[1][0] = (x * y * 2) - (z * w * 2);
	mat.m[1][1] = (w * w) - (x * x) + (y * y) - (z * z);
	mat.m[1][2] = (y * z * 2) + (x * w * 2);

	mat.m[2][0] = (x * z * 2) + (y * w * 2);
	mat.m[2][1] = (y * z * 2) - (x * w * 2);
	mat.m[2][2] = (w * w) - (x * x) - (y * y) + (z * z);

	return mat;
}

Quaternion Quaternion::MakeAxisAngle(const Vector3& axis, float angle)
{
	Quaternion q(
		axis.x * sinf(angle / 2.0f),
		axis.y * sinf(angle / 2.0f),
		axis.z * sinf(angle / 2.0f),
		cosf(angle / 2.0f)
	);
	Normalize(q);
	return q;
}

Quaternion Quaternion::RotateVector(const Quaternion& rotation, const Quaternion& vectorQuat)
{
	Quaternion conjugate = rotation;
	conjugate.x = -conjugate.x;
	conjugate.y = -conjugate.y;
	conjugate.z = -conjugate.z;

	Quaternion result = Multiply(rotation, Multiply(vectorQuat, conjugate));
	return result;
}



Quaternion Quaternion::Slerp(const Quaternion& q0, const Quaternion& q1, float t)
{
	float dot = Dot(q0, q1);
	Quaternion q0_ = q0;



	if (dot < 0) {
		Quaternion invQ0 = { -q0.x,-q0.y,-q0.z,-q0.w };
		q0_ = invQ0;
		dot = -dot;
	}

	if (dot >= 1.0f - 0.005f) {
		return { (1.0f - t) * q0.x + (1.0f - t) * q1.x,
			  (1.0f - t) * q0.y + (1.0f - t) * q1.y,
			  (1.0f - t) * q0.z + (1.0f - t) * q1.z,
			  (1.0f - t) * q0.w + (1.0f - t) * q1.w };

	}


	float theta = acosf(dot);

	float scale1 = (sinf((1 - t) * theta) / sinf(theta));
	float scale2 = sinf(t * theta) / sinf(theta);

	Quaternion result =
	{ scale1 * q0.x + scale2 * q1.x,
	  scale1 * q0.y + scale2 * q1.y,
	  scale1 * q0.z + scale2 * q1.z,
	  scale1 * q0.w + scale2 * q1.w };


	return result;
}

float Quaternion::Dot(const Quaternion& q0, const Quaternion& q1)
{
	float result = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.z * q1.z;
	return result;
}

Quaternion Quaternion::DirectionToDirection(const Vector3& u, const Vector3& v)
{
	// uとvを正規化して内積を求める。u,vを単位ベクトル前提とするなら正規化は不要
	float dot = u.x * v.x + u.y * v.y + u.z * v.z;

	// 外積をとる
	Vector3 cross = u.cross(v);

	// 軸は単位ベクトルである必要があるので正規化
	Vector3 axis = cross.nomalize();

	// 単位ベクトルでない席を取っているのでacosで角度を求める
	float theta = acosf(dot);

	// axisとthetaで任意軸回転を作って返す
	Quaternion result;

	result = MakeAxisAngle(axis, theta);

	return result;
}
Matrix4 Quaternion::ToMatrix4(const Quaternion& q) {
	float xx = q.x * q.x * 2.0f;
	float yy = q.y * q.y * 2.0f;
	float zz = q.z * q.z * 2.0f;
	float xy = q.x * q.y * 2.0f;
	float xz = q.x * q.z * 2.0f;
	float yz = q.y * q.z * 2.0f;
	float wx = q.w * q.x * 2.0f;
	float wy = q.w * q.y * 2.0f;
	float wz = q.w * q.z * 2.0f;



	Matrix4 result = {
		1.0f - yy - zz,        xy + wz,       xz - wy, 0.0f,
			   xy - wz, 1.0f - xx - zz,       yz + wx,0.0f,
			   xz + wy,        yz - wx,1.0f - xx - yy,0.0f,
				  0.0f,           0.0f,          0.0f,1.0f
	};

	return result;
}

Quaternion Quaternion::operator/(const float& s) const
{
	return Quaternion(x / s, y / s, z / s, w / s);
}

Quaternion Quaternion::operator*(const float& s) const
{
	return Quaternion(x * s, y * s, z * s, w * s);
}

Quaternion& Quaternion::operator/=(const float& s)
{
	Quaternion result = *this;
	result = result / s;
	*this = result;

	return *this;
}

Quaternion& Quaternion::operator*=(const float& s)
{
	Quaternion result = *this;
	result = result * s;
	*this = result;

	return *this;
}