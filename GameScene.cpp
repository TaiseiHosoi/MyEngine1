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

	//スプライト初期化
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

	//モデル
	bulletModel_ = Mesh::LoadFromOBJ("bume");
	model_ = Mesh::LoadFromOBJ("cube");
	ico_ = Mesh::LoadFromOBJ("ico");
	object3d->SetModel(ico_);
	//レーン
	field_[0].Initialize(ico_, Left);
	field_[1].Initialize(ico_, Center);
	field_[2].Initialize(ico_, Right);

	//自機
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
	case 0: //タイトル


		break;
	case 1://シーン
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

		//デリート
		//デスフラグの立った弾削除
		bullets_.remove_if([](std::unique_ptr<Bullet>& bullet_) { return bullet_->IsDead(); });

		//なんか敵
		Vector3 object3dPos = object3d->GetPosition();
		if (input_->PushKey(DIK_W)) {
			object3dPos.z += 0.1f;
		}
		object3d->SetPosition(object3dPos);
		object3d->Update();



		for (std::unique_ptr<Bullet>& bullet_ : bullets_) {

			bullet_->Update();


		}

		//弾発生
		UpdateBulletPopCommands();

		//自機
		goal_.Update(field_[1].GetTransration());

		CheckAllCollisions();
		break;	//シーン用
	}


}

void GameScene::Draw()
{

	switch (scene) {
	case 0: //タイトル
		spriteCommon_->SpritePreDraw();
		sprite2->Draw();
		spriteCommon_->SpritePostDraw();

		Object3d::PreDraw(dxCommon_->GetCommandList());
		Object3d::PostDraw();
		break;
	case 1://シーン
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
		break;	//シーン用
	}

}


void GameScene::AddBullet(std::unique_ptr<Bullet>& Bullet)
{
	bullets_.push_back(std::move(Bullet));
}

void GameScene::GenerBullet(Vector3 BulletPos, int ID)
{
	//生成
	std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
	//敵キャラの初期化
	float kBulSpeed = 0.8f;
	if (gameLevel_ > 0) {
		kBulSpeed += gameLevel_ * 0.1f + 1.0f;
	}


	newBullet->Initialize(model_, BulletPos, kBulSpeed);

	newBullet->worldTransform_.SetModel(bulletModel_);

	newBullet->SetID(ID);

	//リストに登録する
	bullets_.push_back(std::move(newBullet));

}

void GameScene::LoadBulletPopData()
{

	//ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop2.csv");

	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	bulletPopCommands_ << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GameScene::UpdateBulletPopCommands()
{


	//待機処理
	if (isStand_) {
		standTime_--;
		if (standTime_ <= 0) {
			//待機完了
			isStand_ = false;
		}
		return;
	}
	// 1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(bulletPopCommands_, line)) {
		// 1行分の文字数をストリームに変換して解折しやすくなる
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}
		// POPコマンド
		if (word.find("POP") == 0) {

			//レーン
			std::getline(line_stream, word, ',');
			int lane = static_cast<int>(std::atof(word.c_str()));

			// ID
			std::getline(line_stream, word, ',');
			int ID = static_cast<int>(std::atof(word.c_str()));

			float depth = 120.0f;	//奥行
			float xDifference = 10.0f;	//左右差
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
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = std::atoi(word.c_str());

			//待機開始
			isStand_ = true;
			int maxTimeDiv = 10;
			if (gameLevel_ <= 0) {
				standTime_ = waitTime * (maxTimeDiv - gameLevel_) / maxTimeDiv;
			}
			else {

				standTime_ = waitTime * (maxTimeDiv - gameLevel_) / maxTimeDiv;
			}

			//抜ける
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

	//判定対象AとBの座標
	Vector3 posA, posB;



#pragma region 自弾と敵キャラの当たり判定
	//敵キャラの座標
	for (std::unique_ptr<Bullet>& bullet_ : bullets_) {
		posA = bullet_->GetWorldPosition();


		//自弾の座標
		posB = goal_.GetWorldPosition();

		float x = posB.x - posA.x;
		float y = posB.y - posA.y;
		float z = posB.z - posA.z;

		float cd = sqrt(x * x + y * y + z * z);

		if (cd <= 4.0f) {
			//敵キャラの衝突時コールバックを呼び出す
			bullet_->OnCollision();

			//衝突時コールバックを呼び出す
			//goal_->OnCollision();
		}

	}
}
