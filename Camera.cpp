#include "Camera.h"

using namespace DirectX;

Camera::Camera(int window_width, int window_height)
{
	aspectRatio = (float)window_width / window_height;

	//�r���[�s��̌v�Z
	UpdateViewMatrix();

	// �ˉe�s��̌v�Z
	UpdateProjectionMatrix();

	// �r���[�v���W�F�N�V�����̍���
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
	// ���_���W
	Vector3 eyePosition = eye;
	// �����_���W
	Vector3 targetPosition = target;
	// �i���́j�����
	Vector3 upVector = up;

	// �J����Z���i���������j
	Vector3 cameraAxisZ = targetPosition - eyePosition;
	
	// 0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O

	// �x�N�g���𐳋K��
	cameraAxisZ.nomalize();

	// �J������X���i�E�����j
	Vector3 cameraAxisX;
	// X���͏������Z���̊O�ςŋ��܂�
	cameraAxisX = upVector.cross(cameraAxisZ);
	// �x�N�g���𐳋K��
	cameraAxisX.nomalize();

	// �J������Y���i������j
	Vector3 cameraAxisY;
	// Y����Z����X���̊O�ςŋ��܂�
	cameraAxisY = cameraAxisZ.cross(cameraAxisX);

	// �����܂łŒ�������3�����̃x�N�g��������
	//�i���[���h���W�n�ł̃J�����̉E�����A������A�O�����j	

	// �J������]�s��

	Matrix4 matCameraRot;
	// �J�������W�n�����[���h���W�n�̕ϊ��s��
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
	
	// �]�u�ɂ��t�s��i�t��]�j���v�Z
	matView = MakeInverse(&matCameraRot);

	// ���_���W��-1���|�������W
	Vector3 reverseEyePosition = -eyePosition;
	// �J�����̈ʒu���烏�[���h���_�ւ̃x�N�g���i�J�������W�n�j
	float tX = cameraAxisX.dot(reverseEyePosition);	// X����
	float tY = cameraAxisY.dot(reverseEyePosition);	// Y����
	float tZ = cameraAxisZ.dot(reverseEyePosition);	// Z����
	// ��̃x�N�g���ɂ܂Ƃ߂�
	Vector3 translation = { tX,tY,tZ };
	// �r���[�s��ɕ��s�ړ�������ݒ�
	matView.m[3][0] = translation.x;
	matView.m[3][1] = translation.y;
	matView.m[3][2] = translation.z;
	matView.m[3][3] = 1;

#pragma region �S�����r���{�[�h�s��̌v�Z
	// �r���{�[�h�s��
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

#pragma region Y�����r���{�[�h�s��̌v�Z
	// �J����X���AY���AZ��
	Vector3 ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	// X���͋���
	ybillCameraAxisX = cameraAxisX;
	// Y���̓��[���h���W�n��Y��
	ybillCameraAxisY = upVector.nomalize();
	// Z����X����Y���̊O�ςŋ��܂�
	ybillCameraAxisZ = ybillCameraAxisX.cross(ybillCameraAxisY);

	// Y�����r���{�[�h�s��
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
	// �������e�ɂ��ˉe�s��̐���
	 MakePerspectiveL(
		FieldOfViewY(),
		aspectRatio,
		0.1f, 100000.0f, 
		 matProjection
	);
}

void Camera::MoveEyeVector(const Vector3& move)
{
	// ���_���W���ړ����A���f
	Vector3 eye_moved = GetEye();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	SetEye(eye_moved);
}



void Camera::MoveVector(const Vector3& move)
{
	// ���_�ƒ����_���W���ړ����A���f
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

	//�|���o���@���s���s��
	float sweep[4][8]{};
	//�萔�{�p
	float constTimes = 0.0f;
	//���e����덷
	float MAX_ERR = 1e-10f;
	//�߂�l�p
	Matrix4 retMat;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//weep�̍����ɋt�s������߂�s����Z�b�g
			sweep[i][j] = mat->m[i][j];

			//sweep�̉E���ɒP�ʍs����Z�b�g
			sweep[i][4 + j] = Matrix4::MakeIdentity().m[i][j];
		}
	}

	//�S�Ă̗�̑Ίp�����ɑ΂���J��Ԃ�
	for (int i = 0; i < 4; i++)
	{
		//�ő�̐�Βl�𒍖ڑΊp�����̐�Βl�Ɖ���
		float max = fabs(sweep[i][i]);
		int maxIndex = i;

		//i��ڂ��ő�̐�Βl�ƂȂ�s��T��
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
			//�t�s��͋��߂��Ȃ�
			return Matrix4::MakeIdentity();
		}

		//����(1):i�s�ڂ�maxIndex�s�ڂ����ւ���
		if (i != maxIndex)
		{
			for (int j = 0; j < 8; j++)
			{
				float tmp = sweep[maxIndex][j];
				sweep[maxIndex][j] = sweep[i][j];
				sweep[i][j] = tmp;
			}
		}

		//sweep[i][i]�Ɋ|�����1�ɂȂ�l�����߂�
		constTimes = 1 / sweep[i][i];

		//����(2):p�s�ڂ�a�{����
		for (int j = 0; j < 8; j++)
		{
			//����ɂ��sweep[i][i]��1�ɂȂ�
			sweep[i][j] *= constTimes;
		}

		//����(3)�ɂ��i�s�ڈȊO�̍s��i��ڂ�0�ɂ���
		for (int j = 0; j < 4; j++)
		{
			if (j == i)
			{
				//i�s�ڂ͂��̂܂�
				continue;
			}

			//i�s�ڂɊ|����l�����߂�
			constTimes = -sweep[j][i];

			for (int k = 0; k < 8; k++)
			{
				//j�s�ڂ�i�s�ڂ�a�{�����s�𑫂�
				//����ɂ��sweep[j][i]��0�ɂȂ�
				sweep[j][k] += sweep[i][k] * constTimes;
			}
		}
	}

	//sweep�̉E������mat�̋t�s��
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			retMat.m[i][j] = sweep[i][4 + j];
		}
	}

	return retMat;
}