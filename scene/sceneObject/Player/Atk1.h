/**
 * @file Atk.h
 * @brief 自機の攻撃バリエーション1
 */
#pragma once
#include "PlayerAction.h"
class Atk1 :
    public PlayerAction
{
protected:
    PlayerActionManager* _pActManager;

public:

    Atk1(PlayerActionManager* pActManager);
    ~Atk1() override;

    // 初期化
    void Initialize(FBXObject3d* gameObject) override;
    
    // 更新
    void Update(Input* input) override;
    
    // 描画
    void Draw() override;

private:
    Input* input_ = Input::GetInstance();

private:   //どの行動でも必要
    int animNum;    //何番アニメーションか
    int animFlameCT;    //フレーム数カウント(アニメーションのインターフェース用)
    FBXObject3d* gameObject_;

private:    //固有で必要なもの
    bool isAtk = false; //攻撃行動自体のフラグ
    bool isNextAtk = false; //次の攻撃コンボに遷移するか
    bool isCombo1 = false;  //爪ひっかき攻撃
    bool isCombo2 = false;  //回転突進
    int atkMovePhase = 0;   //その行動の中で行動の順番を管理する(例:1.ブレスをためる→2.ブレスを放つ)
};
