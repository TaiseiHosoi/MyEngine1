#include "FbxPlayer.h"
#include "Ease.h"
#include "GameCamera.h"
#include <random>
#include "HitStopManager.h"
#include"Move.h"
#include"Atk1.h"


#include"RaycastHit.h"



int FbxPlayer::hp_ = 100;
bool FbxPlayer::isAtkCollide_ = false;
bool FbxPlayer::isGuardCollide_ = false;

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
	hoverCarObject_->SetScale({1.f,1.f,1.f});


	SPHERE_COLISSION_NUM = static_cast<int>(gameObject_->GetBonesMatPtr()->size());
	sphere_.resize(SPHERE_COLISSION_NUM);
	spherePos_.resize(SPHERE_COLISSION_NUM);
	gameObject_.get()->isBonesWorldMatCalc_ = true;
	coliderPosTest_.resize(SPHERE_COLISSION_NUM);
	hpModel_ = Mesh::LoadFormOBJ("cube", false);

	//当たり判定初期化
	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		sphere_[i] = new SphereCollider;
		CollisionManager::GetInstance()->AddCollider(sphere_[i]);
		spherePos_[i] = gameObject_.get()->bonesMat_[i].GetWorldPos();
		sphere_[i]->SetBasisPos(&spherePos_[i]);
		sphere_[i]->SetRadius(1.0f);
		sphere_[i]->SetAttribute(COLLISION_ATTR_ALLIES);
		sphere_[i]->Update();
		//test
		coliderPosTest_[i] = Object3d::Create();
		coliderPosTest_[i]->SetModel(hpModel_.get());
		coliderPosTest_[i]->SetPosition(sphere_[i]->center);
		coliderPosTest_[i]->SetScale({ sphere_[i]->GetRadius(),sphere_[i]->GetRadius() ,sphere_[i]->GetRadius() });
		coliderPosTest_[i]->SetRotate({ 0,0,0 });
		coliderPosTest_[i]->Update();

	}

	//ヒットポイント
	hp_ = 100;

	hpObject_ = Object3d::Create();
	hpObject_->SetModel(hpModel_.get());
	hpObject_->SetScale({ static_cast<float>(hp_) * 0.04f,0.1f,0.02f });
	hpObject_->SetPosition({ gameObject_.get()->GetWorldTransform().translation_.x,
		gameObject_.get()->GetWorldTransform().translation_.y + 4.0f,
		gameObject_.get()->GetWorldTransform().translation_.z });
	Matrix4 invViewPro = MathFunc::MakeInverse(&hpObject_.get()->camera_->GetViewProjectionMatrix());
	float yaw = atan2f(-invViewPro.m[2][0], sqrtf(invViewPro.m[2][1] * invViewPro.m[2][1] + invViewPro.m[2][2] * invViewPro.m[2][2]));
	hpObject_->SetRotate({ 0,yaw * 3.14f,0 });
	hpObject_->Update();

	//パーティクル
	particle_ = std::make_unique<ParticleManager>();
	particle_->Initialize();
	particle_->LoadTexture("effect.png");
	particle_->Update();

	// 現在時刻を取得してシード値とする
	std::srand(static_cast<int>(std::time(nullptr)));

	//初期化
	FbxPlayer::isAtkCollide_ = false;
	isGuardCollide_ = false;
	
	
	reticle_.Initialize(gameObject_->GetWorldTransformPtr());

	//弾
	bulletModel_ = Mesh::LoadFormOBJ("cube",true);

	
	

}

