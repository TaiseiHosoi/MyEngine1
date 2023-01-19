#include "GameScene.h"


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
	spriteCommon_->LoadTexture(1, "mario.png");
	spriteCommon_->LoadTexture(2, "reimu.png");


	sprite1 = new Sprite();
	sprite1->Initialize(spriteCommon_, 1);
	sprite2 = new Sprite();
	sprite2->Initialize(spriteCommon_, 2);


	sprite1->SetSize({ 1280,720 });
	sprite2->SetSize({ 200,200 });
	sprite1->SetPozition({ 100,100 });
	sprite2->SetPozition({ 100,200 });
	audio = new Audio();
	audio->Initialize();


	model = Mesh::LoadFromOBJ("cube");
	object3d = Object3d::Create();
	object3d->SetModel(model);

}

void GameScene::Update()
{
	object3d->Update();
}

void GameScene::Draw()
{
	spriteCommon_->SpritePreDraw();

	sprite1->Draw();
	sprite2->Draw();

	spriteCommon_->SpritePostDraw();

	Object3d::PreDraw(dxCommon_->GetCommandList());

	object3d->Draw();

	Object3d::PostDraw();

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
	float kBulSpeed = 0.4f;
	if (gameLevel_ > 0) {
		kBulSpeed += gameLevel_ * 0.1f + 1.0f;
	}


	newBullet->Initialize(model, testTexture_, BulletPos, kBulSpeed);

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

			float depth = 40.0f;	//���s
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
		posB = goal_->GetWorldPosition();

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
