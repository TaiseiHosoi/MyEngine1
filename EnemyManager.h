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

    void DrawAllEnemies();

    void DestroyAllEnemies();

    // ‚»‚Ì‘¼‚Ì“G‚ÌŠÇ—‹@”\
};