void FbxPlayer::Update()
{

	//デスフラグの立った弾を削除
	
		rapidBullets_.remove_if([](std::unique_ptr<PlayerRapidBullet>& bullet) {
			return bullet->ReturnIsDead();
			});
	

	if (isHitStop_ == false)
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

		// レイキャストによるロックオン登録
		if (CollisionManager::GetInstance()->Raycast(ray, &raycast, 120.f)) {

			if (raycast.collider_->GetAttribute() == COLLISION_ATTR_ENEMIES && raycast.object_ != nullptr) {
				// ロックオン処理
				PRockTarget newRockTarget;
				rockTargets_.push_back(newRockTarget);
				int nowRockNum = static_cast<int>(rockTargets_.size()) - 1;
				
				rockTargets_[nowRockNum].targetWtfPtr_ = raycast.collider_->GetObject3d()->GetWorldTransformPtr();
				rockTargets_[nowRockNum].isRockOn_ = true;
				
				// そのロックオンによって弾発射
				std::unique_ptr<PlayerHomingBullet> newHomingBullet;
				newHomingBullet = std::make_unique<PlayerHomingBullet>();
				newHomingBullet->Initialize(bulletModel_.get(), gameObject_->GetPosition(), gameObject_->GetRotate());
				newHomingBullet->SetTargerPtr(rockTargets_[nowRockNum].targetWtfPtr_);
				homingBullets_.push_back(std::move(newHomingBullet));
				
				
				
			}
		}

		// 速射弾生成
		if (input_->TriggerMouseButton(0)) {
			std::unique_ptr<PlayerRapidBullet> newRapidBullet;
			newRapidBullet = std::make_unique<PlayerRapidBullet>();
			newRapidBullet->Initialize(bulletModel_.get(), gameObject_->GetPosition(), gameObject_->GetRotate());
			rapidBullets_.push_back(std::move(newRapidBullet));
		}

		for (std::unique_ptr<PlayerRapidBullet>& rapidBullet : rapidBullets_) {
			
			if (rapidBullet->GetSphereCollider()->GetIsHit() == true) {
				if (rapidBullet->GetSphereCollider()->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_ENEMIES) {
					rapidBullet->SetIsDead(true);
					particle_->RandParticle(rapidBullet->GetSphereCollider()->GetCollisionInfo().inter_);
				}
			}
		}


		//弾更新
		for (int i = 0; i < homingBullets_.size(); i++) {
			homingBullets_[i]->Update();
		}
		for (std::unique_ptr<PlayerRapidBullet>& rapidBullet : rapidBullets_) {
			rapidBullet->Update();
		}


		//y固定
		float yPos = 2.0f;
		hoverCarObject_->worldTransform.translation_.y = yPos;
		gameObject_->wtf_.translation_.y = yPos;
		
		//更新
		PColliderUpdate();

		particle_->Update();
		gameObject_->Update();
		hoverCarObject_->Update();

		count_++;
		

#pragma region hp
		hpObject_->SetScale({ static_cast<float>(hp_) * 0.04f,0.1f,0.02f });
		hpObject_->SetPosition({ gameObject_.get()->GetWorldTransform().translation_.x,
			gameObject_.get()->GetWorldTransform().translation_.y + 4.0f,
			gameObject_.get()->GetWorldTransform().translation_.z });
		Matrix4 invViewPro = MathFunc::MakeInverse(&hpObject_.get()->camera_->GetViewMatrix());
		float yaw = atan2f(-invViewPro.m[3][1], sqrtf(invViewPro.m[3][2] * invViewPro.m[3][2] + invViewPro.m[3][3] * invViewPro.m[3][3]));
		hpObject_->SetRotate({ 0,yaw,0 });
		hpObject_->Update();
#pragma endregion hp

		if (oldAnimCT_ != animCT_)
		{
			gameObject_.get()->PlayAnimation(animCT_);
		}



	} //ヒットストップ
	
	
	reticle_.Update();
	//ImGui::Begin("Player");
	//ImGui::InputFloat3("PlayerPos", &gameObject_->wtf.matWorld_.m[3][0]);
	//ImGui::End();
}



void FbxPlayer::Draw(ID3D12GraphicsCommandList* cmdList)
{

	//gameObject_->Draw(cmdList);

	hoverCarObject_->Draw(cmdList);

	particle_->Draw(cmdList);
	
	reticle_.Draw(cmdList);

	for (int i = 0; i < homingBullets_.size(); i++) {
		homingBullets_[i]->Draw(cmdList);
	}
	for (std::unique_ptr<PlayerRapidBullet>& rapidBullet : rapidBullets_) {
		rapidBullet->Draw(cmdList);
	}

}

void FbxPlayer::CreateParticle()
{
	
		for (int i = 0; i < 10; i++) {
			
			const float rnd_pos = 2.0f;
			Vector3 pos{};
			pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

			const float rnd_vel = 0.3f;
			Vector3 vel{};
			vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

			Vector3 acc{};
			const float rnd_acc = 0.01f;
			acc.y = -(float)rand() / RAND_MAX * rnd_acc;

			//追加
			particle_->Add(60, pos, vel, acc);
		}
	
}

void FbxPlayer::minusHp(int damage)
{
	hp_ -= damage;
}

