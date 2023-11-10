#include "FbxPlayer.h"
#include "Ease.h"
#include "GameCamera.h"
#include <random>
#include "HitStopManager.h"
#include"Move.h"
#include"Atk1.h"


#include"RaycastHit.h"



int FbxPlayer::hp = 100;
bool FbxPlayer::isAtkCollide = false;
bool FbxPlayer::isGuardCollide = false;

FbxPlayer::FbxPlayer()
{

}

FbxPlayer::~FbxPlayer()
{

}

void FbxPlayer::Initialize(FBXModel* fbxModel)
{
	input_ = Input::GetInstance();
	audio_ = std::make_unique<Audio>();
	audio_->Initialize();

	//音データ読み込み
	audio_->LoadWave("kuri.wav");

	gameObject_ = FBXObject3d::Create();
	gameObject_->SetModel(fbxModel);
	gameObject_->SetIsBonesWorldMatCalc(true);	//ボーンワールド行列計算あり
	gameObject_->SetScale({ 1,1,1 });
	gameObject_->SetPosition({ 0,15.f,0 });
	gameObject_->Update();

	hoverCarModel_ = Mesh::LoadFormOBJ("hoverCar", false);
	hoverCarObject_ = Object3d::Create();
	hoverCarObject_->SetModel(hoverCarModel_.get());
	hoverCarObject_->SetPosition(gameObject_->GetPosition());
	hoverCarObject_->SetRotate(gameObject_->GetRotate());
	hoverCarObject_->SetScale({ 1.f,1.f,1.f });
	

	SPHERE_COLISSION_NUM = static_cast<int>(gameObject_->GetBonesMatPtr()->size());
	sphere.resize(SPHERE_COLISSION_NUM);
	spherePos.resize(SPHERE_COLISSION_NUM);
	gameObject_.get()->isBonesWorldMatCalc = true;
	coliderPosTest_.resize(SPHERE_COLISSION_NUM);
	hpModel_ = Mesh::LoadFormOBJ("cube", false);

	//当たり判定初期化
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		sphere[i] = new SphereCollider;
		CollisionManager::GetInstance()->AddCollider(sphere[i]);
		spherePos[i] = gameObject_.get()->bonesMat[i].GetWorldPos();
		sphere[i]->SetBasisPos(&spherePos[i]);
		sphere[i]->SetRadius(2.5f);
		sphere[i]->SetAttribute(COLLISION_ATTR_ALLIES);
		sphere[i]->Update();
		//test
		coliderPosTest_[i] = Object3d::Create();
		coliderPosTest_[i]->SetModel(hpModel_.get());
		coliderPosTest_[i]->SetPosition(sphere[i]->center);
		coliderPosTest_[i]->SetScale({ sphere[i]->GetRadius(),sphere[i]->GetRadius() ,sphere[i]->GetRadius() });
		coliderPosTest_[i]->SetRotate({ 0,0,0 });
		coliderPosTest_[i]->Update();

	}

	//ヒットポイント
	hp = maxHp_;

	//パーティクル
	hitParticle_ = std::make_unique<ParticleManager>();
	hitParticle_->Initialize();
	hitParticle_->LoadTexture("effect.png");
	hitParticle_->Update();

	bombParticle_ = std::make_unique<ParticleManager>();
	bombParticle_->Initialize();
	bombParticle_->LoadTexture("bombBlack.png");
	bombParticle_->Update();

	// 現在時刻を取得してシード値とする
	std::srand(static_cast<int>(std::time(nullptr)));

	//初期化
	FbxPlayer::isAtkCollide = false;
	isGuardCollide = false;


	reticle_.Initialize(gameObject_->GetWorldTransformPtr());

	//弾
	bulletModel_ = Mesh::LoadFormOBJ("cube", true);


}

