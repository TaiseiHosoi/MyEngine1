#pragma once
#include"Camera.h"
#include"Input.h"

class GameCamera :
	public Camera
{
public:
	//�R���X�g���N�^
	GameCamera(int window_width, int window_height, Input* input);
	void Initialize();
	void Update();

private:
	Input* input_ = nullptr;
};

