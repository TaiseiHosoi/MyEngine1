#include "MathFunc.h"
#include<iostream>
#include <cmath>


const float PI = 3.141592f;

float MathFunc::Dig2Rad(float dig)
{
	float rad;

	rad = dig * (PI / 180);

	return rad;
}

float MathFunc::Rad2Dig(float rad)
{
	float deg;

	deg = rad * (180 / PI);

	return deg;
}

Matrix4 MathFunc::Initialize() {
	Matrix4 matInitialize = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return matInitialize;
}

Matrix4 MathFunc::Scale(Vector3 scale) {

	//スケーリング行列を宣言
	Matrix4 matScale = {
		scale.x, 0.0f, 0.0f,    0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f,    0.0f, scale.z, 0.0f,
		0.0f, 0.0f,    0.0f, 1.0f
	};

	Matrix4 scaling = Initialize();
	scaling *= matScale;

	return scaling;
}

Matrix4 MathFunc::Rotation(Vector3 rotation, int X_1_Y_2_Z_3_XYZ_6) {
	int rotationX = 1;
	int rotationY = 2;
	int rotationZ = 3;
	int rotationXYZ = 6;

	if (X_1_Y_2_Z_3_XYZ_6 == rotationX) {
		Matrix4 matRotX = {
		  1.0f,0.0f,0.0f,0.0f,
		  0.0f,cos(rotation.x),sin(rotation.x),0.0f,
		  0.0f,-sin(rotation.x),cos(rotation.x),0.0f,
		  0.0f,0.0f,0.0f,1.0f
		};
		Matrix4 rotationX = Initialize();
		rotationX *= matRotX;

		return rotationX;
	}
	else if (X_1_Y_2_Z_3_XYZ_6 == rotationY) {
		Matrix4 matRotY = {
			cos(rotation.y), 0.0f, -sin(rotation.y), 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			sin(rotation.y), 0.0f, cos(rotation.y),  0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
		Matrix4 rotationY = Initialize();
		rotationY *= matRotY;

		return rotationY;
	}
	else if (X_1_Y_2_Z_3_XYZ_6 == rotationZ) {
		Matrix4 matRotZ = {
		  cos(rotation.z),sin(rotation.z),0.0f,0.0f,
		  -sin(rotation.z),cos(rotation.z),0.0f,0.0f,
		  0.0f,0.0f,1.0f,0.0f,
		  0.0f,0.0f,0.0f,1.0f
		};
		Matrix4 rotationZ = Initialize();
		rotationZ *= matRotZ;

		return rotationZ;
	}
	else {
		Matrix4 matRotXYZ;
		Matrix4 matRot_X, matRot_Y, matRot_Z;
		matRot_X = {
		  1.0f,0.0f,0.0f,0.0f,
		  0.0f,cos(rotation.x),sin(rotation.x),0.0f,
		  0.0f,-sin(rotation.x),cos(rotation.x),0.0f,
		  0.0f,0.0f,0.0f,1.0f
		};
		matRot_Y = {
			cos(rotation.y), 0.0f, -sin(rotation.y),
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			sin(rotation.y), 0.0f, cos(rotation.y),
			0.0f, 0.0f, 0.0f, 0.0f, 1.0f
		};
		matRot_Z = {
		  cos(rotation.z),sin(rotation.z),0.0f,0.0f,
		  -sin(rotation.z),cos(rotation.z),0.0f,0.0f,
		  0.0f,0.0f,1.0f,0.0f,
		  0.0f,0.0f,0.0f,1.0f
		};
		//各軸の回転行列を合成
		matRotXYZ = Initialize();

		matRotXYZ *= matRot_X;
		matRotXYZ *= matRot_Y;
		matRotXYZ *= matRot_Z;

		return matRotXYZ;
	}
}

Matrix4 MathFunc::Move(Vector3 Move) {
	Matrix4 matMove = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		Move.x, Move.y, Move.z, 1.0f
	};
	
	return matMove;
}

