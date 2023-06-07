#include "Guard.h"
#include "Ease.h"
#include "FbxPlayer.h"




Guard::Guard(PlayerActionManager* pActManager)
{
	_pActManager = pActManager;
}

Guard::~Guard()
{

}

void Guard::Initialize(FBXObject3d* gameObject)
{
	gameObject_ = gameObject;
	animNum = 6;
	animFlameCT = 0;;
	gameObject_->PlayAnimation(animNum);

	//���[�ǂ��I���ɂȂ����ۂ̏�����
	//�t���O�Ǘ�
	isGuardExcute = false;
	isGuard = true;
	isCounter = false;
	//�J�E���g������
	guardMovePhase = 0;
	animFlameCT = 0;
	animNum = 6;
	gameObject_->PlayAnimation(animNum);
	gameObject_->AnimIsRotateChange(true);

	//�����蔻��|�C���^
	sphere_ = _pActManager->GetSphere();


}

void Guard::Update(Input* input)
{
	if (isGuard == false) {
		_pActManager->SetNowActNum(ACTION_NUM::move);
	}

	
	if (input_->ReleaseMouseButton(1)) {

		isGuard = false;
		
		counterFrameCount = 0;
		if (gameObject_->GetIsAnimRot() == true) {
			gameObject_->AnimIsRotateChange(false);
		}
		animNum = 5;
		gameObject_->PlayAnimation(animNum);
	}


	if (isGuard == true) {

		counterFrameCount++;	//�t���O�Ȃǃv���O�����p�̃J�E���^�[
		if (animFlameCT < 10) {
			animFlameCT++;	//�A�j���[�V������p�J�E���^�[
		}
		

		gameObject_->AnimFlameInter(animFlameCT, 20);

		
		for (int i = 0; i < _pActManager->GetSphereColNum(); i++) {
			
			
			if (sphere_->at(i)->GetIsHit() == true) {
				if (sphere_->at(i)->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_ENEMIES
					|| sphere_->at(i)->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_ENEMIEBULLETS) {
					isGuardExcute = true;
					counterFrameCount = 0;
					animNum = 8;
					gameObject_->PlayAnimation(animNum);

					break;
				}
			}
		}
		
	}

	if (isGuardExcute == true) {
		if (input_->TriggerMouseButton(0)) {
			isCounter = true;
			isGuardExcute = false;
		}

		const int maxCount = 30;

		gameObject_->AnimFlameInter(animFlameCT, maxCount);
		counterFrameCount++;
		if (animFlameCT >= maxCount) {
			isGuardExcute = false;
		}
		else {
			animFlameCT++;
		}

		//�m�b�N�o�b�N����
		if (animFlameCT < 20) {
			
			guardKnockVel = {cosf(gameObject_->wtf.rotation_.y), 0, sinf(gameObject_->wtf.rotation_.y)};

			gameObject_->wtf.translation_ += guardKnockVel * (0.2f / static_cast<float>((maxCount - animFlameCT)));
		}

		//�ăK�[�h
		for (int i = 0; i < _pActManager->GetSphereColNum(); i++) {
			if (sphere_->at(i)->GetIsHit() == true) {
				if (sphere_->at(i)->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_ENEMIES
					|| sphere_->at(i)->GetCollisionInfo().collider->GetAttribute() == COLLISION_ATTR_ENEMIEBULLETS) {
					isGuardExcute = true;
					animFlameCT = 0;
					counterFrameCount = 0;

					break;
				}
			}
		}

	}

	if (isCounter == true) {
		_pActManager->SetNowActNum(ACTION_NUM::counter);
	}

}

void Guard::Draw()
{
}