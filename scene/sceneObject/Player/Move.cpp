#include "Move.h"



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

	faceAngle_ = gameObject_->wtf.rotation_;
	nowPos = {};
}

void Move::Update(Input* input)
{
	static_cast<void>(input);
}

void Move::Draw()
{
}