//void AffinTrans::affin(WorldTransform& affin) {
//	affin.matWorld_ = Initialize();
//	affin.matWorld_ *= Scale(affin.scale_);
//	affin.matWorld_ *= Rotation(affin.rotation_, 6);
//	affin.matWorld_ *= Move(affin.translation_);
//}

Vector3 MathFunc::MatVector(const Vector3 vector3, const Matrix4 matrix4) {

	Vector3 retVec = {};

	retVec.x =
		vector3.x * matrix4.m[0][0] + vector3.y * matrix4.m[1][0] + vector3.z * matrix4.m[2][0];

	retVec.y =
		vector3.x * matrix4.m[0][1] + vector3.y * matrix4.m[1][1] + vector3.z * matrix4.m[2][1];

	retVec.z =
		vector3.x * matrix4.m[0][2] + vector3.y * matrix4.m[1][2] + vector3.z * matrix4.m[2][2];

	return retVec;

}

Vector3 MathFunc:: bVelocity(Vector3& velocity, Matrix4& mat) {

	Vector3 result = { 0, 0, 0 };


	result.x = velocity.x * mat.m[0][0] +
		velocity.y * mat.m[1][0] +
		velocity.z * mat.m[2][0];

	result.y = velocity.x * mat.m[0][1] +
		velocity.y * mat.m[1][1] +
		velocity.z * mat.m[2][1];

	result.z = velocity.x * mat.m[0][2] +
		velocity.y * mat.m[1][2] +
		velocity.z * mat.m[2][2];


	return result;
}

Vector3 MathFunc::RotateVecAngleY(Vector3 v, float angle)
{
	float cosA = cos(angle);
	float sinA = sin(angle);

	Vector3 result(
		v.x * cosA + v.z * sinA,
		v.y,
		-v.x * sinA + v.z * cosA
	);

	return result;
}

float MathFunc::angleYAxis(const Vector3 v)
{
	// ベクトルをX-Z平面に射影
	Vector3 projectedVector(v.x, 0.0f, v.z);

	// ベクトルを正規化
	projectedVector.nomalize();

	// X軸とのなす角を計算
	float angle = std::atan(projectedVector.dot(Vector3(0.0f, 0.0f, 1.0f)));


	return angle;
}



Vector3 MathFunc::GetWorldtransform(const Matrix4 matrix4) {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分
	worldPos.x = matrix4.m[3][0];
	worldPos.y = matrix4.m[3][1];
	worldPos.z = matrix4.m[3][2];

	return worldPos;
}

Vector3 MathFunc::AddVector(const Vector3 v1, const Vector3 v2) {

	Vector3 addVec = {};

	addVec.x = v1.x + v2.x;

	addVec.y = v1.y + v2.y;

	addVec.z = v1.z + v2.z;

	return addVec;

}



Vector3 MathFunc::wDivision(const Vector3& vector3, const Matrix4& matrix4) {

	Vector4 divVec = {};

	divVec.x = vector3.x * matrix4.m[0][0] + vector3.y * matrix4.m[1][0] + vector3.z * matrix4.m[2][0] + 1 * matrix4.m[3][0];

	divVec.y = vector3.x * matrix4.m[0][1] + vector3.y * matrix4.m[1][1] + vector3.z * matrix4.m[2][1] + 1 * matrix4.m[3][1];

	divVec.z = vector3.x * matrix4.m[0][2] + vector3.y * matrix4.m[1][2] + vector3.z * matrix4.m[2][2] + 1 * matrix4.m[3][2];

	divVec.w = vector3.x * matrix4.m[0][3] + vector3.y * matrix4.m[1][3] + vector3.z * matrix4.m[2][3] + 1 * matrix4.m[3][3];

	divVec.x = divVec.x / divVec.w;

	divVec.y = divVec.y / divVec.w;

	divVec.z = divVec.z / divVec.w;

	return { divVec.x, divVec.y, divVec.z };
}

float MathFunc::FieldOfViewY(float focalLengs, float sensor) {

	return 2 * atan(sensor / (2 * focalLengs));

}

