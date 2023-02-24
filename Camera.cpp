#include "Camera.h"

using namespace DirectX;

Camera::Camera(int window_width, int window_height)
{
	aspectRatio = (float)window_width / window_height;

	//ビュー行列の計算
	UpdateViewMatrix();

	// 射影行列の計算
	UpdateProjectionMatrix();

	// ビュープロジェクションの合成
	matViewProjection = matView * matProjection;
}

void Camera::Update()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
	matViewProjection = matView * matProjection;
}

void Camera::UpdateViewMatrix()
{
	// 視点座標
	Vector3 eyePosition = eye;
	// 注視点座標
	Vector3 targetPosition = target;
	// （仮の）上方向
	Vector3 upVector = up;

	// カメラZ軸（視線方向）
	Vector3 cameraAxisZ = targetPosition - eyePosition;
	
	// 0ベクトルだと向きが定まらないので除外

	// ベクトルを正規化
	cameraAxisZ.nomalize();

	// カメラのX軸（右方向）
	Vector3 cameraAxisX;
	// X軸は上方向→Z軸の外積で求まる
	cameraAxisX = upVector.cross(cameraAxisZ);
	// ベクトルを正規化
	cameraAxisX.nomalize();

	// カメラのY軸（上方向）
	Vector3 cameraAxisY;
	// Y軸はZ軸→X軸の外積で求まる
	cameraAxisY = cameraAxisZ.cross(cameraAxisX);

	// ここまでで直交した3方向のベクトルが揃う
	//（ワールド座標系でのカメラの右方向、上方向、前方向）	

	// カメラ回転行列

	Matrix4 matCameraRot;
	// カメラ座標系→ワールド座標系の変換行列
	matCameraRot.m[0][0] = cameraAxisX.x;
	matCameraRot.m[0][1] = cameraAxisX.y;
	matCameraRot.m[0][2] = cameraAxisX.z;
	matCameraRot.m[0][3] = 0;
	matCameraRot.m[1][0] = cameraAxisY.x;
	matCameraRot.m[1][1] = cameraAxisY.y;
	matCameraRot.m[1][2] = cameraAxisY.z;
	matCameraRot.m[1][3] = 0;
	matCameraRot.m[2][0] = cameraAxisZ.x;
	matCameraRot.m[2][1] = cameraAxisZ.y;
	matCameraRot.m[2][2] = cameraAxisZ.z;
	matCameraRot.m[2][3] = 0;
	matCameraRot.m[3][0] = 0;
	matCameraRot.m[3][1] = 0;
	matCameraRot.m[3][2] = 0;
	matCameraRot.m[3][3] = 1;
	
	// 転置により逆行列（逆回転）を計算
	matView = MakeInverse(&matCameraRot);

	// 視点座標に-1を掛けた座標
	Vector3 reverseEyePosition = -eyePosition;
	// カメラの位置からワールド原点へのベクトル（カメラ座標系）
	float tX = cameraAxisX.dot(reverseEyePosition);	// X成分
	float tY = cameraAxisY.dot(reverseEyePosition);	// Y成分
	float tZ = cameraAxisZ.dot(reverseEyePosition);	// Z成分
	// 一つのベクトルにまとめる
	Vector3 translation = { tX,tY,tZ };
	// ビュー行列に平行移動成分を設定
	matView.m[3][0] = translation.x;
	matView.m[3][1] = translation.y;
	matView.m[3][2] = translation.z;
	matView.m[3][3] = 1;

#pragma region 全方向ビルボード行列の計算
	// ビルボード行列
	matBillboard.m[0][0] = cameraAxisX.x;
	matBillboard.m[0][1] = cameraAxisX.y;
	matBillboard.m[0][2] = cameraAxisX.z;
	matBillboard.m[0][3] = 0;
	matBillboard.m[1][0] = cameraAxisY.x;
	matBillboard.m[1][1] = cameraAxisY.y;
	matBillboard.m[1][2] = cameraAxisY.z;
	matBillboard.m[1][3] = 0;
	matBillboard.m[2][0] = cameraAxisZ.x;
	matBillboard.m[2][1] = cameraAxisZ.y;
	matBillboard.m[2][2] = cameraAxisZ.z;
	matBillboard.m[2][3] = 0;
	matBillboard.m[3][0] = 0;
	matBillboard.m[3][1] = 0;
	matBillboard.m[3][2] = 0;
	matBillboard.m[3][3] = 1;
#pragma region

#pragma region Y軸回りビルボード行列の計算
	// カメラX軸、Y軸、Z軸
	Vector3 ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	// X軸は共通
	ybillCameraAxisX = cameraAxisX;
	// Y軸はワールド座標系のY軸
	ybillCameraAxisY = upVector.nomalize();
	// Z軸はX軸→Y軸の外積で求まる
	ybillCameraAxisZ = ybillCameraAxisX.cross(ybillCameraAxisY);

	// Y軸回りビルボード行列
	matBillboardY.m[0][0] = cameraAxisX.x;
	matBillboardY.m[0][1] = cameraAxisX.y;
	matBillboardY.m[0][2] = cameraAxisX.z;
	matBillboardY.m[0][3] = 0;
	matBillboardY.m[1][0] = cameraAxisY.x;
	matBillboardY.m[1][1] = cameraAxisY.y;
	matBillboardY.m[1][2] = cameraAxisY.z;
	matBillboardY.m[1][3] = 0;
	matBillboardY.m[2][0] = cameraAxisZ.x;
	matBillboardY.m[2][1] = cameraAxisZ.y;
	matBillboardY.m[2][2] = cameraAxisZ.z;
	matBillboardY.m[2][3] = 0;
	matBillboardY.m[3][0] = 0;
	matBillboardY.m[3][1] = 0;
	matBillboardY.m[3][2] = 0;
	matBillboardY.m[3][3] = 1;

#pragma endregion
}

