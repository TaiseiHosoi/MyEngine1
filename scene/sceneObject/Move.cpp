#include "Move.h"
#include"PlayerActionManager.h"


Move::Move(PlayerActionManager* pActManager)
{
	_pActManager = pActManager;
}

Move::~Move()
{
}

void Move::Initialize(FBXObject3d* gameObject)
{
	gameObject_ = gameObject;
	isRun_ = false;
	animNum = 5;
	animFlameCT = 0;
	gameObject_->PlayAnimation(animNum);
	_pActManager->SetNowActNum(0);
	gameObject_->wtf.translation_.y = 0;
	faceAngle_.y = gameObject_->wtf.rotation_.y;
}

void Move::Update(Input* input)
{
	//-----animation-----//
	if (isRun_ == true) {
		int maxFlame = 30;

		animFlameCT++;

		if (animFlameCT >= maxFlame) {
			animFlameCT = 0;
		}
		
		gameObject_->AnimFlameInter(animFlameCT, maxFlame);

	}
	else {
		int maxFlame = 30;

		animFlameCT++;
		if (animFlameCT >= maxFlame) {
			animFlameCT = 0;
		}

		gameObject_->AnimFlameInter(animFlameCT, maxFlame);
	}

	//-----s“®ˆ—-----//
	//‘¬“x‚ð0‚É‚·‚é
	velocity_ = { 0 , 0 , 0 };

	Vector3 cameraAngle_ = { 0.0f,
		atan2f(gameObject_->GetCamera().GetTarget().x - gameObject_->GetCamera().GetEye().x,
			gameObject_->GetCamera().GetTarget().z - gameObject_->GetCamera().GetEye().z)
	,0.0f};

	//ƒL[“ü—Í‚ª‚ ‚Á‚½‚ç
	if (input_->PushKey(DIK_W) ||
		input_->PushKey(DIK_A) ||
		input_->PushKey(DIK_S) ||
		input_->PushKey(DIK_D))
	{
		if (isRun_ == false) {
			isRun_ = true;
			animNum = 0;
			gameObject_->PlayAnimation(animNum);
		}

		faceAngle_ -= cameraAngle_;

		//ZŽ²•ûŒü‚É‚Ì‘¬“x‚ð“ü‚ê‚é
		velocity_ = { 0 , 0 , kMoveSpeed_ };

		//W,D‚ð‰Ÿ‚µ‚Ä‚¢‚½‚ç
		if (input_->PushKey(DIK_W) && input_->PushKey(DIK_D))
		{

			//45“x•ûŒü‚ÉŒü‚­‚æ‚¤‚É‰ñ“]‚³‚¹‚é
			if (faceAngle_.y != MathFunc::Dig2Rad(45))
			{
				if (faceAngle_.y >= MathFunc::Dig2Rad(45) &&
					faceAngle_.y <= MathFunc::Dig2Rad(225))
				{

					faceAngle_.y -= kTurnSpeed_;

					if (faceAngle_.y <= MathFunc::Dig2Rad(45) ||
						faceAngle_.y >= MathFunc::Dig2Rad(225))
					{
						faceAngle_.y = MathFunc::Dig2Rad(45);
					}
				}
				else
				{

					faceAngle_.y += kTurnSpeed_;

					if (faceAngle_.y >= MathFunc::Dig2Rad(45) &&
						faceAngle_.y <= MathFunc::Dig2Rad(225))
					{
						faceAngle_.y = MathFunc::Dig2Rad(45);
					}
				}
			}

		}

		//W,A‚ð‰Ÿ‚µ‚Ä‚¢‚½‚ç
		else if (input_->PushKey(DIK_W) && input_->PushKey(DIK_A))
		{

			//135“x•ûŒü‚ÉŒü‚­‚æ‚¤‚É‰ñ“]‚³‚¹‚é
			if (faceAngle_.y != MathFunc::Dig2Rad(315))
			{
				if (faceAngle_.y >= MathFunc::Dig2Rad(125) &&
					faceAngle_.y <= MathFunc::Dig2Rad(315))
				{

					faceAngle_.y += kTurnSpeed_;

					if (faceAngle_.y <= MathFunc::Dig2Rad(125) ||
						faceAngle_.y >= MathFunc::Dig2Rad(315))
					{
						faceAngle_.y = MathFunc::Dig2Rad(315);
					}
				}
				else
				{

					faceAngle_.y -= kTurnSpeed_;

					if (faceAngle_.y >= MathFunc::Dig2Rad(125) &&
						faceAngle_.y <= MathFunc::Dig2Rad(315))
					{
						faceAngle_.y = MathFunc::Dig2Rad(315);
					}
				}
			}

		}

		//S,D‚ð‰Ÿ‚µ‚Ä‚¢‚½‚ç
		else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_D))
		{

			//315“x•ûŒü‚ÉŒü‚­‚æ‚¤‚É‰ñ“]‚³‚¹‚é
			if (faceAngle_.y != MathFunc::Dig2Rad(135))
			{
				if (faceAngle_.y <= MathFunc::Dig2Rad(315) &&
					faceAngle_.y >= MathFunc::Dig2Rad(135))
				{

					faceAngle_.y -= kTurnSpeed_;

					if (MathFunc::Dig2Rad(135) >= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(135);
					}
				}
				else
				{

					faceAngle_.y += kTurnSpeed_;

					if (MathFunc::Dig2Rad(135) <= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(135);
					}
				}
			}

		}

		//S,A‚ð‰Ÿ‚µ‚Ä‚¢‚½‚ç
		else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_A))
		{

			//225“x•ûŒü‚ÉŒü‚­‚æ‚¤‚É‰ñ“]‚³‚¹‚é
			if (faceAngle_.y != MathFunc::Dig2Rad(225))
			{
				if (faceAngle_.y >= MathFunc::Dig2Rad(45) &&
					faceAngle_.y <= MathFunc::Dig2Rad(225))
				{

					faceAngle_.y += kTurnSpeed_;

					if (MathFunc::Dig2Rad(225) <= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(225);
					}
				}
				else
				{

					faceAngle_.y -= kTurnSpeed_;

					if (MathFunc::Dig2Rad(225) >= faceAngle_.y &&
						faceAngle_.y >= MathFunc::Dig2Rad(45))
					{
						faceAngle_.y = MathFunc::Dig2Rad(225);
					}

				}
			}

		}

		//W‚ð‰Ÿ‚µ‚Ä‚¢‚½‚ç
		else if (input_->PushKey(DIK_W))
		{

			//0“x•ûŒü‚ÉŒü‚­‚æ‚¤‚É‰ñ“]‚³‚¹‚é
			if (faceAngle_.y != MathFunc::Dig2Rad(0))
			{
				if (faceAngle_.y <= MathFunc::Dig2Rad(180))
				{

					faceAngle_.y -= kTurnSpeed_;

					if (MathFunc::Dig2Rad(0) >= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(0);
					}
				}
				else
				{

					faceAngle_.y += kTurnSpeed_;

					if (MathFunc::Dig2Rad(360) <= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(0);
					}
				}
			}
		}

		//S‚ð‰Ÿ‚µ‚Ä‚¢‚½‚ç
		else if (input_->PushKey(DIK_S))
		{

			//180“x•ûŒü‚ÉŒü‚­‚æ‚¤‚É‰ñ“]‚³‚¹‚é
			if (faceAngle_.y != MathFunc::Dig2Rad(180))
			{
				if (faceAngle_.y <= MathFunc::Dig2Rad(180))
				{

					faceAngle_.y += kTurnSpeed_;

					if (MathFunc::Dig2Rad(180) <= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(180);
					}
				}
				else
				{

					faceAngle_.y -= kTurnSpeed_;

					if (MathFunc::Dig2Rad(180) >= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(180);
					}
				}
			}
		}

		//D‚ð‰Ÿ‚µ‚Ä‚¢‚½‚ç
		else if (input_->PushKey(DIK_D))
		{

			//90“x•ûŒü‚ÉŒü‚­‚æ‚¤‚É‰ñ“]‚³‚¹‚é
			if (faceAngle_.y != MathFunc::Dig2Rad(90))
			{
				if (faceAngle_.y != MathFunc::Dig2Rad(90))
				{
					if (faceAngle_.y >= MathFunc::Dig2Rad(90) &&
						faceAngle_.y <= MathFunc::Dig2Rad(270))
					{
						faceAngle_.y -= kTurnSpeed_;

						if (faceAngle_.y <= MathFunc::Dig2Rad(90) ||
							MathFunc::Dig2Rad(270) <= faceAngle_.y)
						{
							faceAngle_.y = MathFunc::Dig2Rad(90);
						}

					}
					else
					{
						faceAngle_.y += kTurnSpeed_;

						if (faceAngle_.y >= MathFunc::Dig2Rad(90) &&
							faceAngle_.y <= MathFunc::Dig2Rad(270))
						{
							faceAngle_.y = MathFunc::Dig2Rad(90);
						}
					}
				}
			}
		}

		//A‚ð‰Ÿ‚µ‚Ä‚¢‚½‚ç
		else if (input_->PushKey(DIK_A))
		{
			//270“x•ûŒü‚ÉŒü‚­‚æ‚¤‚É‰ñ“]‚³‚¹‚é
			if (faceAngle_.y != MathFunc::Dig2Rad(270))
			{
				if (faceAngle_.y >= MathFunc::Dig2Rad(90) &&
					faceAngle_.y <= MathFunc::Dig2Rad(270))
				{
					faceAngle_.y += kTurnSpeed_;

					if (faceAngle_.y <= MathFunc::Dig2Rad(90) ||
						MathFunc::Dig2Rad(270) <= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(270);
					}

				}
				else
				{
					faceAngle_.y -= kTurnSpeed_;

					if (faceAngle_.y >= MathFunc::Dig2Rad(90) &&
						MathFunc::Dig2Rad(270) >= faceAngle_.y)
					{
						faceAngle_.y = MathFunc::Dig2Rad(270);
					}
				}
			}
		}

		if (MathFunc::Dig2Rad(360) < faceAngle_.y)
		{
			faceAngle_.y += -MathFunc::Dig2Rad(360);
		}
		if (faceAngle_.y < 0)
		{
			faceAngle_.y += MathFunc::Dig2Rad(360);
		}

		faceAngle_ += cameraAngle_;

		gameObject_->wtf.rotation_ = faceAngle_;


		
	}
	else
	{
		if (isRun_ == true) {
			isRun_ = false;
			animNum = 5;
			gameObject_->PlayAnimation(animNum);
		}
	}

	
	if (input_->TriggerMouseButton(0) && _pActManager->GetNowActNum() != ACTION_NUM::atk1) {
		//pActManager_->ChangeAction(new Atk1(pActManager_.get()));
		_pActManager->SetNowActNum(ACTION_NUM::atk1);
	}

	//gameObject_->wtf.UpdateMatWorld();

	velocity_ = MathFunc::MatVector(velocity_, gameObject_->wtf.matWorld_);

	gameObject_->wtf.translation_ += velocity_;



	
}

void Move::Draw()
{
}
