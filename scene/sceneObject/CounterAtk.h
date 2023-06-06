#pragma once
#include "PlayerAction.h"
class CounterAtk :
    public PlayerAction
{
protected:
    PlayerActionManager* _pActManager;

public:

    CounterAtk(PlayerActionManager* pActManager);
    ~CounterAtk() override;

    void Initialize(FBXObject3d* gameObject) override;
    void Update(Input* input) override;
    void Draw() override;

private:

    Input* input_ = Input::GetInstance();
private:   //どの行動でも必要
    int animNum;    //何番アニメーションか
    int animFlameCT;    //フレーム数カウント(アニメーションのインターフェース用)
    FBXObject3d* gameObject_;

private:    //固有で必要なもの
    //フェーズのターニングポイント
    const int max2animNum = 30;	//サマーソルトの最大フレーム
    const int max3animNum = max2animNum + 25;	//回転して力をためる最大フレーム
    const int max4animNum = max3animNum + 20;
    const int max5animNum = max4animNum + 45;
    const int max6animNum = max5animNum + 35;
    
    int counterFrameCount = 0;  //フェーズ管理用カウンタ
    int phase = 0;  //フェーズ
    float kAccumulateRotVel = 0;    //回転の速度を加速度的に蓄積させるための変数
};