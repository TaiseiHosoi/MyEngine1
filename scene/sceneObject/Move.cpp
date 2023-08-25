#include "Move.h"
//#include"PlayerActionManager.h"


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
	//animNum = 5;
	//animFlameCT = 0;
	//gameObject_->PlayAnimation(animNum);
	//_pActManager->SetNowActNum(0);
	faceAngle_ = gameObject_->wtf.rotation_;
	nowPos = {};
}

void Move::Update(Input* input)
{
	
}

void Move::Draw()
{
}
