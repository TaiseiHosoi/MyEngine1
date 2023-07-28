#include "HitStopManager.h"

HitStopManager* HitStopManager::GetInstance()
{
	static HitStopManager instance;
	return &instance;
}

void HitStopManager::Update()
{
	if (hitStopFlag_)
	{
		Execute();
	}
}

void HitStopManager::SetHitStop(bool* hitStopFlag , int timer)
{
	srand(static_cast<int>(time(nullptr)));

	hitStopFlag_ = hitStopFlag;
	*hitStopFlag_ = true;

	timer_ = timer;

	CulShakeAngle();
}

void HitStopManager::SetGameCamera(GameCamera* gameCamera)
{
	gameCamera_ = gameCamera;
}

GameCamera* HitStopManager::GetGameCamera()
{
	return gameCamera_;
}

void HitStopManager::CulShakeAngle()
{
	shakeAngle_.x = static_cast<float>(rand() % 21 - 10);
	shakeAngle_.y = static_cast<float>(rand() % 21 - 10);
	shakeAngle_.z = static_cast<float>(rand() % 21 - 10);

	shakeAngle_.nomalize();
}

void HitStopManager::Execute()
{

	CameraShake();
	timer_--;

	if (timer_ <= 0)
	{
		Finalize();
	}

}

void HitStopManager::CameraShake()
{
	gameCamera_->SetShakeVec(gameCamera_->GetTarget() + (shakeAngle_ * 3));
	gameCamera_->Update();
}

void HitStopManager::Finalize()
{
	timer_ = 0;
	*hitStopFlag_ = false;
	hitStopFlag_ = nullptr;
	shakeAngle_ = {0 , 0 , 0};
	gameCamera_->SetShakeVec({0 , 0 , 0});
}