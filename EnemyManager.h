#pragma once
#include <vector>
#include <d3d12.h>
#include "Enemy.h"

class EnemyManager {
public:
    EnemyManager(int maxEnemies);
    ~EnemyManager();

    void InitializeEnemies();
    void UpdateEnemies();
    void DrawEnemies(ID3D12GraphicsCommandList* cmdList);

private:
    int maxEnemies;
    std::vector<Enemy*> enemies;
};