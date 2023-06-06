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

	//�J�E���g�v���X
	animFlameCT++;
	
	//�A�j���[�V��������
	if (animFlameCT > 30)
	{	
		
		//30�t���ڂŃA�j���[�V�����I���
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

		//�U�����̈ړ�����
		int atkVelCt = animFlameCT - 15;
		int ct = abs(atkVelCt);
		float atkVel = Ease::InOutQuad(4.0, 0.0, 15, ct);
		Vector3 vel = { 0,0,0.3f };
		vel = MathFunc::bVelocity(vel, gameObject_->wtf.matWorld_);
		gameObject_->wtf.translation_ += vel * (4.0f - atkVel);

		
		//���t���[���ڂɍU�����肪�o�邩
		if (animFlameCT > 5 && animFlameCT < 29) {
			FbxPlayer::SetIsAtkCollide(true);
		}
		else {
			FbxPlayer::SetIsAtkCollide(false);
		}

	}
	else if (isCombo1 == true  && isCombo2 == false) {

		//���̃R���{�ɂȂ��邩�ǂ���
		if (input_->TriggerMouseButton(0) && animFlameCT > 8) {
			isNextAtk = true;
		}

		//�A�j���[�V�����Ə���������
		gameObject_->AnimFlameInter(animFlameCT, 40);
		if (animFlameCT < 40) {	//40�t���[���܂ŃA�j���[�V����
			
		}
		else {
			if (isNextAtk == false) {	//���̍s���ɑJ�ڂ��Ȃ���
				gameObject_->AnimFlameInter(animFlameCT, 80);
				animNum = 5;
				gameObject_->PlayAnimation(animNum);
				isAtk = false;
				isCombo1 = false;
				animFlameCT = 0;
			}
			else {	//���̍U���ɑJ��
				
				animFlameCT = 0;
				atkMovePhase = 0;
				isNextAtk = false;
				

			}

		}

		//�U�����̈ړ�����
		int atkVelCt = animFlameCT - 20;
		int ct = abs(atkVelCt);
		float atkVel = Ease::InOutQuad(4.0, 0.0, 20, ct);
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


		//���t���[���ڂɍU�����肪�o�邩
		if (animFlameCT > 5 && animFlameCT < 15 || animFlameCT > 25 && animFlameCT < 35) {
			FbxPlayer::SetIsAtkCollide(true);
		}
		else {
			FbxPlayer::SetIsAtkCollide(false);
		}

	}
	//else if (isCombo2 == true && isCombo1 == false) {

	//	if (atkMovePhase == 0) {
	//		int maxCurrent = 30;
	//		if (animFlameCT == 0) {	//���t���[��(������)
	//			animNum = 2;
	//			gameObject_->PlayAnimation(animNum);

	//		}

	//		//�����ł��オ�铮��
	//		gameObject_->SetPosition({ gameObject_->GetWorldTransform().translation_.x,
	//			static_cast<float>(Ease::OutQuad(5, 0, maxCurrent,animFlameCT)),
	//			gameObject_->GetWorldTransform().translation_.z }
	//		);

	//		//�A�j���[�V����
	//		gameObject_->AnimFlameInter(animFlameCT, maxCurrent + 3);
	//		
	//		if (animFlameCT >= maxCurrent) {
	//			atkMovePhase = 1;	//�J�����g�ő�l�ɂȂ�����t�F�[�Y��i�߂�
	//			animFlameCT = 0;
	//		}

	//		////���t���[���ڂɍU�����肪�o�邩
	//		//if (animFlameCT > 10 && animFlameCT < 30) {
	//		//	FbxPlayer::isAtkCollide = true;
	//		//}
	//		//else {
	//		//	FbxPlayer::isAtkCollide = false;
	//		//}

	//	}
	//	else if (atkMovePhase == 1) {
	//		int maxCurrent = 60;

	//		if (animFlameCT == 0) {	//���t���[��(������)
	//			animNum = 3;
	//			gameObject_->PlayAnimation(animNum);

	//		}

	//		//�A�j���[�V�����Ǘ�
	//		gameObject_->AnimFlameInter(10, 20);
	//		

	//		//��]
	//		float rotSpeed = 0.8f;
	//		gameObject_->wtf.rotation_.x += rotSpeed;


	//		////���t���[���ڂɍU�����肪�o�邩
	//		//if (animFlameCT > 10 && animFlameCT < 60) {
	//		//	FbxPlayer::isAtkCollide = true;
	//		//}
	//		//else {
	//		//	FbxPlayer::isAtkCollide = false;
	//		//}

	//		//�t�F�[�Y�Ǘ�
	//		if (animFlameCT >= maxCurrent) {
	//			isAtk = false;
	//			isCombo2 = false;	//�J�����g�ő�l�ɂȂ�����t�F�[�Y��i�߂�
	//			animFlameCT = 0;
	//			gameObject_->AnimFlameInter(animFlameCT, 80);
	//			animNum = 5;
	//			gameObject_->PlayAnimation(animNum);
	//			atkMovePhase = 0;
	//			gameObject_->wtf.translation_.y = 0;
	//		}
	//	}
	//}

	if (isAtk == false) {
		_pActManager->SetNowActNum(ACTION_NUM::move);
	}
}

void Atk1::Draw()
{
}
