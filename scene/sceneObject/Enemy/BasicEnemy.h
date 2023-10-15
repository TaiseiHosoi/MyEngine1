/**
 * @file BasicEnemy.h
 * @brief 基本的な敵のクラス
 */
#pragma once
#include "Enemy.h"
class BasicEnemy :
    public Enemy
{
public:
    // 初期化
    void Initialize(Mesh* Model) override;

    // 更新
    void Update() override;

    // 描画
    void Draw(ID3D12GraphicsCommandList* cmdList) override;
    // 基本的な敵に特有の追加機能
};

