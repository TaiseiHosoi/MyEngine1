#include "GameCamera.h"
#include "Ease.h"

#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include"WinApp.h"
#include <WinUser.h>

float GameCamera::magnitude = 0.0f;
float GameCamera::duration = 0.0f;
bool GameCamera::isShake = 0.0f;

GameCamera::GameCamera(int window_width , int window_height , Input* input)
	: Camera(window_width , window_height)
{
	//���̓C���X�^���X�擾
	input_ = Input::GetInstance();

	//�J�����̏�����
	Vector3 eye = {0.0f , 0.0f , -5.0f};
	Vector3 up = {0 , 1 , 0};
	Vector3 target = {0 , 0 , 300.0f};
	this->SetEye(eye);
	this->SetUp(up);
	this->SetTarget(target);

	isPause_ = false;
}

void GameCamera::Initialize()
{

}

void GameCamera::Update()
{
	//if (GetActiveWindow() == WinApp::GetInstance()->GetHwnd())
	//{
	//	ShowCursor(false);

	//	//ViewPointMovement();

	//	CulDirection();
	//}

	

	FollowPlayer();

	Camera::Update();
}

void GameCamera::SetTargetPos(WorldTransform* targetPos)
{
	targetPos_ = targetPos;
}

WorldTransform* GameCamera::GetTargetPos()
{
	return targetPos_;
}

void GameCamera::RemoveTargetPos()
{
	targetPos_ = nullptr;
}

void GameCamera::SetEyePos(WorldTransform* eyePos)
{
	eyePos_ = eyePos;
}

void GameCamera::SetFollowerPos(WorldTransform* wtf)
{
	followerPos_ = wtf;
}

WorldTransform* GameCamera::GetEyePos()
{
	return eyePos_;
}

void GameCamera::RemoveEyePos()
{
	eyePos_ = nullptr;
}

void GameCamera::ViewPointMovement()
{
	//�J�����̉�]�x�N�g��
	Vector3 rotat = {0 , 0 , 0};
	//�J�����̈ړ��̑���
	const float cameraSpeed = 0.0005f;

	Vector2 windowWH = Vector2(WinApp::window_width / 2 , WinApp::window_height / 2);
	POINT mousePosition;
	//�}�E�X���W(�X�N���[�����W)���擾����
	GetCursorPos(&mousePosition);

	//�N���C�A���g�G���A���W�ɕϊ�����
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd , &mousePosition);

	int xPos_absolute , yPos_absolute;

	int xPos = windowWH.x;  //�ړ��������������W�i�E�B���h�E���̑��΍��W�j
	int yPos = windowWH.y; //�ړ��������������W�i�E�B���h�E���̑��΍��W�j

	WINDOWINFO windowInfo;
	//�E�B���h�E�̈ʒu���擾
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd , &windowInfo);

	//�}�E�X�̈ړ���̐�΍��W�i���j�^�[���ォ��̍��W�j
	xPos_absolute = xPos + windowInfo.rcWindow.left + 8;//�Ȃ񂩂���Ă邩�璼��
	yPos_absolute = yPos + windowInfo.rcWindow.top + 31; //�E�B���h�E�̃^�C�g���o�[�̕��i31px�j���v���X

	SetCursorPos(xPos_absolute , yPos_absolute);//�ړ�������

	//�}�E�X�̈ړ��ʂ��擾
	mouseMove = Vector2(0 , 0);
	mouseMove = (Vector2(mousePosition.y , mousePosition.x) - Vector2(windowWH.y , windowWH.x));//���W���ŉ�]���Ă���֌W�ł����Ȃ�(X��Y������ւ�)

	if (isPause_ == false) {

		//�}�E�X�̈ړ��ʂ��J�����̉�]�𔽉f
		rotation_.y -= MathFunc::Dig2Rad(mouseMove.y * mouseSensitivity_);
		if (2 * MathFunc::PI <= rotation_.y)
		{
			rotation_.y -= 2 * MathFunc::PI;
		}
		else if (rotation_.y < 0)
		{
			rotation_.y += 2 * MathFunc::PI;
		}

		rotation_.x -= MathFunc::Dig2Rad(mouseMove.x * mouseSensitivity_);
		if (MathFunc::Dig2Rad(60) <= rotation_.x)
		{
			rotation_.x = MathFunc::Dig2Rad(60);
		}
		else if (rotation_.x < -MathFunc::Dig2Rad(15))
		{
			rotation_.x = -MathFunc::Dig2Rad(15);
		}
	}

}



float GameCamera::randomFloat(float min , float max)
{
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

void GameCamera::ShakePrim()
{
	if (isShake == true)
	{

		if (elapsedTime < duration)
		{
			float offsetX = randomFloat(-1.0f , 1.0f) * magnitude;
			float offsetY = randomFloat(-1.0f , 1.0f) * magnitude;
			float offsetZ = randomFloat(-1.0f , 1.0f) * magnitude;

			loolAtPos = {offsetX , offsetY , offsetZ};

			elapsedTime += deltaTime;
		}
		else
		{
			elapsedTime = 0.0f;

			isShake = false;
		}

	}

}

void GameCamera::SetShakePrimST(float dura , float mag , bool isShakePrim)
{
	duration = dura;
	magnitude = mag;
	isShake = isShakePrim;
}

void GameCamera::SetShakeVec(Vector3 shakeVec)
{
	shakeVec_ = shakeVec;
}

void GameCamera::CulDirection()
{
	//LShift�������ƓG���^�[�Q�b�g����
	if (input_->GetInstance()->TriggerKey(DIK_LSHIFT))
	{

		dir_ = targetPos_->translation_ - followerPos_->translation_;
		dir_.nomalize();

		

		Vector2 vec1 = {dir_.x , dir_.z};
		Vector2 vec2 = {0 , 1};

		vec1.normalize();

		float dot = vec1.dot(vec2);

		rotation_.y = acosf(dot);

		//rotation_.y += MathFunc::Dig2Rad(90);

		if (0 < dir_.x)
		{
			rotation_.y = -rotation_.y;
			rotation_.y += MathFunc::Dig2Rad(180);
		}
	}

	//�J�����̉�]�ʂ���������v�Z
	dir_ = {cos(rotation_.y) , sin(rotation_.x) , sin(rotation_.y)};
	dir_.nomalize();

}



void GameCamera::FollowPlayer()
{
	

	
		Vector3 basePos = {followerPos_->translation_.x , followerPos_->translation_.y , followerPos_->translation_.z};

		Vector3 tempEye = basePos - dir_ * MAX_CAMERA_DISTANCE;
		

		SetEye(tempEye);
		
		SetTarget({ tempEye.x,tempEye.y,tempEye.z + 1.f });

	
	
}

void GameCamera::ChangeFollowFlag(bool flag)
{
	isFollowPlayer_ = flag;
}