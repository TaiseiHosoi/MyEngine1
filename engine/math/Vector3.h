/**
 * @file Vector3.h
 * @brief �x�N�^�[3�N���X
 */
#pragma once

class Vector3
{
public:


	float x;
	float y;
	float z;

public:

	// �R���X�g���N�^
	Vector3();
	Vector3(float x, float y, float z);

	//�@����
	float length() const;
	// �@������
	Vector3& nomalize();
	// ����
	float dot(const Vector3& v) const;
	// �O��
	Vector3 cross(const Vector3& v) const;
	// �����Z
	Vector3 sub(const Vector3& v)const;
	// ���[�v
	Vector3 lerp(const Vector3& start, const Vector3& end, const float t);
	// �X�v���C�����[�v
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