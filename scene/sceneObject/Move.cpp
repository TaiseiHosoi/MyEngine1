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

	//-----�s������-----//
	//���x��0�ɂ���
	velocity_ = { 0 , 0 , 0 };

	Vector3 cameraAngle_ = { 0.0f,
		atan2f(gameObject_->GetCamera().GetTarget().x - gameObject_->GetCamera().GetEye().x,
			gameObject_->GetCamera().GetTarget().z - gameObject_->GetCamera().GetEye().z)
	,0.0f};

	Vector3 nowPos = gameObject_->GetPosition();
	

	//�L�[���͂���������
	if (input_->PushKey(DIK_W) ||
		input_->PushKey(DIK_A) ||
		input_->PushKey(DIK_S) ||
		input_->PushKey(DIK_D))
	{
		

		//Z�������ɂ̑��x������
		velocity_ = { 0 , 0 , kMoveSpeed_ };

		float kDiagonalSpeed = kMoveSpeed_ * 0.707f;

		//W,D�������Ă�����
		if (input_->PushKey(DIK_W) && input_->PushKey(DIK_D))
		{
			nowPos.x += kDiagonalSpeed;
			nowPos.y += kDiagonalSpeed;

		}

		//W,A�������Ă�����
		else if (input_->PushKey(DIK_W) && input_->PushKey(DIK_A))
		{

			nowPos.x -= kDiagonalSpeed;
			nowPos.y += kDiagonalSpeed;

		}

		//S,D�������Ă�����
		else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_D))
		{

			nowPos.x += kDiagonalSpeed;
			nowPos.y -= kDiagonalSpeed;

		}

		//S,A�������Ă�����
		else if (input_->PushKey(DIK_S) && input_->PushKey(DIK_A))
		{
			nowPos.x -= kDiagonalSpeed;
			nowPos.y -= kDiagonalSpeed;

		}

		//W�������Ă�����
		else if (input_->PushKey(DIK_W))
		{
			nowPos.y += kMoveSpeed_;
			
		}

		//S�������Ă�����
		else if (input_->PushKey(DIK_S))
		{
			nowPos.y -= kMoveSpeed_;
			
		}

		//D�������Ă�����
		else if (input_->PushKey(DIK_D))
		{
			nowPos.x += kMoveSpeed_;
			
		}

		//A�������Ă�����
		else if (input_->PushKey(DIK_A))
		{
			nowPos.x -= kMoveSpeed_;
		}

		
		if (input_->PushKey(DIK_UP)) {
			nowPos.z += kMoveSpeed_;
		}
		

		gameObject_->wtf.rotation_ = faceAngle_;


		
	}
	else
	{
		/*if (isRun_ == true) {
			isRun_ = false;
			animNum = 5;
			gameObject_->PlayAnimation(animNum);
		}*/
	}

	
	if (input_->TriggerMouseButton(0) && _pActManager->GetNowActNum() != ACTION_NUM::atk1) {
		//pActManager_->ChangeAction(new Atk1(pActManager_.get()));
		_pActManager->SetNowActNum(ACTION_NUM::atk1);
	}

	//gameObject_->wtf.UpdateMatWorld();

	//velocity_ = MathFunc::MatVector(velocity_, gameObject_->wtf.matWorld_);

	gameObject_->SetPosition(nowPos);



	
}

void Move::Draw()
{
}
