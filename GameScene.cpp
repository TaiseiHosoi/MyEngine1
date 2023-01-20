#include "GameScene.h"

#include <Bullet.h>


GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize(DirectXCommon* dxcomon)
{
	assert(dxcomon);
	dxCommon_ = dxcomon;

	//�X�v���C�g������
	spriteCommon_ = new SpriteCommon;
	spriteCommon_->Initialize(dxCommon_);
	spriteCommon_->LoadTexture(1, "gamen.png");
	spriteCommon_->LoadTexture(2, "title.png");


	sprite1 = new Sprite();
	sprite1->Initialize(spriteCommon_, 1);
	sprite2 = new Sprite();
	sprite2->Initialize(spriteCommon_, 2);


	sprite1->SetSize({ 1280,720 });
	sprite2->SetSize({ 1280,720 });
	sprite1->SetPozition({ 0,0 });
	sprite2->SetPozition({ 0,0 });
	audio = new Audio();
	audio->Initialize();


	model = Mesh::LoadFromOBJ("lowpoliHitokunIdle");
	object3d = Object3d::Create();

	object3d->position.z = 90.0f;


	//game

	//���f��
	bulletModel_ = Mesh::LoadFromOBJ("bume");
	model_ = Mesh::LoadFromOBJ("cube");
	ico_ = Mesh::LoadFromOBJ("ico");
	object3d->SetModel(ico_);
	//���[��
	field_[0].Initialize(ico_, Left);
	field_[1].Initialize(ico_, Center);
	field_[2].Initialize(ico_, Right);

	//���@
	goal_.Initialize(model);

	BulletReset();
}

void GameScene::Update()
{
	if (input_->TriggerKey(DIK_SPACE)) {
		if (scene == 0) {
			scene = 1;
			for (std::unique_ptr<Bullet>& bullet_ : bullets_) {
				bullet_->OnCollision();
			}
			BulletReset();
			gameLevel_ = 0;
			gameTimer_ = 0;
			field_[1].Initialize(ico_, Center);
		}
		else {
			scene = 0;
		}
	}


	switch (scene) {
	case 0: //�^�C�g��


		break;
	case 1://�V�[��
		gameTimer_++;
		if (gameTimer_ > 200) {
			if (gameLevel_ < levelMax_) {
				gameTimer_ = 0;
				gameLevel_++;
			}
			else {
				gameTimer_ = 0;
			}

		}


		for (int i = 0; i < 3; i++) {
			field_[i].Update();
		}

		//�f���[�g
		//�f�X�t���O�̗������e�폜
		bullets_.remove_if([](std::unique_ptr<Bullet>& bullet_) { return bullet_->IsDead(); });

		//�Ȃ񂩓G
		Vector3 object3dPos = object3d->GetPosition();
		if (input_->PushKey(DIK_W)) {
			object3dPos.z += 0.1f;
		}
		object3d->SetPosition(object3dPos);
		object3d->Update();



		for (std::unique_ptr<Bullet>& bullet_ : bullets_) {

			bullet_->Update();


		}

		//�e����
		UpdateBulletPopCommands();

		//���@
		goal_.Update(field_[1].GetTransration());

		CheckAllCollisions();
		break;	//�V�[���p
	}


}

void GameScene::Draw()
{

	switch (scene) {
	case 0: //�^�C�g��
		spriteCommon_->SpritePreDraw();
		sprite2->Draw();
		spriteCommon_->SpritePostDraw();

		Object3d::PreDraw(dxCommon_->GetCommandList());
		Object3d::PostDraw();
		break;
	case 1://�V�[��
		spriteCommon_->SpritePreDraw();

		sprite1->Draw();
		//sprite2->Draw();

		spriteCommon_->SpritePostDraw();

		Object3d::PreDraw(dxCommon_->GetCommandList());

		/*for (int i = 0; i < 3; i++) {
			field_[i].Draw();
		}*/

		object3d->Draw();

		for (std::unique_ptr<Bullet>& bullet_ : bullets_) {
			bullet_->Draw();
		}

		goal_.Draw();


		Object3d::PostDraw();
		break;	//�V�[���p
	}

}