void Camera::UpdateProjectionMatrix()
{
	// 透視投影による射影行列の生成
	 MakePerspectiveL(
		FieldOfViewY(),
		aspectRatio,
		0.1f, 100000.0f, 
		 matProjection
	);
}

void Camera::MoveEyeVector(const Vector3& move)
{
	// 視点座標を移動し、反映
	Vector3 eye_moved = GetEye();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	SetEye(eye_moved);
}



void Camera::MoveVector(const Vector3& move)
{
	// 視点と注視点座標を移動し、反映
	Vector3 eye_moved = GetEye();
	Vector3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetEye(eye_moved);
	SetTarget(target_moved);
}


float Camera::FieldOfViewY() {

	return 2 * atan(sensor / (2 * focalLengs));

}

void Camera::MakePerspectiveL(float fovAngleY, float aspect, float near_, float far_, Matrix4& matrix)
{

	float sinFov = 0.0f;
	float cosFov = 0.0f;
	sinFov = sinf(0.5f * fovAngleY);
	cosFov = cosf(0.5f * fovAngleY);

	float range = far_ / (far_ - near_);
	float height = cosFov / sinFov;

	matrix.m[0][0] = height / aspect;

	matrix.m[1][1] = cosFov / sinFov;

	matrix.m[2][2] = range;
	matrix.m[2][3] = 1.0f;

	matrix.m[3][2] = -range * near_;

	matrix.m[0][1] = matrix.m[0][2] = matrix.m[0][3] =
		matrix.m[1][0] = matrix.m[1][2] = matrix.m[1][3] =
		matrix.m[2][0] = matrix.m[2][1] =
		matrix.m[3][0] = matrix.m[3][1] = matrix.m[3][3] = 0.0f;
}
void Camera::MakeLookL(const Vector3& eye, const Vector3& target, const Vector3& up, Matrix4& mat)
{
	Vector3 zVec = target - eye;
	zVec.nomalize();

	Vector3 xVec = up.cross(zVec);
	xVec.nomalize();

	Vector3 yVec = zVec.cross(xVec);
	yVec.nomalize();

	mat.m[0][0] = xVec.x;
	mat.m[0][1] = xVec.y;
	mat.m[0][2] = xVec.z;
	mat.m[1][0] = yVec.x;
	mat.m[1][1] = yVec.y;
	mat.m[1][2] = yVec.z;
	mat.m[2][0] = zVec.x;
	mat.m[2][1] = zVec.y;
	mat.m[2][2] = zVec.z;
	mat.m[3][0] = eye.x;
	mat.m[3][1] = eye.y;
	mat.m[3][2] = eye.z;
}

Matrix4 Camera::MakeInverse(const Matrix4* mat)
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