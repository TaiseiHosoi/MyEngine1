#include "FloatingEnemy.h"
#include "Ease.h"

FloatingEnemy::FloatingEnemy()
{
}

FloatingEnemy::~FloatingEnemy()
{
}

void FloatingEnemy::Initialize(Mesh* model)
{
	model_ = model;
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(true);
	object3d_->SetModel(model_);
	object3d_->SetScale({ enemyScale_,enemyScale_,enemyScale_ });
	object3d_->SetPosition(object3d_->GetPosition());
	nowPhase_ = MOVE_PHASE::forward;	//自機の挙動が何から始まるか
	advancedValue_ = 0.0f;

	//当たり判定初期化
	sphere_ = std::make_unique<SphereCollider>();
	CollisionManager::GetInstance()->AddCollider(sphere_.get());
	sphere_->SetAttribute(COLLISION_ATTR_ENEMIES);
	sphere_->SetBasisPos(&colliderPos_);
	sphere_->SetRadius(object3d_->worldTransform.scale_.x);
	sphere_->Update();

	collider_ = CollisionManager::GetInstance();

	//ステータス初期化
	state_.hp_ = 1;
	state_.isDead_ = false;
	state_.isAtk_ = false;

}

void FloatingEnemy::Update()
{

	//弾リムーヴ処理
	bullets_.remove_if([](std::unique_ptr<EnemyNormalBullet>& bullet) {
		return bullet->ReturnIsDead();
		});

	// レールカメラ情報から現在の進行度を求める
	if (railCameraInfo_ != nullptr) {
		primaryPos_ = MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate);	//ゲーム進行度合を一点に定めた場合

		object3d_->worldTransform.translation_ =	//挙動
			MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate + advancedValue_ + moveDifferenceValue_);
	}

	// パターン行動
	if (oldFlamePhase_ != nowPhase_) {
		moveCount_ = 0;
	}
	oldFlamePhase_ = nowPhase_;

	if (nowPhase_ == MOVE_PHASE::none) {

	}
	else if (nowPhase_ == MOVE_PHASE::forward) {
		Forward();

	}
	else if (nowPhase_ == MOVE_PHASE::turn) {
		Turn();
	}
	else if (nowPhase_ == MOVE_PHASE::atk) {
		Atk();
		rotMode_ = ROT_MODE::toPlayer;
	}

	// 回転角,位置計算
	Vector3 nowOffset;
	if (railCameraInfo_ != nullptr) {
		//進行上の向いている方向(顔の向きではない)
		directionLoot_ = MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate + offsetBattlePosTimeRate_)
			- MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate);
		directionLoot_.nomalize();

		float dirAngle = {};
		dirAngle = atan2(directionLoot_.x, directionLoot_.z);


		nowOffset += offsetPos_;
		nowOffset = MathFunc::RotateVecAngleY(nowOffset, dirAngle);

		//値をオブジェクトに挿入
		nowOffset.y = 0;	// yの値は別計算
		object3d_->worldTransform.translation_ += nowOffset;
		object3d_->worldTransform.translation_.y = posY_;

		//ワールド上の自機の回転量yを求める
		object3d_->worldTransform.rotation_.y = adjustFAngle_ + dirAngle;

	}

	// 当たり判定計算
	if (sphere_->GetIsHit() == true) {
		if (sphere_->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_ALLIESBULLETS) {
			state_.hp_--;
		}
	}

	if (state_.hp_ <= 0) {
		state_.isDead_ = true;
	}

	// コライダー更新
	colliderPos_ = object3d_->worldTransform.matWorld_.GetWorldPos();
	sphere_->Update();

	//object3d更新
	object3d_->Update();

	for (std::unique_ptr<EnemyNormalBullet>& rapidBullet : bullets_) {
		rapidBullet->Update();
	}

	//ImGui::Begin("FloatingEnemy");
	//ImGui::InputFloat3("translation", &object3d_->worldTransform.translation_.x);
	//ImGui::End();


}

void FloatingEnemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	object3d_->Draw(cmdList);

	for (std::unique_ptr<EnemyNormalBullet>& rapidBullet : bullets_) {
		rapidBullet->Draw(cmdList);
	}
}

void FloatingEnemy::SetPlayerWorldTransform(WorldTransform* worldTransform)
{
	playerWorldTransform = worldTransform;
}

EnemyState* FloatingEnemy::GetState()
{
	return &state_;
}

void FloatingEnemy::Forward()
{
	int maxTime = maxFowardTime_;
	if (moveCount_ < maxTime) {
		moveCount_++;
	}
	else {

		nowPhase_ = MOVE_PHASE::turn;
	}


	advancedValue_ = Ease::LinearEaseOutEasing(spownBattlePosTimeRate_, offsetBattlePosTimeRate_, moveCount_, maxTime, forwardEaseStrength);
	//ImGui::Begin("enemy");
	//ImGui::InputInt("count",&moveCount_);
	//ImGui::InputFloat("y", &object3d_->worldTransform.translation_.y);
	//ImGui::End();

}

void FloatingEnemy::Turn()
{
	int maxTime = maxTurnTime_;
	if (turnCount_ < maxTime) {
		turnCount_++;
	}
	else {
		nowPhase_ = MOVE_PHASE::atk;
	}

	adjustFAngle_ = Ease::LinierEaseInOutEasing(minAdjustFAngle_, maxAdjustFAngle_, turnCount_, maxTime, turnEaseStrength) * MathFunc::PI / 180.f;
}

void FloatingEnemy::Atk()
{
	// カウント処理
	if (atkMoveCount_ <= atkMaxMoveCount_) {
		atkMoveCount_++;
	}
	else {
		nowPhase_ = MOVE_PHASE::none;
		isDead_ = true;
	}

	// 移動処理
	if (atkMoveCount_ < atkMaxFowardMoveCount_) {
		moveDifferenceValue_ = Ease::LinierEaseInOutEasing(0, maxMoveDifferencePosTimeRate_, atkMoveCount_, atkMaxFowardMoveCount_, forwardEaseStrength);
	}
	else if (atkMoveCount_ >= atkMaxFowardMoveCount_ && atkMoveCount_ < atkMaxMoveCount_) {
		moveDifferenceValue_ = Ease::LinearEasing(maxMoveDifferencePosTimeRate_, minMoveDifferencePosTimeRate_, atkMoveCount_ - atkMaxFowardMoveCount_, atkMaxMoveCount_ - atkMaxFowardMoveCount_, forwardEaseStrength);
	}

	ShotBullet();


}

void FloatingEnemy::ShotBullet()
{
	nowShotDelay_++;
	if (nowShotDelay_ > bulletShotDelay_) {
		nowShotDelay_ = 0;

		AddBullet();

	}


}

void FloatingEnemy::AddBullet()
{
	std::unique_ptr<EnemyNormalBullet> newRapidBullet;
	newRapidBullet = std::make_unique<EnemyNormalBullet>();

	Vector3 bulletRot = object3d_->worldTransform.rotation_;
	bulletRot.z -= directlyBelow_;
	newRapidBullet->Initialize(bulletModel_, object3d_->worldTransform.matWorld_.GetWorldPos(), bulletRot);
	bullets_.push_back(std::move(newRapidBullet));
}

bool FloatingEnemy::compultionTrue()
{
	return true;
}

void FloatingEnemy::SetBulletModel(Mesh* model)
{
	bulletModel_ = model;
}
