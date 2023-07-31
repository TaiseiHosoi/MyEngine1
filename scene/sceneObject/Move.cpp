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
	faceAngle_ = gameObject_->wtf.rotation_;
	nowPos = {};
}

void Move::Update(Input* input)
{
	//-----animation-----//
	/*if (isRun_ == true) {
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
	}*/

	//-----行動処理-----//

	Vector3 cameraAngle_ = { 0.0f,
		atan2f(gameObject_->GetCamera().GetTarget().x - gameObject_->GetCamera().GetEye().x,
			gameObject_->GetCamera().GetTarget().z - gameObject_->GetCamera().GetEye().z)
	,0.0f};

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
			//nowPos.y -= kDiagonalSpeed;


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
			//nowPos.y -= kDiagonalSpeed;

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
			//nowPos.y += kMoveSpeed_;

			if (faceAngle_.x >= -faceMaxAngle_) {
				faceAngle_.x -= faceRotSpeed_;
			}
			
		}

		//Sを押していたら
		else if (input_->PushKey(DIK_S))
		{
			//nowPos.y -= kMoveSpeed_;

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
		
		
		/*if (input_->PushKey(DIK_UP)) {
			nowPos.z += kMoveSpeed_;
		}*/		

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
	
	gameObject_->wtf.rotation_ = { faceAngle_.x, faceAngle_.y + pAngle ,faceAngle_.z};

	//画面上で自機が動くためのmatrix
	pAngleMat.identity();
	pAngleMat.rotateY(pAngle);
	Vector3 viewPos = MathFunc::bVelocity(nowPos, pAngleMat);
	
	Vector3 playerPos = primaryPos + viewPos;
	playerPos.y = 0.3f;


	//gameObject_->wtf.UpdateMatWorld();

	//velocity_ = MathFunc::MatVector(velocity_, gameObject_->wtf.matWorld_);

	gameObject_->SetPosition(playerPos);

	//if (input_->TriggerMouseButton(0) && _pActManager->GetNowActNum() != ACTION_NUM::atk1) {
	//	//pActManager_->ChangeAction(new Atk1(pActManager_.get()));
	//	_pActManager->SetNowActNum(ACTION_NUM::atk1);
	//}

	
}

void Move::Draw()
{
}
