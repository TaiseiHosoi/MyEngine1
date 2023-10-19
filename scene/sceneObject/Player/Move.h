/**
 * @file Move.h
 * @brief 自機の移動
 */
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

    // 初期化
    void Initialize(FBXObject3d* gameObject) override;

    // 更新
    void Update(Input* input) override;

    // 描画
    void Draw() override;

private:

    Input* input_ = Input::GetInstance();
private:   //どの行動でも必要
    int animNum_;
    int animFlameCT_;
    FBXObject3d* gameObject_;

private:    //固有で必要なもの
    Vector3 velocity_;
    //移動速度
    const float kMoveSpeed_ = 0.4f;
    //旋回速度
    const float kTurnSpeed_ = MathFunc::Dig2Rad(10);
    bool isRun_ = false;
    Vector3 faceAngle_ = {};
    //自機回転の最大
    const float faceMaxAngle_ = 0.4f;
    float faceRotSpeed_ = 0.05f;
    float returnRotSpeed_ = 0.03f;
    //自機の移動用Matrix
    Matrix4 pAngleMat_ = {};
    Vector3 nowPos_ = {};
    
};