Matrix4 MathFunc::ConvertXMMATtoMat4(DirectX::XMMATRIX XMMatrix) {
	Matrix4 result;
	for (int i = 0; i < 4; i++) {

		result.m[i][0] = DirectX::XMVectorGetX(XMMatrix.r[i]);
		result.m[i][1] = DirectX::XMVectorGetY(XMMatrix.r[i]);
		result.m[i][2] = DirectX::XMVectorGetZ(XMMatrix.r[i]);
		result.m[i][3] = DirectX::XMVectorGetW(XMMatrix.r[i]);
	}


	return result;
}

DirectX::XMMATRIX MathFunc::ConvertMat4toXMMat(Matrix4 m) {
	DirectX::XMMATRIX result;
	result = DirectX::XMMatrixSet(
		m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3],
		m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3],
		m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3],
		m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]);

	return result;
}

Matrix4 MathFunc::MakeInverse(const Matrix4* mat)
{
	assert(mat);

	//掃き出し法を行う行列
	float sweep[4][8]{};
	//定数倍用
	float constTimes = 0.0f;
	//許容する誤差
	float MAX_ERR = 1e-10f;
	//戻り値用
	Matrix4 retMat;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//weepの左側に逆行列を求める行列をセット
			sweep[i][j] = mat->m[i][j];

			//sweepの右側に単位行列をセット
			sweep[i][4 + j] = Matrix4::MakeIdentity().m[i][j];
		}
	}

	//全ての列の対角成分に対する繰り返し
	for (int i = 0; i < 4; i++)
	{
		//最大の絶対値を注目対角成分の絶対値と仮定
		float max = fabs(sweep[i][i]);
		int maxIndex = i;

		//i列目が最大の絶対値となる行を探す
		for (int j = i + 1; j < 4; j++)
		{
			if (fabs(sweep[j][i]) > max)
			{
				max = fabs(sweep[j][i]);
				maxIndex = j;
			}
		}

		if (fabs(sweep[maxIndex][i]) <= MAX_ERR)
		{
			//逆行列は求められない
			return Matrix4::MakeIdentity();
		}

		//操作(1):i行目とmaxIndex行目を入れ替える
		if (i != maxIndex)
		{
			for (int j = 0; j < 8; j++)
			{
				float tmp = sweep[maxIndex][j];
				sweep[maxIndex][j] = sweep[i][j];
				sweep[i][j] = tmp;
			}
		}

		//sweep[i][i]に掛けると1になる値を求める
		constTimes = 1 / sweep[i][i];

		//操作(2):p行目をa倍する
		for (int j = 0; j < 8; j++)
		{
			//これによりsweep[i][i]が1になる
			sweep[i][j] *= constTimes;
		}

		//操作(3)によりi行目以外の行のi列目を0にする
		for (int j = 0; j < 4; j++)
		{
			if (j == i)
			{
				//i行目はそのまま
				continue;
			}

			//i行目に掛ける値を求める
			constTimes = -sweep[j][i];

			for (int k = 0; k < 8; k++)
			{
				//j行目にi行目をa倍した行を足す
				//これによりsweep[j][i]が0になる
				sweep[j][k] += sweep[i][k] * constTimes;
			}
		}
	}

	//sweepの右半分がmatの逆行列
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			retMat.m[i][j] = sweep[i][4 + j];
		}
	}

	return retMat;
}

Vector3 MathFunc::lerp(const Vector3& start, const Vector3& end, const float t) {

	return start * (1.0f - t) + end * t;
}

Vector3 MathFunc::slarp(const Vector3& v1, const Vector3& v2, float t)
{
	// tをクランプして[0, 1]の範囲に収める（tが0以下ならv1、tが1以上ならv2を返す）
	t = std::max(0.0f, std::min(1.0f, t));

	// 補間結果を計算して返す
	return v1 + (v2 - v1) * t;
}

