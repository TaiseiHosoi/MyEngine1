#include "Bullet.h"
#include <cmath>
#include "math.h"
#define PI 3.141592653589

void Bullet::Initialize(Mesh* model, Vector3 vector3, float kBulSpeed)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	worldTransform_.SetModel(model);
	//textureHandle_ = textureHandle;

	//�V���O���C���X�^���X���擾����
	input_ = Input::GetInstance();

	//�������W���Z�b�g
	worldTransform_.Initialize();
	worldTransform_.position = vector3;
	worldTransform_.scale = { 1,1,1 };
	worldTransform_.rotation = { 0,0,0 };

	//���݂�X�ɂ���ă��[����ύX
	if (worldTransform_.position.x < 0)
	{
		lane_ = Left;
	}
	else if (worldTransform_.position.x == 0)
	{
		lane_ = Center;
	}
	else if (worldTransform_.position.x > 0)
	{
		lane_ = Right;
	}

}

void Bullet::Update()
{

	//�L�[���͂ɉ�����Lane��ύX
	if (input_->PushKey(DIK_LEFT) && input_->TriggerKey(DIK_SPACE))
	{
		if (lane_ == Left)lane_ = Center;
		else if (lane_ == Center)lane_ = Left;
	}
	if (input_->PushKey(DIK_RIGHT) && input_->TriggerKey(DIK_SPACE))
	{
		if (lane_ == Right)lane_ = Center;
		else if (lane_ == Center)lane_ = Right;
	}

	//Lane�ɉ�����X��ύX
	if (lane_ == Left)
	{
		worldTransform_.position.x = -xDifference;
	}
	else if (lane_ == Center)
	{
		worldTransform_.position.x = 0;
	}
	else if (lane_ == Right)
	{
		worldTransform_.position.x = xDifference;
	}
	worldTransform_.Update();

	//�m�[�c�̑��x�̏���
	kBulletSpeedZ += kBulletSpeedAcc;
	Vector3 kBulletSpeed = { 0.0f,0.0f,-easeIn(kBulletSpeedZ) };
	worldTransform_.position += kBulletSpeed;
	worldTransform_.Update();

}

void Bullet::Draw()
{
	//model_->Draw(worldTransform_, viewProjection, textureHandle_);
	worldTransform_.Draw();
}

float Bullet::easeIn(float x)
{
	return 1 - cos((x * PI) / 2);
}

Vector3 Bullet::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ�����
	worldPos.x = worldTransform_.matWorld.m[3][0];
	worldPos.y = worldTransform_.matWorld.m[3][1];
	worldPos.z = worldTransform_.matWorld.m[3][2];

	return worldPos;
}

void Bullet::OnCollision()
{
	isDead_ = true;
}