void GameScene::AddBullet(std::unique_ptr<Bullet>& Bullet)
{
	bullets_.push_back(std::move(Bullet));
}

void GameScene::GenerBullet(Vector3 BulletPos, int ID)
{
	//����
	std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
	//�G�L�����̏�����
	float kBulSpeed = 0.8f;
	if (gameLevel_ > 0) {
		kBulSpeed += gameLevel_ * 0.1f + 1.0f;
	}


	newBullet->Initialize(model_, BulletPos, kBulSpeed);

	newBullet->worldTransform_.SetModel(bulletModel_);

	newBullet->SetID(ID);

	//���X�g�ɓo�^����
	bullets_.push_back(std::move(newBullet));

}

void GameScene::LoadBulletPopData()
{

	//�t�@�C�����J��
	std::ifstream file;
	file.open("Resources/enemyPop2.csv");

	assert(file.is_open());

	//�t�@�C���̓��e�𕶎���X�g���[���ɃR�s�[
	bulletPopCommands_ << file.rdbuf();

	//�t�@�C�������
	file.close();
}

void GameScene::UpdateBulletPopCommands()
{


	//�ҋ@����
	if (isStand_) {
		standTime_--;
		if (standTime_ <= 0) {
			//�ҋ@����
			isStand_ = false;
		}
		return;
	}
	// 1�s���̕����������ϐ�
	std::string line;

	//�R�}���h���s���[�v
	while (getline(bulletPopCommands_, line)) {
		// 1�s���̕��������X�g���[���ɕϊ����ĉ�܂��₷���Ȃ�
		std::istringstream line_stream(line);

		std::string word;
		//,��؂�ōs�̐擪�������擾
		getline(line_stream, word, ',');

		//"//"����n�܂�s�̓R�����g
		if (word.find("//") == 0) {
			//�R�����g�s���΂�
			continue;
		}
		// POP�R�}���h
		if (word.find("POP") == 0) {

			//���[��
			std::getline(line_stream, word, ',');
			int lane = static_cast<int>(std::atof(word.c_str()));

			// ID
			std::getline(line_stream, word, ',');
			int ID = static_cast<int>(std::atof(word.c_str()));

			float depth = 120.0f;	//���s
			float xDifference = 10.0f;	//���E��
			if (lane == 1) {
				GenerBullet(Vector3(-xDifference, 0, depth), ID);
			}
			else if (lane == 2) {
				GenerBullet(Vector3(0, 0, depth), ID);
			}
			else if (lane == 3) {
				GenerBullet(Vector3(xDifference, 0, depth), ID);
			}
			else {
				GenerBullet(Vector3(0, 3.0f, depth), ID);
			}
		}
		// WAIT�R�}���h
		else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');

			//�҂�����
			int32_t waitTime = std::atoi(word.c_str());

			//�ҋ@�J�n
			isStand_ = true;
			int maxTimeDiv = 10;
			if (gameLevel_ <= 0) {
				standTime_ = waitTime * (maxTimeDiv - gameLevel_) / maxTimeDiv;
			}
			else {

				standTime_ = waitTime * (maxTimeDiv - gameLevel_) / maxTimeDiv;
			}

			//������
			break;
		}
	}
}

void GameScene::BulletReset()
{
	bulletPopCommands_.str("");
	bulletPopCommands_.clear(std::stringstream::goodbit);
	LoadBulletPopData();
}


void GameScene::CheckAllCollisions() {

	//����Ώ�A��B�̍��W
	Vector3 posA, posB;



#pragma region ���e�ƓG�L�����̓����蔻��
	//�G�L�����̍��W
	for (std::unique_ptr<Bullet>& bullet_ : bullets_) {
		posA = bullet_->GetWorldPosition();


		//���e�̍��W
		posB = goal_.GetWorldPosition();

		float x = posB.x - posA.x;
		float y = posB.y - posA.y;
		float z = posB.z - posA.z;

		float cd = sqrt(x * x + y * y + z * z);

		if (cd <= 4.0f) {
			//�G�L�����̏Փˎ��R�[���o�b�N���Ăяo��
			bullet_->OnCollision();

			//�Փˎ��R�[���o�b�N���Ăяo��
			//goal_->OnCollision();
		}

	}
}
