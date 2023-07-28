#include "Boss.h"
#include "FbxPlayer.h"

#include"PlayerActionManager.h"

#include "Ground.h"


int Boss::hp = 100;	//hp実態
bool Boss::isAtkCollide = false;

void Boss::Initialize(DirectXCommon* dxcomon)
{
	dxcomon_ = dxcomon;
	input_ = Input::GetInstance();
	audio_ = std::make_unique<Audio>();
	audio_->Initialize();
	//音の読み込み
	audio_->LoadWave("kuri.wav");


	FBXObject3d::SetDevice(dxcomon_->GetDevice());
	// グラフィックスパイプライン生成
	FBXObject3d::CreateGraphicsPipeline();

	//ボスのFBX読み込み
	bossFbxM_.reset(FbxLoader::GetInstance()->LoadModelFromFile("boss_prot4", true));
	bossFbxO_ = std::make_unique<FBXObject3d>();
	bossFbxO_->Initialize();
	bossFbxO_->SetModel(bossFbxM_.get());

	bossFbxO_->PlayAnimation(0);
	bossFbxO_->SetPosition({ 0,0,30 });
	bossFbxO_->SetScale({ 3,3,3 });
	bossFbxO_->SetIsBonesWorldMatCalc(true);	// ボーンワールド行列計算あり
	bossFbxO_->Update();

	bossModel_ = Mesh::LoadFormOBJ("enemy", true);
	bossObj_ = Object3d::Create();

	bossObj_->SetModel(bossModel_.get());
	bossObj_->worldTransform.translation_ = { 0,0,30 };
	bossFbxO_->AnimPlay();

	//testobj
	bossObj_ = Object3d::Create();
	ico_ = Mesh::LoadFormOBJ("ico", true);
	//testObj_->SetModel(ico_.get());

	bulletO_ = Object3d::Create();
	bulletM_ = Mesh::LoadFormOBJ("cube", true);
	bulletO_->SetModel(bulletM_.get());

	//当たり判定用
	SPHERE_COLISSION_NUM = static_cast<int>(bossFbxO_->GetBonesMatPtr()->size());
	sphere.resize(SPHERE_COLISSION_NUM);
	spherePos.resize(SPHERE_COLISSION_NUM);
	bossFbxO_.get()->isBonesWorldMatCalc = true;	// ボーンの行列を取得するか
	coliderPosTest_.resize(SPHERE_COLISSION_NUM);
	hpModel_ = Mesh::LoadFormOBJ("cube", false);



	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		sphere[i] = new SphereCollider;
		CollisionManager::GetInstance()->AddCollider(sphere[i]);
		spherePos[i] = bossFbxO_.get()->bonesMat[i].GetWorldPos();
		sphere[i]->SetBasisPos(&spherePos[i]);
		sphere[i]->SetRadius(8.0f);
		sphere[i]->Update();
		sphere[i]->SetAttribute(COLLISION_ATTR_ENEMIES);
		//test
		coliderPosTest_[i] = Object3d::Create();
		coliderPosTest_[i]->SetModel(hpModel_.get());
		coliderPosTest_[i]->SetPosition(sphere[i]->center);
		coliderPosTest_[i]->SetScale({ sphere[i]->GetRadius(),sphere[i]->GetRadius() ,sphere[i]->GetRadius() });
		coliderPosTest_[i]->SetRotate({ 0,0,0 });
		coliderPosTest_[i]->Update();
	}

	hp = 100;
	//hpModel_ = Mesh::LoadFormOBJ("cube", false);
	hpObject_ = Object3d::Create();
	hpObject_->SetModel(hpModel_.get());
	hpObject_->SetScale({ static_cast<float>(hp) * 0.2f,1.3f,0.02f });
	hpObject_->SetPosition({ bossFbxO_->GetWorldTransform().translation_.x,
		 bossFbxO_->GetWorldTransform().translation_.y + 12.0f,
		 bossFbxO_->GetWorldTransform().translation_.z });
	Matrix4 invViewPro = MathFunc::MakeInverse(&hpObject_.get()->camera_->GetViewProjectionMatrix());
	float yaw = atan2f(-invViewPro.m[2][0], sqrtf(invViewPro.m[2][1] * invViewPro.m[2][1] + invViewPro.m[2][2] * invViewPro.m[2][2]));
	hpObject_->SetRotate({ 0,bossFbxO_->GetWorldTransform().rotation_.y,0 });
	hpObject_->Update();

	//攻撃判定初期化

}

