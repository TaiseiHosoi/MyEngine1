#include "FbxPlayer.h"
#include "Ease.h"
#include "GameCamera.h"
#include <random>
#include "HitStopManager.h"
#include"Move.h"
#include"Atk1.h"


#include"RaycastHit.h"





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
	gameObject_->SetPosition({ 0,0,0 });
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
	hp_ = maxHp_;
	isHitByATK_ = false;

	//パーティクル
	hitParticle_ = std::make_unique<ParticleManager>();
	hitParticle_->Initialize();
	hitParticle_->LoadTexture("effect.png");
	hitParticle_->Update();

	bombParticle_ = std::make_unique<ParticleManager>();
	bombParticle_->Initialize();
	bombParticle_->LoadTexture("bombBlack.png");
	bombParticle_->Update();

	slashParticle_ = std::make_unique<ParticleManager>();
	slashParticle_->Initialize();
	slashParticle_->LoadTexture("ParticleEffectR.png");
	slashParticle_->Update();


	// 現在時刻を取得してシード値とする
	std::srand(static_cast<int>(std::time(nullptr)));


	reticle_.Initialize(gameObject_->GetWorldTransformPtr(), railTargetPosPtr_);

	//弾
	bulletModel_ = Mesh::LoadFormOBJ("bume", true);

	


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
		MoveBody();
		//ターゲット移動処理
		MoveTarget();

		Ray ray;
		ray.start = gameObject_->GetCamera().GetEye();
		ray.dir = reticle_.GetFarReticleObjPtr()->worldTransform.matWorld_.GetWorldPos() - gameObject_->GetCamera().GetEye();


		nowShotDelayCount_++;
		if (input_->PushMouseButton(0) || input_->ButtonInput(RT)) {

			
			if (nowShotDelayCount_ > shotDelay_) {

				std::unique_ptr<PlayerRapidBullet> newRapidBullet;
				newRapidBullet = std::make_unique<PlayerRapidBullet>();
				newRapidBullet->Initialize(bulletModel_.get(), gameObject_->GetPosition(), gameObject_->GetRotate());
				rapidBullets_.push_back(std::move(newRapidBullet));

				nowShotDelayCount_ = 0;
			}
		}

		for (std::unique_ptr<PlayerRapidBullet>& rapidBullet : rapidBullets_) {

			if (rapidBullet->GetSphereCollider()->GetIsHit() == true) {
				if (rapidBullet->GetSphereCollider()->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_ENEMIES) {
					rapidBullet->SetIsDead(true);
					CreateBulHitParticle(rapidBullet->GetSphereCollider()->GetCollisionInfo().inter);
				}
			}
		}
		
		
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

		

		//更新
		PColliderUpdate();
		DamageEffectUpdate();
		hitParticle_->Update();
		bombParticle_->Update();
		slashParticle_->Update();
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

void FbxPlayer::ParticleDraw(ID3D12GraphicsCommandList* cmdList)
{
	hitParticle_->Draw(cmdList);
	bombParticle_->Draw(cmdList);
	slashParticle_->Draw(cmdList);
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

		float sRot = 0;
		const float rnd_sRot = 2.0f;
		sRot = (float)rand() / RAND_MAX * rnd_sRot - 1.0f;

		//追加
		bombParticle_->Add(maxHitParticleLife_, posArg + pos, vel, acc, startHitParticleSize_, endHitParticleSize_);
		slashParticle_->Add(maxHitParticleLife_, posArg, {0,0,0},{0,0,0}, 10.f, 0.1f, sRot,0.1f);
	}

}

void FbxPlayer::MinusHp(int damage)
{
	hp_ -= damage;
	
}

int FbxPlayer::GetHp()
{
	return hp_;
}

void FbxPlayer::SetHp(int hp)
{
	hp_ = hp;
}



