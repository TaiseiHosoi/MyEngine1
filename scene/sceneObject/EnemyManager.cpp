#include "EnemyManager.h"

void EnemyManager::AddEnemy(Enemy* enemy)
{
    enemies.push_back(enemy);
}

void EnemyManager::UpdateAllEnemies()
{
    for (Enemy* enemy : enemies) {
        enemy->Update();
    }
}

void EnemyManager::DrawAllEnemies()
{
    for (Enemy* enemy : enemies) {
        enemy->Draw();
    }
}

void EnemyManager::DestroyAllEnemies()
{
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
}