void FbxPlayer::Move()
{
	

	Vector3 primaryNorm = MathFunc::TangentSplinePosition( railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate+0.005f)
		- MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate);
	primaryNorm.nomalize();

	float pAngle = atan2f(primaryNorm.x, primaryNorm.z);

	Vector3 primaryPos = MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate + 0.002f) + primaryNorm * 10.0f;



	//キー入力があったら
	if (input_->PushKey(DIK_W) ||
		input_->PushKey(DIK_A) ||
		input_->PushKey(DIK_S) ||
		input_->PushKey(DIK_D))
	{


		//Z軸方向にの速度を入れる
		velocity_ = { 0 , 0 , 0.1f };

		float kDiagonalSpeed = kMoveSpeed_ * 0.707f;

		//W,Dを押していたら
		if (input_->PushKey(DIK_W) && input_->PushKey(DIK_D))
		{
			nowPos_.x += kDiagonalSpeed;
			//nowPos.y += kDiagonalSpeed;

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

			nowPos_.x -= kDiagonalSpeed;
			//nowPos.y += kDiagonalSpeed;
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

			nowPos_.x += kDiagonalSpeed;


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
			nowPos_.x -= kDiagonalSpeed;

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
			nowPos_.x += kMoveSpeed_;
			

			if (faceAngle_.y <= faceMaxAngle_) {
				faceAngle_.y += faceRotSpeed_;
				

			}


		}

		//Aを押していたら
		else if (input_->PushKey(DIK_A))
		{
			nowPos_.x -= kMoveSpeed_;
			if (faceAngle_.y >= -faceMaxAngle_) {
				faceAngle_.y -= faceRotSpeed_;

			}

		}

		//押されていないときの処理

		if (input_->PushKey(DIK_A) != 1 && input_->PushKey(DIK_D) != 1) {
			if (faceAngle_.y > 0.02f) {

				faceAngle_.y -= returnRotSpeed_;

			}
			else if (faceAngle_.y < -0.02f) {
				faceAngle_.y += returnRotSpeed_;

			}

		}

		if (input_->PushKey(DIK_W) != 1 && input_->PushKey(DIK_S) != 1) {
			if (faceAngle_.x > 0.02f) {
				faceAngle_.x -= returnRotSpeed_;
			}
			else if (faceAngle_.x < -0.02f) {
				faceAngle_.x += returnRotSpeed_;
			}

		}

	}
	else
	{

		//押されていないときの処理
		if (faceAngle_.x > 0.02f) {
			faceAngle_.x -= returnRotSpeed_;
		}
		else if (faceAngle_.x < -0.02f) {
			faceAngle_.x += returnRotSpeed_;
		}

		if (faceAngle_.y > 0.02f) {
			faceAngle_.y -= returnRotSpeed_;

		}
		else if (faceAngle_.y < -0.02f) {
			faceAngle_.y += returnRotSpeed_;

		}
	}

	gameObject_->wtf_.rotation_ = { faceAngle_.x, faceAngle_.y + pAngle ,faceAngle_.z };

	//画面上で自機が動くためのmatrix
	pAngleMat_.identity();
	pAngleMat_.rotateY(pAngle);
	Vector3 viewPos = MathFunc::bVelocity(nowPos_, pAngleMat_);

	Vector3 playerPos = primaryPos + viewPos;
	playerPos.y = 0.3f;

	gameObject_->SetPosition(playerPos);

	
}

void FbxPlayer::BulletShot()
{
}

FBXObject3d* FbxPlayer::GetObject3d()
{
	return gameObject_.get();
}

bool FbxPlayer::GetIsAtkCollide()
{
	return FbxPlayer::isAtkCollide_;
}

void FbxPlayer::SetIsAtkCollide(bool isAtkCollideArg)
{
	FbxPlayer::isAtkCollide_ = isAtkCollideArg;
}

bool FbxPlayer::GetIsGuardCollide()
{
	return isGuardCollide_;
}

void FbxPlayer::SetIsGuardCollide(bool isGuardCollideArg)
{
	FbxPlayer::isGuardCollide_ = isGuardCollideArg;
}

int FbxPlayer::GetHp()
{
	return FbxPlayer::hp_;
}

void FbxPlayer::SetHp(int hpArg)
{
	FbxPlayer::hp_ = hpArg;
}

void FbxPlayer::PColliderUpdate()
{
	if (hitDeley_ > 0) {	//毎フレームヒットを防止
		hitDeley_--;
	}


	for (int i = 0; i < SPHERE_COLISSION_NUM; i++)
	{
		if (isAtkCollide_ == true && hitDeley_ <= 0 && sphere_[i]->GetIsHit() == true) {

			if (sphere_[i]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_ENEMIES) {

				audio_->PlayWave("kuri.wav");
				//Boss::minusHp(1);
				hitDeley_ = 4;
				particle_->RandParticle(sphere_[i]->GetCollisionInfo().inter_);
				HitStopManager::GetInstance()->SetHitStop(&isHitStop_, 2);
				break;
			}

		}
		if (sphere_[i]->GetIsHit() == true &&
			sphere_[i]->GetCollisionInfo().collider_->GetAttribute() == COLLISION_ATTR_ENEMIEBULLETS
			&& hitDeley_ <= 0) {
			audio_->PlayWave("kuri.wav");
			hitDeley_ = 6;
			this->hp_ -= 2;
			HitStopManager::GetInstance()->SetHitStop(&isHitStop_, 2);
			break;
		}
	}

	for (int i = 0; i < SPHERE_COLISSION_NUM; i++) {
		spherePos_[i] = gameObject_.get()->bonesMat_[i].GetWorldPos();
		sphere_[i]->Update();
		coliderPosTest_[i]->SetPosition(sphere_[i]->center);
		coliderPosTest_[i]->Update();
	}



}


Vector3 FbxPlayer::GetNowFaceAngle()
{

	Vector3 nowVelocity_ = MathFunc::MatVector(velocity_, gameObject_->wtf_.matWorld_);
	return nowVelocity_;
}

void FbxPlayer::SetRailCameraInfo(RailCameraInfo* info)
{
	railCameraInfo_ = info;
}