void FbxPlayer::MoveBody()
{

	Vector3 primaryNorm = MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate + 0.005f)
		- MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate);
	primaryNorm.nomalize();

	float pAngle = atan2f(primaryNorm.x, primaryNorm.z);

	Vector3 primaryPos = MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate + 0.002f) + primaryNorm * 10.0f;


	//移動速度計算
	moveSpeed_ = increaseSpeed_;
	//float kDiagonalSpeed = kMoveSpeed_ * 0.707f;	// √2をかける

	//キー入力があったら
	if (input_->PushKey(DIK_W) ||
		input_->PushKey(DIK_A) ||
		input_->PushKey(DIK_S) ||
		input_->PushKey(DIK_D) || 
		input_->StickInput(L_RIGHT) ||
		input_->StickInput(L_LEFT) ||
		input_->StickInput(L_UP) ||
		input_->StickInput(L_DOWN) &&
		isDead_ == false)
	{

		//押し続けると増えるスピード
		increaseSpeed_ += increaseSpeedVel_;



		//W,Dを押していたら
		if (input_->PushKey(DIK_W) && input_->PushKey(DIK_D) || input_->StickInput(L_UP) && input_->StickInput(L_RIGHT))
		{

			//if (nowPos.x < maxParallelMovement_) {
			if (nowFlameParallelMove_ < maxIncreaseSpeed_) {
				nowFlameParallelMove_ += increaseSpeedVel_;
			}
			//}


			if (faceAngle_.x >= -faceMaxAngleY_) {
				faceAngle_.x -= faceRotSpeedY_;
			}
			if (faceAngle_.y <= faceMaxAngleY_) {
				faceAngle_.y += faceRotSpeedY_;

			}



		}

		//W,Aを押していたら
		else if (input_->PushKey(DIK_W) && input_->PushKey(DIK_A) || input_->StickInput(L_UP) && input_->StickInput(L_LEFT))
		{
			if (nowFlameParallelMove_ > -maxIncreaseSpeed_) {
				nowFlameParallelMove_ -= increaseSpeedVel_;
			}

			if (faceAngle_.x >= -faceMaxAngleX_) {
				faceAngle_.x -= faceRotSpeedX_;
			}
			if (faceAngle_.y >= -faceMaxAngleY_) {
				faceAngle_.y -= faceRotSpeedY_;

			}


		}

		//S,Dを押していたら
		else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_D) || input_->StickInput(L_DOWN) && input_->StickInput(L_RIGHT))
		{
			if (nowFlameParallelMove_ < maxIncreaseSpeed_) {
				nowFlameParallelMove_ += increaseSpeedVel_;
			}


			if (faceAngle_.x <= minFaceAngleX_) {
				faceAngle_.x += faceRotSpeedX_;
			}
			if (faceAngle_.y <= faceMaxAngleY_) {
				faceAngle_.y += faceRotSpeedY_;

			}


		}

		//S,Aを押していたら
		else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_A) || input_->StickInput(L_DOWN) && input_->StickInput(L_LEFT))
		{
			if (nowFlameParallelMove_ > -maxIncreaseSpeed_) {
				nowFlameParallelMove_ -= increaseSpeedVel_;
			}

			if (faceAngle_.x <= minFaceAngleX_) {
				faceAngle_.x += faceRotSpeedX_;
			}
			if (faceAngle_.y >= -faceMaxAngleY_) {
				faceAngle_.y -= faceRotSpeedY_;

			}


		}

		//Wを押していたら
		else if (input_->PushKey(DIK_W) || input_->StickInput(L_UP))
		{

			if (faceAngle_.x >= -faceMaxAngleX_) {
				faceAngle_.x -= faceRotSpeedX_;
			}

		}

		//Sを押していたら
		else if (input_->PushKey(DIK_S) || input_->StickInput(L_DOWN))
		{

			if (faceAngle_.x <= minFaceAngleX_) {
				faceAngle_.x += faceRotSpeedY_;
			}

		}

		//Dを押していたら
		else if (input_->PushKey(DIK_D) || input_->StickInput(L_RIGHT))
		{
			if (nowFlameParallelMove_ < maxIncreaseSpeed_) {
				nowFlameParallelMove_ += increaseSpeedVel_;
			}

			if (faceAngle_.y <= faceMaxAngleY_) {
				faceAngle_.y += faceRotSpeedY_;


			}


		}

		//Aを押していたら
		else if (input_->PushKey(DIK_A) || input_->StickInput(L_LEFT))
		{
			if (nowFlameParallelMove_ > -maxIncreaseSpeed_) {
				nowFlameParallelMove_ -= increaseSpeedVel_;
			}

			if (faceAngle_.y >= -faceMaxAngleY_) {
				faceAngle_.y -= faceRotSpeedY_;

			}

		}

		//押されていないときの処理

		if (input_->PushKey(DIK_A) || input_->StickInput(L_LEFT) != 1 && input_->PushKey(DIK_D) || input_->StickInput(L_RIGHT) != 1) {
			if (faceAngle_.y > 0.02f) {

				faceAngle_.y -= returnRotSpeed_;

			}
			//else if (faceAngle_.y < -0.02f) {
			//	faceAngle_.y += returnRotSpeed_;

			//}

		}

		if (input_->PushKey(DIK_W) || input_->StickInput(L_UP) != 1 && input_->PushKey(DIK_S) || input_->StickInput(L_DOWN) != 1) {
			if (faceAngle_.x > 0.02f) {
				faceAngle_.x -= returnRotSpeed_;
			}
			/*else if (faceAngle_.x < -0.02f) {
				faceAngle_.x += returnRotSpeed_;
			}*/

		}

	}
	else
	{
		increaseSpeed_ = 0;	//加速度リセット

		//押されていないときの処理
		if (faceAngle_.x > maxFaceAngle_) {
			faceAngle_.x -= returnRotSpeed_;
		}
		//else if (faceAngle_.x < -maxFaceAngle_) {
		//	faceAngle_.x += returnRotSpeed_;
		//}

		if (faceAngle_.y > maxFaceAngle_) {
			faceAngle_.y -= returnRotSpeed_;

		}
		else if (faceAngle_.y < -maxFaceAngle_) {
			faceAngle_.y += returnRotSpeed_;

		}

		//減速
		if (nowFlameParallelMove_ > 0) {
			nowFlameParallelMove_ -= increaseSpeedVel_;
		}
		else if (nowFlameParallelMove_ < 0) {
			nowFlameParallelMove_ += increaseSpeedVel_;
		}

		
	}

	//一定以上の左右移動量行かないようにする
	if (nowPos.x > maxParallelMovement_ ) {
		nowFlameParallelMove_ = 0;
		nowPos.x = maxParallelMovement_;
	}
	else if (nowPos.x < -maxParallelMovement_) {
		nowFlameParallelMove_ = 0;
		nowPos.x = -maxParallelMovement_;
	}

	//挙動
	nowPos.x += nowFlameParallelMove_;
	playerParalellMoveVal_ = nowPos.x;
	gameObject_->wtf.rotation_ = { faceAngle_.x, faceAngle_.y + pAngle ,faceAngle_.z };


	//画面上で自機が動くためのmatrix
	pAngleMat.identity();
	pAngleMat.rotateY(pAngle);
	Vector3 viewPos = MathFunc::bVelocity(nowPos, pAngleMat);

	Vector3 playerPos = primaryPos + viewPos;
	playerPos.y = 0.3f;

	gameObject_->SetPosition(playerPos);


}