Vector3 MathFunc::InterpolateBetweenPoints(const std::vector<Vector3>& points, float& totalTime)
{
	size_t numPoints = points.size();

	if (numPoints == 0) {
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	if (numPoints == 1) {
		return points[0];
	}

	//合計距離を計算
	float totalDistance = 0.0f;
	for (size_t i = 0; i < numPoints - 1; ++i) {
		totalDistance += CalculateDistance(points[i], points[i + 1]);
	}

	//一定の速度で移動する時間を計算
	float speed = 1.0f; //速度を調整
	float scaledTime = speed * totalTime / totalDistance;

	//現在のセグメントと距離を計算
	size_t currentIndex = 0;
	float segmentDistance = CalculateDistance(points[currentIndex], points[currentIndex + 1]);

	while (scaledTime > segmentDistance) {
		scaledTime -= segmentDistance;
		currentIndex++;

		if (currentIndex >= numPoints - 1) {
			return points[numPoints - 1];
		}

		segmentDistance = CalculateDistance(points[currentIndex], points[currentIndex + 1]);
	}

	//現在のポイントと次のポイントの間を補間
	float tInSegment = scaledTime / segmentDistance;
	return lerp(points[currentIndex], points[currentIndex + 1], tInSegment);
}

Vector3 MathFunc::TangentSplinePosition(const std::vector<Vector3>& points, size_t startIndex, float t)
{
	// 補間すべき点の数
	size_t n = points.size() - 2;

	if (startIndex > n) return points[n];	//値を使わない
	if (startIndex < 1) return points[1];

	// 各points間の距離を計算
	std::vector<float> distances;
	float totalDistance = 0.0f;
	for (size_t i = 0; i < n; i++)
	{
		float distance = (points[i + 1] - points[i]).length();
		distances.push_back(distance);
		totalDistance += distance;
	}

	// 指定されたtに基づいて移動すべきpoints間を見つける
	float targetDistance = totalDistance * t;
	float currentDistance = 0.0f;
	size_t targetSegment = 0;
	for (size_t i = 0; i < n; i++)
	{
		currentDistance += distances[i];
		if (currentDistance >= targetDistance)
		{
			targetSegment = i;
			break;
		}
	}

	// セグメント内でのtを計算
	float segmentT = (targetDistance - (currentDistance - distances[targetSegment])) / distances[targetSegment];


	Vector3 p0 , p1, p2, p3;


	if (targetSegment == points.size() - 3) {
		p0 = points[targetSegment];
		p1 = points[0];
		p2 = points[1];
		p3 = points[2];
	}
	else if (targetSegment == points.size() - 2) {
		p0 = points[targetSegment];
		p1 = points[targetSegment + 1];
		p2 = points[0];
		p3 = points[1];
	}
	else if (targetSegment == points.size()-1) 
	{
		p0 = points[targetSegment];
		p1 = points[targetSegment + 1];
		p2 = points[targetSegment + 2];
		p3 = points[0];
	}
	else {
		p0 = points[targetSegment];
		p1 = points[targetSegment + 1];
		p2 = points[targetSegment + 2];
		p3 = points[targetSegment + 3];
	}

	Vector3 tangent0 = CalculateTangent(p0, p1);
	Vector3 tangent1 = CalculateTangent(p1, p2);
	Vector3 tangent2 = CalculateTangent(p2, p3);

	Vector3 position = 0.5 * (2 * p1 + (-p0 + p2) * segmentT + (2 * p0 - 5 * p1 + 4 * p2 - p3) * (segmentT * segmentT) + (-p0 + 3 * p1 - 3 * p2 + p3) * (segmentT * segmentT * segmentT));

	Vector3 tangent = lerp(tangent1, tangent2, segmentT); // 補完された接線ベクトル

	// ポジションに接線ベクトルを適用して位置を修正
	float someScaleFactor = 1.0f; // スプラインの柔らかさ
	position += tangent * someScaleFactor; // 適切なスケールファクターを使って修正

	return position;
}

Vector3 MathFunc::CalculateTangent(const Vector3& prevPoint, const Vector3& nextPoint)
{
	Vector3 calcVec = nextPoint - prevPoint;
	return calcVec.nomalize();
}

float MathFunc::CalculateDistance(const Vector3& a, const Vector3& b)
{
	return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y) + (b.z - a.z) * (b.z - a.z));
}
