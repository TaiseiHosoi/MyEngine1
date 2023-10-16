#include "Atk1.h"
#include "Ease.h"
#include "FbxPlayer.h"


Atk1::Atk1(PlayerActionManager* pActManager)
{
	_pActManager = pActManager;
}

Atk1::~Atk1()
{
	isAtk = false;
}

void Atk1::Initialize(FBXObject3d* gameObject)
{
	gameObject_ = gameObject;
	animNum = 1;
	animFlameCT = 0;;
	gameObject_->PlayAnimation(animNum);
	_pActManager->SetNowActNum(ACTION_NUM::atk1);
	isAtk = true;
	isNextAtk = false;
	isCombo1 = false;
	isCombo2 = false;
	atkMovePhase = 0;
}

void Atk1::Update(Input* input)
{
	input_ = input;
	//カウントプラス
	animFlameCT++;
	
	//アニメーション処理
	if (animFlameCT > 30)
	{	
		
		//30フレ目でアニメーション終わり
		if (isNextAtk == false) {
			gameObject_->AnimFlameInter(animFlameCT, 80);
			animNum = 5;
			gameObject_->PlayAnimation(animNum);
			isAtk = false;
			animFlameCT = 0;
		}
		else {
	
			animFlameCT = 0;
			animNum = 7;
			gameObject_->PlayAnimation(animNum);
			isNextAtk = false;
			isCombo1 = true;
		}



	}
	

	if (isAtk == true && isCombo1 == false && isCombo2 == false)
	{
		if (input_->TriggerMouseButton(0) && animFlameCT > 8) {
			isNextAtk = true;
		}

		//攻撃中の移動処理
		int atkVelCt = animFlameCT - 15;
		int ct = abs(atkVelCt);
		float atkVel = static_cast<float>(Ease::InOutQuad(4.0, 0.0, 15, ct));
		Vector3 vel = { 0,0,0.3f };
		vel = MathFunc::bVelocity(vel, gameObject_->wtf.matWorld_);
		gameObject_->wtf.translation_ += vel * (4.0f - atkVel);

		
		//何フレーム目に攻撃判定が出るか
		if (animFlameCT > 5 && animFlameCT < 29) {
			FbxPlayer::SetIsAtkCollide(true);
		}
		else {
			FbxPlayer::SetIsAtkCollide(false);
		}

	}
	else if (isCombo1 == true  && isCombo2 == false) {

		//次のコンボにつなげるかどうか
		if (input_->TriggerMouseButton(0) && animFlameCT > 8) {
			isNextAtk = true;
		}

		//アニメーションと初期化処理
		gameObject_->AnimFlameInter(animFlameCT, 40);
		if (animFlameCT < 40) {	//40フレームまでアニメーション
			
		}
		else {
			if (isNextAtk == false) {	//次の行動に遷移しない時
				gameObject_->AnimFlameInter(animFlameCT, 80);
				animNum = 5;
				gameObject_->PlayAnimation(animNum);
				isAtk = false;
				isCombo1 = false;
				animFlameCT = 0;
			}
			else {	//次の攻撃に遷移
				
				animFlameCT = 0;
				atkMovePhase = 0;
				isNextAtk = false;
				

			}

		}

		//攻撃中の移動処理
		int atkVelCt = animFlameCT - 20;
		int ct = abs(atkVelCt);
		float atkVel = static_cast<float>(Ease::InOutQuad(4.0, 0.0, 20, ct));
		float atkXVel;
		if (animFlameCT < 20) {
			atkXVel = 0.07f;
		}
		else {
			atkXVel = -0.07f;
		}

		Vector3 vel = { atkXVel,0.0f,0.04f };
		vel = MathFunc::bVelocity(vel, gameObject_->wtf.matWorld_);
		gameObject_->wtf.translation_ += vel * (4.0f - atkVel);


		//何フレーム目に攻撃判定が出るか
		if (animFlameCT > 5 && animFlameCT < 15 || animFlameCT > 25 && animFlameCT < 35) {
			FbxPlayer::SetIsAtkCollide(true);
		}
		else {
			FbxPlayer::SetIsAtkCollide(false);
		}

	}
	

	if (isAtk == false) {
		_pActManager->SetNowActNum(ACTION_NUM::move);
	}
}

void Atk1::Draw()
{
}
