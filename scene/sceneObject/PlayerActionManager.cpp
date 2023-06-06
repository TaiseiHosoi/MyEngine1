#include "PlayerActionManager.h"
#include "Move.h"

PlayerActionManager::PlayerActionManager()
{
	_action.reset(new Move(&*this));
	
}

PlayerActionManager::~PlayerActionManager()
{
	
}

void PlayerActionManager::PActionInitialize(FBXObject3d* gameObject)
{
	gameObject_ = gameObject;
	_action.get()->Initialize(gameObject_);

}

void PlayerActionManager::ActionUpdate(Input* input)
{
	_action.get()->Update(input);
}

void PlayerActionManager::ActionDraw()
{
	_action.get()->Draw();
}

void PlayerActionManager::ChangeAction(PlayerAction* pAction)
{
	_action.reset(pAction);
	PActionInitialize(gameObject_);
	_action.get()->Initialize(gameObject_);
}

void PlayerActionManager::ColliderInitialize(std::vector<SphereCollider*>* sphere,const int sphereColNum)
{
	sphere_ = sphere;
	SPHERE_COLISSION_NUM = sphereColNum;
}

void PlayerActionManager::DamagePlayer()
{
	
}