void FbxPlayer::MoveTarget()
{
	//キー入力があったら
	if (input_->PushKey(DIK_UP) ||
		input_->PushKey(DIK_LEFT) ||
		input_->PushKey(DIK_DOWN) ||
		input_->PushKey(DIK_RIGHT) ||
		input_->StickInput(R_RIGHT) ||
		input_->StickInput(R_LEFT) ||
		input_->StickInput(R_UP) ||
		input_->StickInput(R_DOWN) &&
		isDead_ == false)
	{

		//斜め移動
		float diagonalSp = targetPosMoveSpeed_ * 0.707f;

		//W,Dを押していたら
		if (input_->PushKey(DIK_UP) && input_->PushKey(DIK_RIGHT) || input_->StickInput(R_UP) && input_->StickInput(R_RIGHT))
		{

			
			if (targetPosVelueToAdd_.x <= maxTargetPosVTA_.x) {
				targetPosVelueToAdd_.x += diagonalSp;
			}
			if (targetPosVelueToAdd_.y <= maxTargetPosVTA_.y) {
				targetPosVelueToAdd_.y += diagonalSp;

			}

		}

		//W,Aを押していたら
		else if (input_->PushKey(DIK_UP) && input_->PushKey(DIK_LEFT) || input_->StickInput(R_UP) && input_->StickInput(R_LEFT))
		{


			if (targetPosVelueToAdd_.x >= -maxTargetPosVTA_.x) {
				targetPosVelueToAdd_.x -= diagonalSp;
			}
			if (targetPosVelueToAdd_.y <= maxTargetPosVTA_.y) {
				targetPosVelueToAdd_.y += diagonalSp;

			}


		}

		//S,Dを押していたら
		else if (input_->PushKey(DIK_DOWN) && input_->PushKey(DIK_RIGHT) || input_->StickInput(R_DOWN) && input_->StickInput(R_RIGHT))
		{

			if (targetPosVelueToAdd_.x <= maxTargetPosVTA_.x) {
				targetPosVelueToAdd_.x += diagonalSp;
			}
			if (targetPosVelueToAdd_.y >= -maxTargetPosVTA_.y) {
				targetPosVelueToAdd_.y -= diagonalSp;

			}

		}

		//S,Aを押していたら
		else if (input_->PushKey(DIK_DOWN) && input_->PushKey(DIK_LEFT) || input_->StickInput(R_DOWN) && input_->StickInput(R_LEFT))
		{


			if (targetPosVelueToAdd_.x >= -maxTargetPosVTA_.x) {
				targetPosVelueToAdd_.x -= diagonalSp;
			}
			if (targetPosVelueToAdd_.y >= -maxTargetPosVTA_.y) {
				targetPosVelueToAdd_.y -= diagonalSp;

			}

		}

		//Wを押していたら
		else if (input_->PushKey(DIK_UP) || input_->StickInput(R_UP))
		{
			if (targetPosVelueToAdd_.y <= maxTargetPosVTA_.y) {
				targetPosVelueToAdd_.y += targetPosMoveSpeed_;
			}


		}

		//Sを押していたら
		else if (input_->PushKey(DIK_DOWN) || input_->StickInput(R_DOWN))
		{


			if (targetPosVelueToAdd_.y >= -maxTargetPosVTA_.y) {
				targetPosVelueToAdd_.y -= targetPosMoveSpeed_;
			}

		}

		//Dを押していたら
		else if (input_->PushKey(DIK_RIGHT) || input_->StickInput(R_RIGHT))
		{

			if (targetPosVelueToAdd_.x <= maxTargetPosVTA_.x) {
				targetPosVelueToAdd_.x += targetPosMoveSpeed_;


			}


		}

		//Aを押していたら
		else if (input_->PushKey(DIK_LEFT) || input_->StickInput(R_LEFT))
		{

			if (targetPosVelueToAdd_.x >= -maxTargetPosVTA_.x) {
				targetPosVelueToAdd_.x -= targetPosMoveSpeed_;

			}

		}

	

	}
	else
	{

		//押されていないときの処理
	


	}
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

void FbxPlayer::PColliderUpdate()
{
	if (hitDeley_ > 0) {	//毎フレームヒットを防止
		hitDeley_--;
		isHitByATK_ = false;
	}
	else {
		for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
		{

			if (sphere[i]->GetIsHit() == true &&
				sphere[i]->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_ENEMIEBULLETS
				&& hitDeley_ <= 0) {
				audio_->PlayWave("kuri.wav");
				hitDeley_ = delayCount_;
				SetMaxFramesToMaxAlpha(delayCount_);
				MinusHp(damage_);
				isHitByATK_ = true;
				CreateBulHitParticle(hoverCarObject_->GetPosition());
				break;
			}
			else {
				isHitByATK_ = false;
			}
		}

		for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
			spherePos[i] = gameObject_.get()->bonesMat[i].GetWorldPos();
			sphere[i]->Update();
			coliderPosTest_[i]->SetPosition(sphere[i]->center);
			coliderPosTest_[i]->Update();
		}
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

float FbxPlayer::GetCurrentAlpha()
{
	return currentAlpha_;
}

void FbxPlayer::SetMaxFramesToMaxAlpha(int frame)
{
	maxFramesToMaxAlpha_ = frame / 5;
}

float* FbxPlayer::GetParallelMovePtr()
{
	return &playerParalellMoveVal_;
}

Vector3* FbxPlayer::GetTargetPosVelueToAddPtr()
{
	return &targetPosVelueToAdd_;
}

void FbxPlayer::SetRailTargetPos(Vector3* v)
{
	railTargetPosPtr_ = v;
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

void FbxPlayer::DamageEffectUpdate()
{
	if (maxFramesToMaxAlpha_ > 0) {
		// フレームカウントをインクリメント
		currentDamageFrame_++;

		// 点滅回数を計算
		int blinkCount = currentDamageFrame_ / maxFramesToMaxAlpha_;

		// 奇数回の点滅の場合、アルファ値を上げる
		if (blinkCount % 2 == 1) {
			currentAlpha_ = static_cast<float>(currentDamageFrame_ % maxFramesToMaxAlpha_) / maxFramesToMaxAlpha_;
		}
		else {
			// 偶数回の点滅の場合、アルファ値を下げる
			currentAlpha_ = 1.0f - static_cast<float>(currentDamageFrame_ % maxFramesToMaxAlpha_) / maxFramesToMaxAlpha_;
		}

		if (blinkCount == 3) {
			int a = 0;
			static_cast<void>(a);
		}

		// 点滅回数が5回を超えたらリセット
		if (blinkCount >= 5) {
			currentDamageFrame_ = 0;
			maxFramesToMaxAlpha_ = 0;
			hoverCarObject_->SetRimColor({ 1.f,1.f,1.f,1.f });
		}
		else {
			hoverCarObject_->SetRimColor({ 1.f,1.f,1.f,GetCurrentAlpha() });
		}
	}
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
