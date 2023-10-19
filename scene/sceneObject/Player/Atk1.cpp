#include "Atk1.h"
#include "Ease.h"
#include "FbxPlayer.h"


Atk1::Atk1(PlayerActionManager* pActManager)
{
	_pActManager = pActManager;
}

Atk1::~Atk1()
{
	isAtk_ = false;
}

void Atk1::Initialize(FBXObject3d* gameObject)
{
	gameObject_ = gameObject;
	animNum_ = 1;
	animFlameCT_ = 0;;
	gameObject_->PlayAnimation(animNum_);
	_pActManager->SetNowActNum(ACTION_NUM::atk1);
	isAtk_ = true;
	isNextAtk_ = false;
	isCombo1_ = false;
	isCombo2_ = false;
	atkMovePhase_ = 0;
}

void Atk1::Update(Input* input)
{
	input_ = input;
	//カウントプラス
	animFlameCT_++;
	
	//アニメーション処理
	if (animFlameCT_ > 30)
	{	
		
		//30フレ目でアニメーション終わり
		if (isNextAtk_ == false) {
			gameObject_->AnimFlameInter(animFlameCT_, 80);
			animNum_ = 5;
			gameObject_->PlayAnimation(animNum_);
			isAtk_ = false;
			animFlameCT_ = 0;
		}
		else {
	
			animFlameCT_ = 0;
			animNum_ = 7;
			gameObject_->PlayAnimation(animNum_);
			isNextAtk_ = false;
			isCombo1_ = true;
		}



	}
	

	if (isAtk_ == true && isCombo1_ == false && isCombo2_ == false)
	{
		if (input_->TriggerMouseButton(0) && animFlameCT_ > 8) {
			isNextAtk_ = true;
		}

		//攻撃中の移動処理
		int atkVelCt = animFlameCT_ - 15;
		int ct = abs(atkVelCt);
		float atkVel = static_cast<float>(Ease::InOutQuad(4.0, 0.0, 15, ct));
		Vector3 vel = { 0,0,0.3f };
		vel = MathFunc::bVelocity(vel, gameObject_->wtf_.matWorld_);
		gameObject_->wtf_.translation_ += vel * (4.0f - atkVel);

		
		//何フレーム目に攻撃判定が出るか
		if (animFlameCT_ > 5 && animFlameCT_ < 29) {
			FbxPlayer::SetIsAtkCollide(true);
		}
		else {
			FbxPlayer::SetIsAtkCollide(false);
		}

	}
	else if (isCombo1_ == true  && isCombo2_ == false) {

		//次のコンボにつなげるかどうか
		if (input_->TriggerMouseButton(0) && animFlameCT_ > 8) {
			isNextAtk_ = true;
		}

		//アニメーションと初期化処理
		gameObject_->AnimFlameInter(animFlameCT_, 40);
		if (animFlameCT_ < 40) {	//40フレームまでアニメーション
			
		}
		else {
			if (isNextAtk_ == false) {	//次の行動に遷移しない時
				gameObject_->AnimFlameInter(animFlameCT_, 80);
				animNum_ = 5;
				gameObject_->PlayAnimation(animNum_);
				isAtk_ = false;
				isCombo1_ = false;
				animFlameCT_ = 0;
			}
			else {	//次の攻撃に遷移
				
				animFlameCT_ = 0;
				atkMovePhase_ = 0;
				isNextAtk_ = false;
				

			}

		}

		//攻撃中の移動処理
		int atkVelCt = animFlameCT_ - 20;
		int ct = abs(atkVelCt);
		float atkVel = static_cast<float>(Ease::InOutQuad(4.0, 0.0, 20, ct));
		float atkXVel;
		if (animFlameCT_ < 20) {
			atkXVel = 0.07f;
		}
		else {
			atkXVel = -0.07f;
		}

		Vector3 vel = { atkXVel,0.0f,0.04f };
		vel = MathFunc::bVelocity(vel, gameObject_->wtf_.matWorld_);
		gameObject_->wtf_.translation_ += vel * (4.0f - atkVel);


		//何フレーム目に攻撃判定が出るか
		if (animFlameCT_ > 5 && animFlameCT_ < 15 || animFlameCT_ > 25 && animFlameCT_ < 35) {
			FbxPlayer::SetIsAtkCollide(true);
		}
		else {
			FbxPlayer::SetIsAtkCollide(false);
		}

	}
	

	if (isAtk_ == false) {
		_pActManager->SetNowActNum(ACTION_NUM::move);
	}
}

void Atk1::Draw()
{
}