void FbxPlayer::Update()
{

	//デスフラグの立った弾を削除

	rapidBullets_.remove_if([](std::unique_ptr<PlayerRapidBullet>& bullet) {
		return bullet->ReturnIsDead();
		});


	if (isHitStop == false)
	{
		//ワールド上の自機の回転量yを求める
		cameraAngle_.y =
			atan2(gameObject_.get()->GetCamera().GetTarget().x - gameObject_.get()->GetCamera().GetEye().x,
				gameObject_.get()->GetCamera().GetTarget().z - gameObject_.get()->GetCamera().GetEye().z);

		//移動処理
		Move();

		Ray ray;
		ray.start = gameObject_->GetCamera().GetEye();
		ray.dir = reticle_.GetFarReticleObjPtr()->worldTransform.matWorld_.GetWorldPos() - gameObject_->GetCamera().GetEye();


		RaycastHit raycast;

		//// レイキャストによるロックオン登録
		//if (CollisionManager::GetInstance()->Raycast(ray, &raycast, 120.f)) {

		//	if (raycast.collider->GetAttribute() == COLLISION_ATTR_ENEMIES && raycast.object != nullptr) {
		//		// ロックオン処理
		//		PRockTarget newRockTarget;
		//		rockTargets_.push_back(newRockTarget);
		//		int nowRockNum = static_cast<int>(rockTargets_.size()) - 1;

		//		rockTargets_[nowRockNum].targetWtfPtr = raycast.collider->GetObject3d()->GetWorldTransformPtr();
		//		rockTargets_[nowRockNum].isRockOn = true;

		//		// そのロックオンによって弾発射
		//		std::unique_ptr<PlayerHomingBullet> newHomingBullet;
		//		newHomingBullet = std::make_unique<PlayerHomingBullet>();
		//		newHomingBullet->Initialize(bulletModel_.get(), gameObject_->GetPosition(), gameObject_->GetRotate());
		//		newHomingBullet->SetTargerPtr(rockTargets_[nowRockNum].targetWtfPtr);
		//		homingBullets_.push_back(std::move(newHomingBullet));



		//	}
		//}

		if (input_->TriggerMouseButton(0)) {
			std::unique_ptr<PlayerRapidBullet> newRapidBullet;
			newRapidBullet = std::make_unique<PlayerRapidBullet>();
			newRapidBullet->Initialize(bulletModel_.get(), gameObject_->GetPosition(), gameObject_->GetRotate());
			rapidBullets_.push_back(std::move(newRapidBullet));
		}

		for (std::unique_ptr<PlayerRapidBullet>& rapidBullet : rapidBullets_) {

			if (rapidBullet->GetSphereCollider()->GetIsHit() == true) {
				if (rapidBullet->GetSphereCollider()->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_ENEMIES) {
					rapidBullet->SetIsDead(true);
					CreateBulHitParticle(rapidBullet->GetSphereCollider()->GetCollisionInfo().inter);
				}
			}
		}

		//if (input_->TriggerKey(DIK_N)) {
		//	isDead_ = true;
		//	deadActNum_ = DEAD_ACT_NUM::crash;
		//}

		if (isDead_ == true) {
			//前フレーム処理
			oldDeadActNum_ = deadActNum_;

			if (deadActNum_ == DEAD_ACT_NUM::crash) {
				CrashAction();
			}
			if (deadActNum_ == DEAD_ACT_NUM::bomb) {
				if (oldDeadActNum_ == DEAD_ACT_NUM::crash) {
					bombStartPos_ = gameObject_->wtf.translation_;
					bombStartRot_ = gameObject_->wtf.rotation_;
				}
				BombAction();
			}
			else if (deadActNum_ == DEAD_ACT_NUM::disappear) {
				if (oldDeadActNum_ == DEAD_ACT_NUM::bomb) {

				}
			}




		}



		//更新
		hoverCarObject_->SetPosition(gameObject_->GetPosition());
		hoverCarObject_->SetRotate(gameObject_->GetRotate());
		for (int i = 0; i < homingBullets_.size(); i++) {
			homingBullets_[i]->Update();
		}
		for (std::unique_ptr<PlayerRapidBullet>& rapidBullet : rapidBullets_) {
			rapidBullet->Update();
		}


		//y固定
		//float yPos = 2.0f;
		//if (isDead_ == false) {
		//	hoverCarObject_->worldTransform.translation_.y = yPos;
		//	gameObject_->wtf.translation_.y = yPos;
		//}

		//更新
		PColliderUpdate();

		hitParticle_->Update();
		bombParticle_->Update();
		gameObject_->Update();
		hoverCarObject_->Update();

		count++;




		if (oldAnimCT != animCT)
		{
			gameObject_.get()->PlayAnimation(animCT);
		}



	} //ヒットストップ


	reticle_.Update();



}



void FbxPlayer::Draw(ID3D12GraphicsCommandList* cmdList)
{

	//gameObject_->Draw(cmdList);

	if (deadActNum_ != DEAD_ACT_NUM::disappear) {
		hoverCarObject_->Draw(cmdList);
	}

	hitParticle_->Draw(cmdList);
	bombParticle_->Draw(cmdList);

	if (isDead_ == false) {
		reticle_.Draw(cmdList);
	}

	for (int i = 0; i < homingBullets_.size(); i++) {
		homingBullets_[i]->Draw(cmdList);
	}
	for (std::unique_ptr<PlayerRapidBullet>& rapidBullet : rapidBullets_) {
		rapidBullet->Draw(cmdList);
	}

}

