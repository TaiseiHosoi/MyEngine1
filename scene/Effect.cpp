#include "Effect.h"
#include "stdlib.h"
#include "Matrix4.h"

void Effect::Initialize(Mesh* model, Vector3 vec3)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	worldTransform_->SetModel(model);
	//textureHandle_ = textureHandle;

	//�V���O���C���X�^���X���擾����
	input_ = Input::GetInstance();

	float kMoveSpeed = 0.2f;	//�X�s�[�h����
	float kRotSpeed = 0.2f;

	//�����l���Z�b�g
	for (int i = 0; i < EFFECT_NUM; i++) {
		worldTransform_[i].Initialize(true);
		worldTransform_[i].position = vec3;
		worldTransform_[i].scale = { 0.7,0.7,0.3 };
		worldTransform_[i].rotation = { 0,0,0 };
		worldTransform_->Update();

		rotVector_[i] = { static_cast<float> (rand() % 20 - 10),
			static_cast<float> (rand() % 20 - 10) ,
			static_cast<float> (rand() % 20 - 10) };	//��]�p
		rotVector_[i].nomalize();

		velocity_[i] = { static_cast<float>(rand() % 20 - 10),
			static_cast<float>(rand() % 20 - 10) ,
			static_cast<float>(rand() % 20 - 10) };	//�U�z�̕���
		velocity_[i].nomalize();

		rotVector_[i] *= kRotSpeed;
		velocity_[i] *= kMoveSpeed;


	}


}

void Effect::Update()
{
	for (int i = 0; i < EFFECT_NUM; i++) {



		worldTransform_[i].position += velocity_[i];	//�ړ�
		worldTransform_[i].rotation += rotVector_[i];	//��]
		float kMinusScale = 0.04f;
		worldTransform_[i].scale += {-kMinusScale, -kMinusScale, -kMinusScale };
		worldTransform_->Update();

		float limitScale = 0.0f;
		if (worldTransform_[i].scale.x <= limitScale &&
			worldTransform_[i].scale.y <= limitScale &&
			worldTransform_[i].scale.z <= limitScale) {
			isDead_ = true;
		}
	}
}

void Effect::Draw()
{
	for (int i = 0; i < EFFECT_NUM; i++) {
		//model_->Draw(worldTransform_[i], viewProjection, textureHandle_);
		worldTransform_->Draw();
	}
}
