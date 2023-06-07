#pragma once
#include "PlayerAction.h"
class Stop :
    public PlayerAction
{
protected:
    PlayerActionManager* _pActManager;

public:

    Stop(PlayerActionManager* pActManager);
    ~Stop() override;

    void Initialize(FBXObject3d* gameObject) override;
    void Update(Input* input) override;
    void Draw() override;

private:

    Input* input_ = Input::GetInstance();
private:   //�ǂ̍s���ł��K�v
    int animNum;
    int animFlameCT;
    FBXObject3d* gameObject_;

private:    //�ŗL�ŕK�v�Ȃ���
  
};
