#pragma once
#include"Camera.h"
#include"Input.h"
#include "Object3d.h"

class GameCamera :
	public Camera
{
public:
	//�R���X�g���N�^
	GameCamera(int window_width , int window_height , Input* input);
	void Initialize();
	void Update();
	void SetTargetPos(WorldTransform* targetPos);
	WorldTransform* GetTargetPos();
	void RemoveTargetPos();
	void SetEyePos(WorldTransform* eyePos);
	void SetFollowerPos(WorldTransform* wtf);
	WorldTransform* GetEyePos();
	void RemoveEyePos();
	void ViewPointMovement();


	//�V�F�C�N�p
	float randomFloat(float min , float max);	//�����_���������o��
	void ShakePrim();
	static void SetShakePrimST(float dura , float mag , bool isShakePrim);

	void SetShakeVec(Vector3 shakeVec);

	//�J�����̌������v�Z����֐�
	void CulDirection();

	//�v���C���[��Ǐ]����
	void FollowPlayer();

	// �V�[���؂�ւ��p
	void ChangeFollowFlag(bool flag);

	void SetIsPause(bool isPause)
	{
		isPause_ = isPause;
	}

	// 1-> target ,0-> eye
	WorldTransform swap_[2];
private:
	Input* input_ = nullptr;
	WorldTransform* targetPos_ = nullptr;
	WorldTransform* eyePos_ = nullptr;
	WorldTransform* followerPos_ = nullptr;
	bool isFollowPlayer_ = true;
	bool isTargetenemy_ = false;

	//�J���������֌W
	const float  MAX_CAMERA_DISTANCE = 50.0f;
	const float  MIN_CAMERA_DISTANCE = 7.0f;
	float cameraDistance_ = 20.0f;
	const float MAX_CHANGE_TIMER = 30;
	int cameraModeChangeCountTimer = 30;
	float cameraHeight_ = 6;
	//ShakePrim�p�ϐ�
	static float magnitude;	//�V�F�C�N�̋���
	static float duration;//�V�F�C�N���N��������
	static bool isShake;
	float elapsedTime = 0.0f;
	const float deltaTime = 0.016f;	//1.0f��1�b(60FPS)�ƍl�����ۂ�1F
	Vector3 loolAtPos = {};
	Vector3 shakeVec_ = {0 , 0 , 0};

	//�}�E�X�J�����p
	Vector3 rotation_ = {0 , 0 , 0};
	Vector3 dir_ = {0 , 0 , 1};
	Vector2 mouseMove;
	float mouseSensitivity_ = 0.05;	//�}�E�X���x
	bool isPause_;
};

