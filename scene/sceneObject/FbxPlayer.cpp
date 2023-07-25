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
	gameObject_->SetScale({ 4,4,4 });
	gameObject_->SetPosition({ 0,15.f,0 });
	gameObject_->Update();

	hoverCarModel_ = Mesh::LoadFormOBJ("hoverCar", false);
	hoverCarObject_ = Object3d::Create();
	hoverCarObject_->SetModel(hoverCarModel_.get());
	hoverCarObject_->SetPosition(gameObject_->GetPosition());
	hoverCarObject_->SetRotate(gameObject_->GetRotate());


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

	//行動マネージャー
	pActManager_ = std::make_unique<PlayerActionManager>();
	pActManager_->ColliderInitialize(&sphere, SPHERE_COLISSION_NUM);
	pActManager_->PActionInitialize(gameObject_.get());

	reticle_.Initialize(gameObject_->GetWorldTransform());
	

}

void FbxPlayer::Update()
{

	if (isHitStop == false)
	{

		cameraAngle_.y =
			atan2(gameObject_.get()->GetCamera().GetTarget().x - gameObject_.get()->GetCamera().GetEye().x,
				gameObject_.get()->GetCamera().GetTarget().z - gameObject_.get()->GetCamera().GetEye().z);


		///-----行動マネージャ-----///
		
		

		if (input_->TriggerMouseButton(1) && pActManager_->GetNowActNum() == ACTION_NUM::move) {
			pActManager_->SetNowActNum(ACTION_NUM::guard);
		}

		//更新
		pActManager_->ActionUpdate(input_);

		//前フレームとAction_Numが違ったら行動代入
		if (oldPActNum_ != pActManager_->GetNowActNum()) {
			if (pActManager_->GetNowActNum() == ACTION_NUM::move) {
				pActManager_->ChangeAction(new Move(pActManager_.get()));
			}
			else if (pActManager_->GetNowActNum() == ACTION_NUM::atk1) {
				pActManager_->ChangeAction(new Atk1(pActManager_.get()));
			}
			else if (pActManager_->GetNowActNum() == ACTION_NUM::guard) {
				pActManager_->ChangeAction(new Guard(pActManager_.get()));
			}
			else if (pActManager_->GetNowActNum() == ACTION_NUM::counter) {
				pActManager_->ChangeAction(new CounterAtk(pActManager_.get()));
			}else{}
			
		}

		hoverCarObject_->SetPosition(gameObject_->GetPosition());
		hoverCarObject_->SetRotate(gameObject_->GetRotate());
		
		PColliderUpdate();

		particle_->Update();
		gameObject_->Update();
		hoverCarObject_->Update();
		count++;
		ImGui::Begin("pRotY");
		ImGui::InputFloat3("rocalPos", &gameObject_->wtf.translation_.x);
		ImGui::InputFloat("rocalRoty", &gameObject_->wtf.rotation_.y);
		ImGui::InputFloat("matRoty", &gameObject_->wtf.matWorld_.m[0][2]);
		ImGui::End();

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



	oldPActNum_ = pActManager_->GetNowActNum();



	} //ヒットストップ

	reticle_.nierReticleO_->worldTransform.parent_ = &gameObject_->wtf;
	reticle_.farReticleO_->worldTransform.parent_ = &gameObject_->wtf;
	reticle_.Update();

}



void FbxPlayer::Draw(ID3D12GraphicsCommandList* cmdList)
{

	//gameObject_->Draw(cmdList);

	hoverCarObject_->Draw(cmdList);

	particle_->Draw(cmdList);
	
	reticle_.Draw(cmdList);

}

void FbxPlayer::minusHp(int damage)
{
	hp -= damage;
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