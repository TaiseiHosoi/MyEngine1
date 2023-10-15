/**
 * @file HitStop.h
 * @brief �q�b�g�X�g�b�v�̊Ǘ��N���X
 */
#pragma once
#include "GameCamera.h"
#include "Vector3.h"
#include <random>

class HitStopManager
{
public:

	// �C���X�^���X�擾
	static HitStopManager* GetInstance();

	// �X�V
	void Update();
	
	/// <summary>
	/// �q�b�g�X�g�b�v���Z�b�g
	/// </summary>
	/// <param name="hitStopFlag"></����t���O>
	/// <param name="timer"></��������>
	void SetHitStop(bool* hitStopFlag , int timer);

	// �J�����Z�b�g
	void SetGameCamera(GameCamera* gameCamera);

	// �J�����Q�b�^
	GameCamera* GetGameCamera();

private:
	// �V�F�C�N�̃A���O���v�Z
	void CulShakeAngle();
	
	// ���쏈��
	void Execute();
	
	// �J�����̃V�F�C�N
	void CameraShake();
	
	// �I��������
	void Finalize();

private:
	HitStopManager() = default;
	HitStopManager(const HitStopManager&) = delete;
	~HitStopManager() = default;
	HitStopManager& operator= (const HitStopManager&) = delete;

private:
	bool* hitStopFlag_ = nullptr;
	int timer_;
	Vector3 shakeAngle_;
	GameCamera* gameCamera_;
};

