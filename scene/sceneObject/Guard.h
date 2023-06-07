#pragma once
#include "PlayerAction.h"
class Guard :
    public PlayerAction
{
protected:
    PlayerActionManager* _pActManager;

public:

    Guard(PlayerActionManager* pActManager);
    ~Guard() override;

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
   bool isGuard = false;    //ガード
   bool isCounter = false;  //カウンター起動
   bool isGuardExcute = false;  //ガード成功時
   int guardMovePhase = 0;  //フェーズ管理
   int counterFrameCount = 0;   //フェイズ管理用カウント
   Vector3 guardKnockVel = {};  //ノックバック時のvel
   std::vector<SphereCollider*>* sphere_;   //当たり判定ポインタ

};