#include "Stop.h"
#include "Ease.h"
#include "FbxPlayer.h"

Stop::Stop(PlayerActionManager* pActManager)
{
	_pActManager = pActManager;
}

Stop::~Stop()
{
	
}

void Stop::Initialize(FBXObject3d* gameObject)
{
	gameObject_ = gameObject;
	animNum = 5;
	animFlameCT = 0;;
	gameObject_->PlayAnimation(animNum);
	_pActManager->SetNowActNum(0);
	
}

void Stop::Update(Input* input)
{
	input_ = input;
	if (input_->PushKey(DIK_W) ||
		input_->PushKey(DIK_A) ||
		input_->PushKey(DIK_S) ||
		input_->PushKey(DIK_D)) {
		_pActManager->SetNowActNum(ACTION_NUM::move);
	}
}

void Stop::Draw()
{
}
