#include "FbxPlayer.h"
#include "Ease.h"
#include "GameCamera.h"
#include <random>
#include "HitStopManager.h"
//#include "Boss.h"	//苦肉の策
//PlayerAction
#include"Move.h"
#include"Atk1.h"
#include"Guard.h"
#include"CounterAtk.h"
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
	hoverCarObject_->SetScale({1.f,1.f,1.f});


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
		sphere[i]->SetRadius(1.0f);
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
	hp = 100;

	hpObject_ = Object3d::Create();
	hpObject_->SetModel(hpModel_.get());
	hpObject_->SetScale({ static_cast<float>(hp) * 0.04f,0.1f,0.02f });
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
	FbxPlayer::isAtkCollide = false;
	isGuardCollide = false;
	
	
	reticle_.Initialize(gameObject_->GetWorldTransformPtr());

	//弾
	bulletModel_ = Mesh::LoadFormOBJ("cube",true);
	

}

void FbxPlayer::Update()
{

	if (isHitStop == false)
	{

		cameraAngle_.y =
			atan2(gameObject_.get()->GetCamera().GetTarget().x - gameObject_.get()->GetCamera().GetEye().x,
				gameObject_.get()->GetCamera().GetTarget().z - gameObject_.get()->GetCamera().GetEye().z);

		//移動処理
		Move();

		//弾発射
		Ray ray;
		ray.start = gameObject_->GetCamera().GetEye();
		ray.dir = reticle_.GetFarReticleObjPtr()->GetPosition() - ray.start;
		ray.dir.nomalize();

		RaycastHit raycastHit;

		// レイキャストによるロックオン登録
		if (CollisionManager::GetInstance()->Raycast(ray, &raycastHit) == true) {
			if (raycastHit.collider->GetAttribute() == COLLISION_ATTR_ENEMIES && raycastHit.object != nullptr) {
				// ロックオン処理
				PRockTarget newRockTarget;
				rockTargets_.push_back(newRockTarget);
				int nowRockNum = static_cast<int>(rockTargets_.size()) - 1;
				rockTargets_[nowRockNum].targetWtfPtr = raycastHit.collider->GetObject3d()->GetWorldTransformPtr();
				rockTargets_[nowRockNum].isRockOn = true;
				
				// そのロックオンによって弾発射
				std::unique_ptr<PlayerHomingBullet> newBullet;
				newBullet = std::make_unique<PlayerHomingBullet>();
				newBullet->Initialize(bulletModel_.get(), gameObject_->GetPosition(), { 0,0,0 });
				newBullet->SetTargerPtr(rockTargets_[nowRockNum].targetWtfPtr);
				bullets_.push_back(std::move(newBullet));
				

				
			}
		}



		//更新
		hoverCarObject_->SetPosition(gameObject_->GetPosition());
		hoverCarObject_->SetRotate(gameObject_->GetRotate());
		for (int i = 0; i < bullets_.size(); i++) {
			bullets_[i]->Update();
		}
		
		PColliderUpdate();

		particle_->Update();
		gameObject_->Update();
		hoverCarObject_->Update();
		count++;
		

#pragma region hp
		hpObject_->SetScale({ static_cast<float>(hp) * 0.04f,0.1f,0.02f });
		hpObject_->SetPosition({ gameObject_.get()->GetWorldTransform().translation_.x,
			gameObject_.get()->GetWorldTransform().translation_.y + 4.0f,
			gameObject_.get()->GetWorldTransform().translation_.z });
		Matrix4 invViewPro = MathFunc::MakeInverse(&hpObject_.get()->camera_->GetViewMatrix());
		float yaw = atan2f(-invViewPro.m[3][1], sqrtf(invViewPro.m[3][2] * invViewPro.m[3][2] + invViewPro.m[3][3] * invViewPro.m[3][3]));
		hpObject_->SetRotate({ 0,yaw,0 });
		hpObject_->Update();
#pragma endregion hp

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

	hoverCarObject_->Draw(cmdList);

	particle_->Draw(cmdList);
	
	reticle_.Draw(cmdList);

	for (int i = 0; i < bullets_.size(); i++) {
		bullets_[i]->Draw(cmdList);
	}

}

void FbxPlayer::minusHp(int damage)
{
	hp -= damage;
}

void FbxPlayer::Move()
{
	

	Vector3 cameraNorm = gameObject_->GetCamera().GetTarget() - gameObject_->GetCamera().GetEye();
	cameraNorm.nomalize();
	float pAngle = atan2f(cameraNorm.x, cameraNorm.z);

	Vector3 primaryPos = gameObject_->GetCamera().GetEye() + cameraNorm * 20.0f;



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
			nowPos.x += kDiagonalSpeed;
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

			nowPos.x -= kDiagonalSpeed;
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

FBXObject3d* FbxPlayer::GetObject3d()
{
	return gameObject_.get();
}

bool FbxPlayer::GetIsAtkCollide()
{
	return FbxPlayer::isAtkCollide;
}

void FbxPlayer::SetIsAtkCollide(bool isAtkCollide)
{
	FbxPlayer::isAtkCollide = isAtkCollide;
}

bool FbxPlayer::GetIsGuardCollide()
{
	return isGuardCollide;
}

void FbxPlayer::SetIsGuardCollide(bool isGuardCollide)
{
	FbxPlayer::isGuardCollide = isGuardCollide;
}

int FbxPlayer::GetHp()
{
	return FbxPlayer::hp;
}

void FbxPlayer::SetHp(int hp)
{
	FbxPlayer::hp = hp;
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
				//Boss::minusHp(1);
				hitDeley = 4;
				particle_->RandParticle(sphere[i]->GetCollisionInfo().inter);
				HitStopManager::GetInstance()->SetHitStop(&isHitStop, 2);
				break;
			}

		}
		if (sphere[i]->GetIsHit() == true &&
			sphere[i]->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_ENEMIEBULLETS
			&& hitDeley <= 0) {
			audio_->PlayWave("kuri.wav");
			hitDeley = 6;
			this->hp -= 2;
			HitStopManager::GetInstance()->SetHitStop(&isHitStop, 2);
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


Vector3 FbxPlayer::GetNowFaceAngle()
{

	Vector3 nowVelocity_ = MathFunc::MatVector(velocity_, gameObject_->wtf.matWorld_);
	return nowVelocity_;
}
