/**
 * @file EnemyManager.h
 * @brief 敵の管理クラス
 */
#pragma once
#include <vector>
#include <d3d12.h>
#include "Enemy.h"

class EnemyManager {
public:
    EnemyManager(int maxEnemies);
    ~EnemyManager();

    // 敵の初期化
    void InitializeEnemies();

    // 更新
    void UpdateEnemies();

    // 描画
    void DrawEnemies(ID3D12GraphicsCommandList* cmdList);

private:
    int maxEnemies;
    std::vector<Enemy*> enemies;
};