#pragma once
#include<vector>
#include"Enemy.h"
#include"BasicEnemy.h"
#include"Mesh.h"
#include "Object3d.h"
#include"json.h"

struct LevelData;



class EnemyManager
{
private:
    std::vector<Enemy*> enemies;

    LevelData* levelData;

    std::map<std::string, Mesh*> models;

    std::vector<Object3d> objects;

    std::unique_ptr <Mesh> modelSkydome;
    std::unique_ptr <Mesh> modelMoai;


   

    Object3d* objSkydome = nullptr;
    Object3d* objGround = nullptr;
    Object3d* objFighter = nullptr;
    Object3d* objSphere = nullptr;

public:
    void StaticInit();

    void AddEnemy(Enemy* enemy);

    void UpdateAllEnemies();

    void DrawAllEnemies(ID3D12GraphicsCommandList* cmdList);

    void DestroyAllEnemies();

    // その他の敵の管理機能
};