void FbxPlayer::CreateBulHitParticle(Vector3 posArg)
{

	for (int i = 0; i < 40; i++) {


		const float rnd_pos = 2.0f;
		Vector3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 2.f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		Vector3 acc{};
		const float rnd_acc = 0.1f;
		acc.x = -(float)rand() / RAND_MAX * rnd_acc;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;
		acc.z = -(float)rand() / RAND_MAX * rnd_acc;

		//追加
		hitParticle_->Add(maxHitParticleLife_, posArg + pos, vel, acc, startHitParticleSize_, endHitParticleSize_);
	}

}

void FbxPlayer::minusHp(int damage)
{
	hp -= damage;
}

void FbxPlayer::Move()
{

	Vector3 primaryNorm = MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate + 0.005f)
		- MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate);
	primaryNorm.nomalize();

	float pAngle = atan2f(primaryNorm.x, primaryNorm.z);

	Vector3 primaryPos = MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate + 0.002f) + primaryNorm * 10.0f;



	//キー入力があったら
	if (input_->PushKey(DIK_W) ||
		input_->PushKey(DIK_A) ||
		input_->PushKey(DIK_S) ||
		input_->PushKey(DIK_D) && isDead_ == false)
	{


		//Z軸方向にの速度を入れる
		velocity_ = { 0 , 0 , 0.1f };

		float kDiagonalSpeed = kMoveSpeed_ * 0.707f;	// √2をかける

		//W,Dを押していたら
		if (input_->PushKey(DIK_W) && input_->PushKey(DIK_D))
		{
			nowPos.x += kDiagonalSpeed;


			if (faceAngle_.x >= -faceMaxAngle_) {
				faceAngle_.x -= faceRotSpeed_;
			}
			if (faceAngle_.y <= faceMaxAngle_) {
				faceAngle_.y += faceRotSpeed_;

			}



		}

		//W,Aを押していたら
		else if (input_->PushKey(DIK_W) && input_->PushKey(DIK_A))
		{

			nowPos.x -= kDiagonalSpeed;

			if (faceAngle_.x >= -faceMaxAngle_) {
				faceAngle_.x -= faceRotSpeed_;
			}
			if (faceAngle_.y >= -faceMaxAngle_) {
				faceAngle_.y -= faceRotSpeed_;

			}


		}

		//S,Dを押していたら
		else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_D))
		{

			nowPos.x += kDiagonalSpeed;


			if (faceAngle_.x <= faceMaxAngle_) {
				faceAngle_.x += faceRotSpeed_;
			}
			if (faceAngle_.y <= faceMaxAngle_) {
				faceAngle_.y += faceRotSpeed_;

			}


		}

		//S,Aを押していたら
		else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_A))
		{
			nowPos.x -= kDiagonalSpeed;

			if (faceAngle_.x <= faceMaxAngle_) {
				faceAngle_.x += faceRotSpeed_;
			}
			if (faceAngle_.y >= -faceMaxAngle_) {
				faceAngle_.y -= faceRotSpeed_;

			}


		}

		//Wを押していたら
		else if (input_->PushKey(DIK_W))
		{

			if (faceAngle_.x >= -faceMaxAngle_) {
				faceAngle_.x -= faceRotSpeed_;
			}

		}

		//Sを押していたら
		else if (input_->PushKey(DIK_S))
		{

			if (faceAngle_.x <= faceMaxAngle_) {
				faceAngle_.x += faceRotSpeed_;
			}

		}

		//Dを押していたら
		else if (input_->PushKey(DIK_D))
		{
			nowPos.x += kMoveSpeed_;


			if (faceAngle_.y <= faceMaxAngle_) {
				faceAngle_.y += faceRotSpeed_;


			}


		}

		//Aを押していたら
		else if (input_->PushKey(DIK_A))
		{
			nowPos.x -= kMoveSpeed_;
			if (faceAngle_.y >= -faceMaxAngle_) {
				faceAngle_.y -= faceRotSpeed_;

			}

		}

		//押されていないときの処理

		if (input_->PushKey(DIK_A) != 1 && input_->PushKey(DIK_D) != 1) {
			if (faceAngle_.y > 0.02f) {

				faceAngle_.y -= returnRotSpeed;

			}
			else if (faceAngle_.y < -0.02f) {
				faceAngle_.y += returnRotSpeed;

			}

		}

		if (input_->PushKey(DIK_W) != 1 && input_->PushKey(DIK_S) != 1) {
			if (faceAngle_.x > 0.02f) {
				faceAngle_.x -= returnRotSpeed;
			}
			else if (faceAngle_.x < -0.02f) {
				faceAngle_.x += returnRotSpeed;
			}

		}

	}
	else
	{

		//押されていないときの処理
		if (faceAngle_.x > 0.02f) {
			faceAngle_.x -= returnRotSpeed;
		}
		else if (faceAngle_.x < -0.02f) {
			faceAngle_.x += returnRotSpeed;
		}

		if (faceAngle_.y > 0.02f) {
			faceAngle_.y -= returnRotSpeed;

		}
		else if (faceAngle_.y < -0.02f) {
			faceAngle_.y += returnRotSpeed;

		}
	}


	gameObject_->wtf.rotation_ = { faceAngle_.x, faceAngle_.y + pAngle ,faceAngle_.z };


	//画面上で自機が動くためのmatrix
	pAngleMat.identity();
	pAngleMat.rotateY(pAngle);
	Vector3 viewPos = MathFunc::bVelocity(nowPos, pAngleMat);

	Vector3 playerPos = primaryPos + viewPos;
	playerPos.y = 0.3f;

	gameObject_->SetPosition(playerPos);


}

