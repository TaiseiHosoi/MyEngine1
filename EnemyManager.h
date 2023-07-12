#pragma once
#include<vector>
#include"Enemy.h"
#include"BasicEnemy.h"
#include"Mesh.h"
class EnemyManager
{
private:
    std::vector<Enemy*> enemies;

public:
    void AddEnemy(Enemy* enemy);

    void UpdateAllEnemies();

    void DrawAllEnemies(ID3D12GraphicsCommandList* cmdList);

    void DestroyAllEnemies();

    // その他の敵の管理機能
};