void Boss::Update()
{
	srand(static_cast<int>(time(nullptr)));

	BulletUpdate();
	switch (mode)
	{
		//地上戦パート時

	case BattleMode::Ground:

		bossFbxO_->PlayAnimation(0);
		Attack();
		break;
	}

	bossFbxO_->Update();

	hpObject_->SetScale({ static_cast<float>(hp) * 0.2f,1.3f,0.02f });
	hpObject_->SetPosition({ bossFbxO_->GetWorldTransform().translation_.x,
		 bossFbxO_->GetWorldTransform().translation_.y + 25.0f,
		 bossFbxO_->GetWorldTransform().translation_.z });
	Matrix4 invViewPro = MathFunc::MakeInverse(&hpObject_.get()->camera_->GetViewProjectionMatrix());
	float yaw = atan2f(-invViewPro.m[2][0], sqrtf(invViewPro.m[2][1] * invViewPro.m[2][1] + invViewPro.m[2][2] * invViewPro.m[2][2]));
	hpObject_->SetRotate({ 0,bossFbxO_->GetWorldTransform().rotation_.y,0 });
	hpObject_->Update();

	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		spherePos[i] = bossFbxO_.get()->bonesMat[i].GetWorldPos();
		sphere[i]->Update();
		/*if (sphere[i]->GetIsHit() == true && player_->GetIsAtkCollide() == true){


		 }*/

	}

}

void Boss::Draw()
{
	bossFbxO_->Draw(dxcomon_->GetCommandList());
	BulletDraw();
}

void Boss::Attack()
{
	oldValue = attackValue;

	isGround = Field::EnemyOnGround(bossFbxO_->GetPosition(), 10);

	oldPos = bossFbxO_->GetPosition();

	if (isGround == true)
	{
		bossFbxO_->SetPosition(oldPos);
	}

	returnPos = { 0,0,30 };
	returnDis = returnPos - bossFbxO_->GetPosition();

	//攻撃していないとき
	if (isAttack == false)
	{
		//
		if (oldValue == AttackValue::TACKLE || oldValue == AttackValue::TRAMPLE || oldValue == AttackValue::CLOSELANGE)
		{

			returnSpeed = 0.1f;
			returnDis *= returnSpeed;

			bossObj_->worldTransform.translation_.x += returnDis.x;
			bossObj_->worldTransform.translation_.y += returnDis.y;
			bossObj_->worldTransform.translation_.z += returnDis.z;

			bossFbxO_->SetPosition(bossObj_->worldTransform.translation_);
		}
		Reset();
		attackTimer--;


	}
	if (attackTimer <= 0)
	{
		animFlameCount = 0;
		isAttack = true;

		attackValue = rand() % 5 + 1;

		while (oldValue == attackValue)
		{
			attackValue = rand() % 5 + 1;
		}

		//attackValue = AttackValue::TACKLE;
		//attackValue = AttackValue::CLOSELANGE;
		//attackValue = AttackValue::TRAMPLE;
		animNum = 0;
		bossFbxO_->PlayAnimation(animNum);

		attackTimer = 120.0f;
	}

	//攻撃手段が射撃の時
	if (attackValue == AttackValue::FIRE)
	{
		if (isFire == false)
		{
			isFire = true;
		}
		Shot();

	}
	//攻撃手段が突進の時
	else if (attackValue == AttackValue::TACKLE)
	{
		Tackle();
	}
	//攻撃手段がレーザーの時
	else if (attackValue == AttackValue::LASER)
	{
		if (isLaser == false)
		{
			isLaser = true;
		}
		Laser();
	}
	//攻撃手段がのしかかりの時
	else if (attackValue == AttackValue::TRAMPLE)
	{
		Trample();
	}

	//攻撃手段が突進射撃の時
	else if (attackValue == AttackValue::CLOSELANGE)
	{
		if (isclCharge == false)
		{
			isclCharge = true;
		}
		CrossLange();
	}
}