void FbxPlayer::BulletShot()
{
}

void FbxPlayer::CrashAction()
{
	if (deadActCount_ < maxCrashActCount_) {
		deadActCount_++;
	}
	else {
		deadActCount_ = 0;
		deadActNum_ = DEAD_ACT_NUM::bomb;
	}

	if (deadActCount_ % 10 == 0) {
		bombParticle_->Add(40, hoverCarObject_->GetPosition(), Vector3(0, 0.01f, 0), Vector3(0, 0.01f, 0), 7.f, 0.2f);
	}





}

void FbxPlayer::BombAction()
{
	if (deadActCount_ < maxCrashActCount_) {
		deadActCount_++;
	}
	else {
		deadActCount_ = 0;
		deadActNum_ = DEAD_ACT_NUM::disappear;
	}

	if (deadActCount_ == 1) {
		bombParticle_->RandParticle(hoverCarObject_->GetPosition());
	}

	//　吹っ飛び処理
	bombStartPos_ += bombPosVel_;
	bombStartRot_ += bombRotateVel_;
	gameObject_->wtf.translation_ = bombStartPos_;
	gameObject_->wtf.rotation_ = bombStartRot_;




}

FBXObject3d* FbxPlayer::GetObject3d()
{
	return gameObject_.get();
}

bool FbxPlayer::GetIsAtkCollide()
{
	return FbxPlayer::isAtkCollide;
}

void FbxPlayer::SetIsAtkCollide(bool isAtkCollideArg)
{
	FbxPlayer::isAtkCollide = isAtkCollideArg;
}

bool FbxPlayer::GetIsGuardCollide()
{
	return isGuardCollide;
}

void FbxPlayer::SetIsGuardCollide(bool isGuardCollideArg)
{
	FbxPlayer::isGuardCollide = isGuardCollideArg;
}

int FbxPlayer::GetHp()
{
	return FbxPlayer::hp;
}

void FbxPlayer::SetHp(int hpArg)
{
	FbxPlayer::hp = hpArg;
}

void FbxPlayer::PColliderUpdate()
{
	if (hitDeley > 0) {	//毎フレームヒットを防止
		hitDeley--;
	}


	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		if (isAtkCollide == true && hitDeley <= 0 && sphere[i]->GetIsHit() == true) {

			if (sphere[i]->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_ENEMIES) {

				audio_->PlayWave("kuri.wav");

				hitDeley = delayCount_;
				hitParticle_->RandParticle(sphere[i]->GetCollisionInfo().inter);
				HitStopManager::GetInstance()->SetHitStop(&isHitStop, 2);
				break;
			}

		}
		if (sphere[i]->GetIsHit() == true &&
			sphere[i]->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_ENEMIEBULLETS
			&& hitDeley <= 0) {
			audio_->PlayWave("kuri.wav");
			hitDeley = delayCount_;
			this->hp -= damage_;
			break;
		}
	}

	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		spherePos[i] = gameObject_.get()->bonesMat[i].GetWorldPos();
		sphere[i]->Update();
		coliderPosTest_[i]->SetPosition(sphere[i]->center);
		coliderPosTest_[i]->Update();
	}



}


int FbxPlayer::GetIsDeadActNum()
{
	return deadActNum_;
}

void FbxPlayer::SetIsDeadActNum(int arg)
{
	deadActNum_ = arg;
}

void FbxPlayer::PlayerPalamReset()
{

	deadActCount_ = 0;
	oldDeadActNum_ = 0;
	deadActNum_ = 0;
	bombStartPos_ = {};
	bombStartRot_ = {};
	SetIsDeadActNum(DEAD_ACT_NUM::none);
	isDead_ = false;

}

Vector3 FbxPlayer::GetNowFaceAngle()
{

	Vector3 nowVelocity_ = MathFunc::MatVector(velocity_, gameObject_->wtf.matWorld_);
	return nowVelocity_;
}

void FbxPlayer::SetRailCameraInfo(RailCameraInfo* info)
{
	railCameraInfo_ = info;
}

void FbxPlayer::GoGameOver()
{
	isDead_ = true;
	deadActNum_ = DEAD_ACT_NUM::crash;
}
