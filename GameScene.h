#pragma once
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Audio.h"
#include "Mesh.h"

#include "sstream"
#include "memory"
#include "list"

#include"Goal.h"
#include"Field.h"
#include"Enemy.h"
#include"Ease.h"


class GameScene {
public:
	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	GameScene();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GameScene();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(DirectXCommon* dxcomon);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//�e���X�g���擾
	std::list<std::unique_ptr<Bullet>> bullets_;
	const std::list<std::unique_ptr<Bullet>>& GetBullets() { return bullets_; }

	//�e�𑫂�
	void AddBullet(std::unique_ptr<Bullet>& Bullet);
	void GenerBullet(Vector3 BulletPos, int ID);

	/// <summary>
	/// �G�����f�[�^�ǂݍ���
	/// </summary>
	void LoadBulletPopData();
	void BulletReset();
	void UpdateBulletPopCommands();

	//�����蔻��
	void CheckAllCollisions();

	int gameLevel_ = 0;
	int levelMax_ = 8;
	//���[��
	Field field_[3];

	//�S�[���p
	Goal* goal_ = nullptr;

private:
	DirectXCommon* dxCommon_ = nullptr;
	
	Audio* audio = nullptr;
	SpriteCommon* spriteCommon_ = nullptr;
	Object3d* object3d = nullptr;
	Mesh* model = nullptr;

private://���\�[�X
	Sprite* sprite1 = nullptr;
	Sprite* sprite2 = nullptr;

	//csv
	std::stringstream bulletPopCommands_;

	//�e�N�X�`��
	uint32_t testTexture_ = 0;

	//���[���f�o�b�O�p�̃e�N�X�`��
	uint32_t laneTexture_[3];

	//�ҋ@���t���O
	bool isStand_ = false;
	//�ҋ@�^�C�}�[
	int standTime_ = 0;

	int gameTimer_ = 0;

	
};