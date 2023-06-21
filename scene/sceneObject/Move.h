#pragma once
#include "PlayerAction.h"
class Move :
    public PlayerAction
{
protected:
    PlayerActionManager* _pActManager;

public:

    Move(PlayerActionManager* pActManager);
    ~Move() override;

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
    Vector3 velocity_;
    //�ړ����x
    const float kMoveSpeed_ = 0.1f;
    //���񑬓x
    const float kTurnSpeed_ = MathFunc::Dig2Rad(10);
    bool isRun_ = false;
    Vector3 faceAngle_ = {};
};

