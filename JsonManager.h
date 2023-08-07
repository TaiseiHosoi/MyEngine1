#pragma once
#include<vector>
#include"Enemy.h"
#include"BasicEnemy.h"
#include"Mesh.h"
#include "Object3d.h"
#include"json.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"

struct LevelData;



class JsonManager
{
public:
    ~JsonManager();
public:
    struct EnemyState{
        bool isAlive_ = true;
        bool isAtk_ = false;
        int hp_ = 1;
    };

private:
    std::vector<Enemy*> enemies;

    LevelData* levelData;

    std::map<std::string, Mesh*> models;

    std::vector<Object3d> objects;
    std::vector<Object3d> camObjs;
    std::vector<Object3d> moaiObjs;
    std::vector<EnemyState> moaiState;

    std::unique_ptr <Mesh> modelSkydome;
    std::unique_ptr <Mesh> modelMoai;
    std::unique_ptr <Mesh> modelCube;
    std::unique_ptr <Mesh> modelRoad1;
    std::unique_ptr <Mesh> modelCam;


   

    Object3d* objSkydome = nullptr;
    Object3d* objGround = nullptr;
    Object3d* objFighter = nullptr;
    Object3d* objSphere = nullptr;
    Object3d* objRoad = nullptr;

    std::vector<SphereCollider*> moaiSpCollider;
    std::vector<Vector3> moaiSpherePos = {};
    int moaiDigRot = 0;


public:
    void StaticInit();

    void AddEnemy(Enemy* enemy);

    void UpdateAllObjects();

    void DrawAllEnemies(ID3D12GraphicsCommandList* cmdList);

    void DestroyAllEnemies();

public:
    std::vector<Object3d>* GetCamObjsPtr() { return &camObjs; };

    // その他の敵の管理機能
};

