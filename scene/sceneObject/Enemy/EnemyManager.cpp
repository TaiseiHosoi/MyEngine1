#include "EnemyManager.h"
#include <cstdlib>
#include <ctime>

EnemyManager::EnemyManager(int maxEnemies) : maxEnemies_(maxEnemies) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

EnemyManager::~EnemyManager() {
   
}

void EnemyManager::InitializeEnemies() {
   //for (int i = 0; i < maxEnemies; ++i) {
   //     if (rand() % 2 == 0) {
   //         enemies.push_back(new GoblinEnemy());
   //     }
   //     else {
   //         enemies.push_back(new DragonEnemy());
   //     }

   //     enemies.back()->Initialize(enemyModel);
   //}
}

void EnemyManager::UpdateEnemies() {
    for (Enemy* enemy : enemies_) {
        enemy->Update();
    }
}

void EnemyManager::DrawEnemies(ID3D12GraphicsCommandList* cmdList) {
    for (Enemy* enemy : enemies_) {
        enemy->Draw(cmdList);
    }
}