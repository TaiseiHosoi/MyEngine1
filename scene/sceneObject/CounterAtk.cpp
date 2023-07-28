#include "CounterAtk.h"
#include "Ease.h"
#include "FbxPlayer.h"


CounterAtk::CounterAtk(PlayerActionManager* pActManager)
{
	_pActManager = pActManager;
}

CounterAtk::~CounterAtk()
{
	gameObject_->wtf.translation_.y = 0.0f;
	gameObject_->wtf.rotation_.x = 0.0f;
	FbxPlayer::SetIsAtkCollide(false);
}

void CounterAtk::Initialize(FBXObject3d* gameObject)
{
	gameObject_ = gameObject;
	animNum = 6;
	animFlameCT = 0;;
	gameObject_->PlayAnimation(animNum);
	_pActManager->SetNowActNum(ACTION_NUM::counter);

}

void CounterAtk::Update(Input* input)
{
	Vector3 angleVec = gameObject_->GetPosition() - gameObject_->GetCamera().GetTarget();
	float homingAngle = atan2f(angleVec.x, angleVec.z);
	gameObject_->wtf.rotation_.y = homingAngle;

	if (gameObject_->GetIsAnimRot() == true)
	{
		gameObject_->AnimIsRotateChange(false);
	}
	counterFrameCount++;	//カウンター行動が開始してからのフレームカウント

	

	//何フレーム目に攻撃判定が出るか
	if (counterFrameCount > 10 && counterFrameCount < 30 ||
		counterFrameCount > 45 && counterFrameCount < 120) {
		FbxPlayer::SetIsAtkCollide(true);
	}
	else {
		FbxPlayer::SetIsAtkCollide(false);
	}

	Vector3 cameraAngle_ = {0,
		atan2(gameObject_->GetCamera().GetTarget().x - gameObject_->GetCamera().GetEye().x,
			gameObject_->GetCamera().GetTarget().z - gameObject_->GetCamera().GetEye().z) ,
	0};

	//メイン処理
	if (counterFrameCount == 1)
	{	//初フレーム
		phase = 1;	//行動フェイズ
		animNum = 2;	//アニメーション番号
		gameObject_->PlayAnimation(animNum);

	}
	else if (counterFrameCount == max2animNum)
	{
		phase = 2;
		animFlameCT = 0;
		animNum = 3;
		gameObject_->PlayAnimation(animNum);
	}
	else if (counterFrameCount == max3animNum)
	{
		phase = 3;

	}
	else if (counterFrameCount == max4animNum)
	{
		phase = 4;
		animFlameCT = 10;
		animNum = 3;
		gameObject_->PlayAnimation(animNum);
		gameObject_->wtf.rotation_.x = 0;


	}
	else if (counterFrameCount == max5animNum)
	{
		phase = 5;
		animFlameCT = 10;
		animNum = 2;
		gameObject_->PlayAnimation(animNum);

	}
	else if (counterFrameCount == max6animNum)
	{
		//終了時処理
		phase = 0;
		counterFrameCount = 0;
		//isCounter = false;
		animNum = 5;
		gameObject_->PlayAnimation(animNum);
		kAccumulateRotVel = 0.0f;

		if (gameObject_->GetIsAnimRot() == false)
		{	//もし繰り返しアニメーションが無効だった場合
			gameObject_->AnimIsRotateChange(true);
		}

		_pActManager->SetNowActNum(ACTION_NUM::move);
		gameObject_->wtf.translation_.y = 0.0f;
		gameObject_->wtf.rotation_.x = 0.0f;
	}

	//
	if (phase == 0)
	{
		gameObject_->wtf.translation_.y = 0.0f;
	}
	else if (phase == 1)
	{
		if (animFlameCT < 30)
		{
			animFlameCT += 1;
		}
		gameObject_->wtf.translation_.y = static_cast<float>(Ease::InQuad(7.0, 0.0, max2animNum, counterFrameCount));
		gameObject_->AnimFlameInter(animFlameCT, 30);

	}
	else if (phase == 2)
	{
		if (animFlameCT < 15)
		{
			animFlameCT += 1;
		}
		kAccumulateRotVel += 0.05f;
		//gameObject_->wtf.translation_.y = Ease::InQuad(8.0, 7.0, max2animNum, counterFrameCount - max2animNum);
		gameObject_->wtf.rotation_.x += kAccumulateRotVel;
		gameObject_->AnimFlameInter(animFlameCT, 30);
	}
	else if (phase == 3)
	{

		kAccumulateRotVel += 0.08f;
		gameObject_->wtf.rotation_.x += kAccumulateRotVel;
		gameObject_->AnimFlameInter(10, 20);
	}
	else if (phase == 4)
	{
		Vector3 kATKSpeedVel = { 0 , 0 , 4.0f };
		//突進行列計算
		Matrix4 tackleMat = MathFunc::Rotation(Vector3(0.0f, cameraAngle_.y, 0.0f), 2);
		Vector3 kTackleVel = MathFunc::bVelocity(kATKSpeedVel, tackleMat);
		gameObject_->wtf.rotation_.x += kAccumulateRotVel;
		gameObject_->wtf.translation_ += kTackleVel;
		gameObject_->AnimFlameInter(10, 20);

	}
	else if (phase == 5)
	{
		if (animFlameCT < 60)
		{
			animFlameCT += 1;
		}

		gameObject_->wtf.translation_.y = static_cast<float>(Ease::InQuad(10.0, 5.0, max6animNum - max5animNum, max6animNum - counterFrameCount));
		gameObject_->wtf.translation_.x -= cameraAngle_.x * 0.1f;
		gameObject_->wtf.translation_.z -= cameraAngle_.z * 0.1f;

		gameObject_->AnimFlameInter(animFlameCT, 60);
	}



}

void CounterAtk::Draw()
{
}