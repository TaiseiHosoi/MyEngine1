/**
 * @file Enemy.h
 * @brief 敵の基底クラス
 */
#pragma once
#include"Mesh.h"
#include"Object3d.h"
#include"GameCamera.h"

struct EnemyState {
    bool isAlive_ = true;
    bool isAtk_ = false;
    int hp_ = 1;
};

class Enemy
{
public:
    // 初期化
    virtual void Initialize(Mesh* Model) = 0;
    // 更新
    virtual void Update() = 0;
    // 描画
    virtual void Draw(ID3D12GraphicsCommandList* cmdList) = 0;
    //レールカメラ情報セッタ
    virtual void SetRailCameraInfo(RailCameraInfo* info) = 0;
    // その他の敵の共通機能

};

