#include "Quaternion.h"
#include "Matrix4.h"
#include"MathFunc.h"
#include <math.h>

Quaternion Quaternion::Multiply(const Quaternion& r)
{
	Quaternion result;
	/*Vector3 vq = {x,y,z};
	Vector3 vr = {r.x,r.y,r.z};

	result.w = w * r.w - vq.dot(vr);
	Vector3 qrv = vq.cross(vr) + r.w * vq + w * vr;
	result.x = qrv.x;
	result.x = qrv.y;
	result.x = qrv.z;*/

	result.w = w * r.w - x * r.x - y * r.y - z * r.z;
	result.x = y * r.z - z * r.y + r.w * x + w * r.x;
	result.y = z * r.x - x * r.z + r.w * y + w * r.y;
	result.z = x * r.y - y * r.x + r.w * z + w * r.z;

	return result;
}

Quaternion Quaternion::IdentityQuaternion()
{
	return *this = { 0.0f,0.0f,0.0f,1.0f };
}

Quaternion Quaternion::Conjugate(const Quaternion& quaternion)
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
	result = Conjugate(result);
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

Vector3 Quaternion::RotateVector(const Vector3& vector)
{

	Quaternion result;

	Quaternion VecQua;

	VecQua.w = 0;
	VecQua.x = vector.x;
	VecQua.y = vector.y;
	VecQua.z = vector.z;

	Quaternion conQuaternion = this->Conjugate(VecQua);

	result = this->Multiply(VecQua);

	result = result.Multiply(conQuaternion);

	return { result.x,result.y,result.z };
}
Matrix4 Quaternion::MakeRotateMatrix(const Quaternion& q)
{
	return Matrix4(
		q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
		2 * (q.x * q.y + q.w * q.z),
		2 * (q.x * q.z - q.w * q.y),
		0,

		2 * (q.x * q.y - q.w * q.z),
		q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
		2 * (q.y * q.z + q.w * q.x),
		0,

		2 * (q.x * q.z + q.w * q.y),
		2 * (q.y * q.z - q.w * q.x),
		q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
		0,

		0, 0, 0, 1
	);
}

Quaternion Quaternion::operator/(const float& s) const
{
	return Quaternion(x / s, y / s, z / s, w / s);
}

Quaternion& Quaternion::operator/=(const float& s)
{
	Quaternion result = *this;
	result = result / s;
	*this = result;

	return *this;
}