void Boss::BulletUpdate()
{
	//デスフラグが立った球を削除
	bullets_.remove_if([](std::unique_ptr<BossBullet>& bullet) { return bullet->isDead(); });
	//デスフラグが立った球を削除
	lasers_.remove_if([](std::unique_ptr<BossLaser>& laser) { return laser->isDead(); });
	//デスフラグが立った球を削除
	bursts_.remove_if([](std::unique_ptr<BossBurstBullet>& burst) { return burst->isDead(); });

	for (std::unique_ptr<BossBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
	for (std::unique_ptr<BossLaser>& laser : lasers_)
	{
		laser->Update();
	}
	for (std::unique_ptr<BossBurstBullet>& burst : bursts_)
	{
		burst->Update();
	}
}

void Boss::BulletDraw()
{
	//Fire関数用
	for (std::unique_ptr<BossBullet>& bullet : bullets_)
	{
		bullet->Draw(dxcomon_->GetCommandList());
	}
	//Laser関数用
	for (std::unique_ptr<BossLaser>& laser : lasers_)
	{
		laser->Draw(dxcomon_->GetCommandList());
	}
	//CloseLange関数用
	for (std::unique_ptr<BossBurstBullet>& burst : bursts_)
	{
		burst->Draw(dxcomon_->GetCommandList());
	}
}

void Boss::Shot()
{

	Vector3 playerPos;
	Vector3 bulletPos;
	Vector3 distance;

	bulletTimer--;
	if (bulletTimer <= 0)
	{
		if (isFire == true)
		{
			bulletPos = GetPosition();

			// 弾を生成し初期化
			std::unique_ptr<BossBullet> newBullet = std::make_unique<BossBullet>();
			newBullet->Initialize(bulletM_.get(), bossObj_->worldTransform.translation_);

			bulletTimer = 60.0f;
			bulletCount++;
			//
			newBullet->SetPlayer(player_);
			newBullet->SetCollider(new SphereCollider);

			bullets_.push_back(std::move(newBullet));

		}

	}
	//弾の数が3個以上だった時
	if (bulletCount >= 3)
	{
		isAttack = false;
		isFire = false;
		bulletCount = 0;
	}

	if (isFire == false)
	{
		playerPos = player_->GetObject3d()->GetPosition();

		distance - playerPos - bulletPos;

	}
}

void Boss::Tackle()
{


	Vector3 bossPos = bossObj_->GetPosition();
	Vector3 bossPosO_ = bossFbxO_->GetPosition();

	tackleDis = playerPos - bossPos;

	if (isTackle == false)
	{
		//アニメーション用初期化
		if (animFlameCount < 30) {
			animFlameCount++;
		}
		moveTimer--;
	}

	if (moveTimer < 0)
	{
		if (isTackle == false)
		{
			isTackle = true;
			moveTimer = 30.0f;

		}
	}
	if (isTackle == true)
	{
		stopTimer--;
	}
	if (stopTimer < 0)
	{
		isTackle = false;

		stopTimer = 30.0f;
		tackleCount++;
	}
	if (tackleCount < 3)
	{

		if (isTackle == false)
		{
			if (moveTimer >= 45.0f)
			{
				playerPos = player_->GetObject3d()->GetPosition();
				//向きをキャラのいる方向を常に正面にする
				angle = (atan2(tackleDis.x, tackleDis.z) + 3.141592f / 2);

				bossFbxO_->SetRotate(Vector3(0, (angle + 3.141592f / 2) * 1, 0));
			}
		}
		if (isTackle == true)
		{
			if (isDown == false)
			{

				if (isGround == false)
				{
					tackleSpeed = 0.1f;
					tackleDis *= tackleSpeed;

					bossObj_->worldTransform.translation_.x += tackleDis.x;
					//bossObj_->worldTransform.translation_.y += tackleDis.y;
					bossObj_->worldTransform.translation_.z += tackleDis.z;

					bossFbxO_->SetPosition(bossObj_->worldTransform.translation_);
				}

			}

		}
	}
	else
	{
		if (animFlameCount < 60) {
			animFlameCount++;
		}
		isAttack = false;
		tackleCount = 0;
	}

	if (isTackle == true) {

		if (player_->GetPlayerActionManager()->GetNowActNum() == ACTION_NUM::counter)
		{
			audio_->PlayWave("kuri.wav");
			bossToBack = playerPos - bossPosO_;
			bossToBack.nomalize();

			isDown = true;

			
		}


	}
	if (isDown == true)
	{
		bossToBack *= 0.5f;

		downtimer--;
		if (downtimer >= 170)
		{
			bossObj_->worldTransform.translation_.x -= bossToBack.x;
			bossObj_->worldTransform.translation_.z -= bossToBack.z;

			bossFbxO_->SetPosition(bossObj_->worldTransform.translation_);
		}


		if (downtimer < 0)
		{
			tackleCount = 0;
			downtimer = 180;
			isTackle = false;
			isDown = false;
			isAttack = false;
		}

		//KnockBack(bossFbxO_->GetPosition(), bossToBack, 2.0f);
		//downtimer--;
		//if (downtimer < 0)
		//{
		//	downtimer = 180.0f;
		//	isDown = false;
		//	isAttack == false;
		//}
	}

	bossFbxO_->AnimFlameInter(animFlameCount, 60);
}

void Boss::Reset()
{
	attackValue = AttackValue::nasi;

	moveTimer = 60.0f;
	stopTimer = 60.0f;

#pragma region  フラグ関係のリセット

	//攻撃フラグ
	isAttack = false;
	//突進フラグ
	isTackle = false;
	//射撃フラグ
	isFire = false;
	//レーザーフラグ
	isLaser = false;
	//のしかかりのジャンプフラグ
	isJump = false;
	//のしかかるフラグ
	isTrample = false;

	isReturn = false;

#pragma endregion 

#pragma region  タイマー関係のリセット
	//
	//attackTimer = 60.0f;

	coolTimer = 90.0f;

	//動き出すまでのタイマー
	moveTimer = 60.0f;
	//止まるまでのタイマー
	stopTimer = 60.0f;

	//射撃時間
	bulletTimer = 60.0f;

	//レーザー時間
	laserTimer = 180.0f;
	//レーザーの射撃間隔の時間
	lcoolTimer = 90.0f;

	//ジャンプしている時間
	jumpTimer = 60.0f;
	//のしかかるまでの時間
	tramChageTimer = 120.0f;

	//のしかかってる時間
	trampleTimer = 120.0f;

#pragma endregion
#pragma region  pos関係のリセット
	//敵のノックバック用のベクトル
	bossToBack = { 0,0,0 };
	//プレイヤーと敵の距離
	distance = { 0,0,0 };
	//distanceのNormalize
	normDis = { 0,0,0 };
	//突進時のプレイヤーの座標
	playerPos = { 0,0,0 };
	//敵のプレイヤーの距離
	tackleDis = { 0,0,0 };
	//距離のNormalize
	normTdis = { 0,0,0 };
	//弾とプレイヤーの距離
	bulletDistance = { 0,0,0 };
	//のしかかり時のプレイヤーの座標
	tramPlayerPos = { 0,0,0 };
	//のしかかり時のプレイヤーと敵の距離
	tramDistance = { 0,0,0 };
	//レーザー時のプレイヤーの座標
	lPlayerPos = { 0,0,0 };
	//レーザー時の敵の座標
	laserPos = { 0,0,0 };
	//レーザー時のプレイヤーと敵の距離
	lDistance = { 0,0,0 };
#pragma endregion

#pragma region  カウント関係のリセット
	//突進の回数
	tackleCount = 0;
	//Fire関数内の弾の個数
	bulletCount = 0;
	//レーザーの個数
	laserCount = 0;
#pragma endregion

	//突進射撃時の弾の個数
	burstCount = 0;
	//射撃間隔
	bcoolTimer = 3.0f;
	//射撃している時間
	burstTimer = 180.0f;
	//突進射撃時のクールタイマー
	clTimer = 60.0f;
	//突進射撃時の突進開始までの時間
	clChargeTimer = 100.0f;
	//突進チャージ中かどうかのフラグ
	isclCharge = false;
	//突進しているかどうかのフラグ
	isCross = false;
	//射撃しているかどうかのフラグ
	isBurst = false;
}

Vector3 Boss::GetPosition()
{
	return bossObj_->worldTransform.translation_;
}

Boss* Boss::GetInstance()
{
	static Boss instance;
	return &instance;
}

void Boss::Laser()
{
	laserTimer--;

	laserPos = bossObj_->GetPosition();

	//敵の向きを自キャラの正面に向くように回転させる
	angle = (atan2(lDistance.x, lDistance.z) + 3.141592f / 2);
	//回転を反映
	bossFbxO_->SetRotate(Vector3(0, (angle + 3.141592f / 2) * 1, 0));


	if (laserTimer < 0)
	{

		if (isLaser == true)
		{
			//レーザーの攻撃回数が3回未満の時
			if (laserCount < 3)
			{
				// 弾を生成し初期化
				std::unique_ptr<BossLaser> newBullet = std::make_unique<BossLaser>();
				newBullet->Initialize(Vector3(bossObj_->worldTransform.translation_.x, bossObj_->worldTransform.translation_.y + 8, bossObj_->worldTransform.translation_.z));

				laserTimer = 60.0f;
				laserCount++;
				//
				newBullet->SetPlayer(player_);
				newBullet->SetCollider(new SphereCollider);

				lasers_.push_back(std::move(newBullet));
			}
			else
			{
				isAttack = false;
				laserCount = 0;
			}
		}
	}
	else
	{
		lPlayerPos = player_->GetObject3d()->GetPosition();

		lDistance = lPlayerPos - laserPos;
	}
}

void Boss::Trample()
{
	Vector3 bossPosF_ = bossObj_->GetPosition();

	//自機とボスの位置の差分ベクトル
	Vector3 rotate = tramPlayerPos - bossPosF_;

	if (isTrample == false)
	{
		tramPlayerPos = player_->GetObject3d()->GetPosition();
	}

	Vector3 bossToJumpDis;
	//ジャンプ位置とボスの位置の差分ベクトル
	bossToJumpDis = JumpPos - bossPosF_;

	//自機とボスのジャンプ位置の差分ベクトル
	tramDistance = tramPlayerPos - JumpPos;

#pragma region ジャンプ前
	if (isJump == false)
	{
		if (tramChageTimer > 0)
		{
			tramChageTimer--;
		}
		if (isTrample == false)
		{
			//敵の向きを自機の正面
			angle = (atan2(rotate.x, rotate.z) + 3.141592f / 2);

			bossFbxO_->SetRotate(Vector3(0, (angle + 3.141592f / 2) * 1, 0));
		}

	}
	if (tramChageTimer <= 0)
	{
		if (isJump == false)
		{
			JumpPos.x = player_->GetObject3d()->wtf.translation_.x;
			JumpPos.y = player_->GetObject3d()->wtf.translation_.y + 30;
			JumpPos.z = player_->GetObject3d()->wtf.translation_.z;
			isJump = true;
		}
	}
#pragma endregion 

#pragma region ジャンプ中
	if (isJump == true)
	{
		if (jumpTimer > 0)
		{
			jumpTimer--;

			bossToJumpDis *= (trampleSpeed / 5);

			bossObj_->worldTransform.translation_.x += bossToJumpDis.x;
			bossObj_->worldTransform.translation_.y += bossToJumpDis.y;
			bossObj_->worldTransform.translation_.z += bossToJumpDis.z;

			bossFbxO_->SetPosition(bossObj_->worldTransform.translation_);

		}
		else
		{
			isJump = false;
			if (isTrample == false)
			{
				isTrample = true;
			}
		}
	}
#pragma endregion 

#pragma region 
	if (isTrample == true)
	{
		trampleTimer--;
		//地面との当たり判定(パワーコード)
		if (bossObj_->worldTransform.translation_.y <= 5)
		{
			bossObj_->worldTransform.translation_.y = 15;
		}

		tramDistance *= trampleSpeed;
		bossObj_->worldTransform.translation_.y += tramDistance.y;

		bossFbxO_->SetPosition(bossObj_->worldTransform.translation_);

		tackleDis.x = std::abs(tackleDis.x);
		tackleDis.z = std::abs(tackleDis.z);
	}
	if (trampleTimer <= 0)
	{
		if (isAttack == true)
		{
			isAttack = false;
			isJump = false;
			isTrample = false;
			trampleTimer = 120.0f;
			tramChageTimer = 120.0f;
			jumpTimer = 60.0f;
		}
		bossFbxO_->SetRotate(Vector3(0, 0, 0));
	}
#pragma endregion 

}
void Boss::CrossLange()
{

	float speed = 0.1f;

	//突進のチャージをしているとき
	if (isclCharge == true)
	{
		clChargeTimer--;
	}
	//射撃しているとき
	if (isBurst == true)
	{
		burstTimer--;
		bcoolTimer--;
	}

	crossLangePos = bossFbxO_->GetPosition();

	//射撃フラグがfalseの時
	if (isBurst == false)
	{
		clDistance = clPlayerPos - crossLangePos;

	}

	//敵の向きの調整
	if (clChargeTimer > 90.0f || isBurst == true)
	{
		clPlayerPos = player_->GetObject3d()->GetPosition();
		clRotate = clPlayerPos - crossLangePos;

		//敵の向きを自機の正面
		angle = (atan2(clRotate.x, clRotate.z) + 3.141592f / 2);

		bossFbxO_->SetRotate(Vector3(0, (angle + 3.141592f / 2) * 1, 0));
	}
	//チャージタイマーが0以下の時
	if (clChargeTimer <= 0)
	{
		isclCharge = false;
		isCross = true;
	}

	//突進フラグがtrueの時
	if (isCross == true)
	{
		clTimer--;
		//ステージ内にいるとき
		if (isGround == false)
		{
			clDistance *= speed;

			bossObj_->worldTransform.translation_.x += clDistance.x;
			bossObj_->worldTransform.translation_.y += clDistance.y;
			bossObj_->worldTransform.translation_.z += clDistance.z;

			bossFbxO_->SetPosition(bossObj_->worldTransform.translation_);

		}

	}

	//突進中タイマーが0以下の時
	if (clTimer <= 0)
	{
		isCross = false;
		isBurst = true;
	}

	//射撃間隔が0以下になったとき
	if (bcoolTimer <= 0)
	{
		//射撃フラグがオン
		if (isBurst == true)
		{
			//弾の数が30未満
			if (burstCount < 30)
			{
				// 弾を生成し初期化
				std::unique_ptr<BossBurstBullet> newBullet = std::make_unique<BossBurstBullet>();
				newBullet->Initialize(bulletM_.get(), Vector3(bossObj_->worldTransform.translation_.x, bossObj_->worldTransform.translation_.y + 8, bossObj_->worldTransform.translation_.z));

				burstCount++;
				bcoolTimer = 3.0f;
				//
				newBullet->SetPlayer(player_);
				newBullet->SetCollider(new SphereCollider);

				bursts_.push_back(std::move(newBullet));
			}


		}
	}
	//全体の射撃時間が0以下になったとき
	if (burstTimer <= 0)
	{
		Reset();
		burstCount = 0;
		isBurst = false;
		isAttack = false;

	}
}

void Boss::Damage()
{
	Boss::hp -= 1;
}

bool Boss::GetIsAtkCollide()
{
	return isAtkCollide;
}

FBXObject3d* Boss::GetObject3d()
{
	return bossFbxO_.get();
}

void Boss::minusHp(int damage)
{
	hp -= damage;
}

Vector3 Boss::KnockBack(const Vector3& position, Vector3& distance, float speed)
{
	//
	/*Vector3 normal = distance.nomalize();*/
	Vector3 pos = position;
	pos = pos + distance * speed;

	return pos;
}
