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
private:   //どの行動でも必要
    int animNum;
    int animFlameCT;
    FBXObject3d* gameObject_;

private:    //固有で必要なもの
    Vector3 velocity_;
    //移動速度
    const float kMoveSpeed_ = 0.4f;
    //旋回速度
    const float kTurnSpeed_ = MathFunc::Dig2Rad(10);
    bool isRun_ = false;
    Vector3 faceAngle_ = {};
};

