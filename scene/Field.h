#pragma once
#include "Vector3.h"
#include "Matrix4.h"
#include "Object3d.h"
#include "memory"
#include "Mesh.h"
#include "Input.h"
#include "Ease.h"

//���[���̗񋓌^
	enum Lane
	{
		Left,
		Center,
		Right
	};

class Field
{
public:
	void Initialize(Mesh* model, Lane lane);	//���[����X���W�����߂ď�����
	void Draw();
	void Update();
	//�Q�b�^�[
	Vector3 GetTransration() { return worldTransform_.GetPosition(); };
	int GetLane() { return lane_; };

	void LaneChange();
private:

	Input* input_ = nullptr;
	Object3d worldTransform_;
	Mesh* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	float length = 200.0f;	//���[���̒���
	float zLen_ = 165.0f;	//okuyuki
	//���݂̃��[��
	Lane lane_;

	//���[���̕�
	float laneWidth = 10.0f;
	//�C�[�W���O�p
	Ease* ease_ = nullptr;
	bool isChangeRight_ = false;		//���[���`�F���W
	bool isChangeLeft_ = false;
	const int maxTime_ = 10;
	int time_ = 0;
	int changeMode_ = 0; //1����2���E


};

