#include "GameCamera.h"



GameCamera::GameCamera(int window_width, int window_height, Input* input)
	: Camera(window_width, window_height)
{
	//入力インスタンス取得
	input_->GetInstance();

	//カメラの初期化
	Vector3 eye = { 0.0f, 0.0f, -5.0f };
	Vector3 up = { 0, 1, 0 };
	Vector3 target = { 0, 0, 300.0f };
	this->SetEye(eye);
	this->SetUp(up);
	this->SetTarget(target);
}

void GameCamera::Initialize()
{
	
}

void GameCamera::Update()
{
	Camera::Update();
}
