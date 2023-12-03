#include "OjamaFence.h"

#include "Ease.h"

OjamaFence::OjamaFence()
{
}

OjamaFence::~OjamaFence()
{
	collider_->RemoveCollider(sphere_.get());
}

void OjamaFence::Initialize(Mesh* model)
{
	model_ = model;
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(true);
	object3d_->SetModel(model_);
	object3d_->SetScale({ enemyScale_,enemyScale_,enemyScale_ });
	object3d_->SetPosition(object3d_->GetPosition());
	advancedValue_ = 0.0f;

	//当たり判定初期化
	sphere_ = std::make_unique<SphereCollider>();
	CollisionManager::GetInstance()->AddCollider(sphere_.get());
	sphere_->SetAttribute(COLLISION_ATTR_ENEMIEBULLETS);
	sphere_->SetBasisPos(&colliderPos_);
	sphere_->SetRadius(object3d_->worldTransform.scale_.x);
	sphere_->Update();

	collider_ = CollisionManager::GetInstance();

	//ステータス初期化
	state_.hp_ = 1;
	state_.isDead_ = false;
	state_.isAtk_ = false;

	//弾情報初期化


}

void OjamaFence::Update()
{

	


	// 当たり判定計算
	if (sphere_->GetIsHit() == true) {
		if (sphere_->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_ALLIES) {
			state_.hp_--;
		}
	}

	if (state_.hp_ <= 0) {
		isDeathAction_ = true;
	}

	// 死亡アクション
	DeadAction();
	

	// コライダー更新
	colliderPos_ = object3d_->worldTransform.matWorld_.GetWorldPos();
	sphere_->Update();


	//object3d更新
	object3d_->Update();



	//ImGui::Begin("OjamaFence");
	//ImGui::InputFloat3("translation", &object3d_->worldTransform.translation_.x);
	//ImGui::End();


}

void OjamaFence::Draw(ID3D12GraphicsCommandList* cmdList)
{
	object3d_->Draw(cmdList);
	
}

void OjamaFence::SetPlayerWorldTransform(WorldTransform* worldTransform)
{
	playerWorldTransform = worldTransform;
}

EnemyState* OjamaFence::GetState()
{
	return &state_;
}







void OjamaFence::DeadAction()
{

	if (isDeathAction_ == true) {
		deathActionCount_++;
		if (deathActionCount_ >= maxDeathActionCount_) {
			state_.isDead_ = true;
		}


		if (object3d_->worldTransform.translation_.y < lowestPosY_) {
			nowFallSpeed_ = offsetBoundSpeed_;
			posY_ = 0;
		}
		else {
			nowFallSpeed_ -= fallSpeedVel_;
		}
		posY_ += nowFallSpeed_;	//y変動
		object3d_->GetWorldTransformPtr()->translation_.y = posY_;
		object3d_->GetWorldTransformPtr()->rotation_.x += deathActionRotateVel_;
		nowSubtractTimeRate_ += subtractTimeRateVel_;	//タイムレートが落ちてくる

	}


}



bool OjamaFence::compultionTrue()
{
	return true;
}

void OjamaFence::SetWorldTransform(const WorldTransform& arg)
{
	object3d_->worldTransform = arg;
}




