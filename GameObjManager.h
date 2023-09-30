#pragma once
#include<vector>
#include"Enemy.h"
#include"WalkingEnemy.h"
#include"BasicEnemy.h"
#include"Mesh.h"
#include "Object3d.h"
#include"json.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include <cassert>
#include<fstream>
#include<string>

struct LevelData;

enum ENEMY_NUM {
    WALKING_ENEMY,
};

class GameObjManager
{
public:
    struct EnemyState{
        bool isAlive_ = true;
        bool isAtk_ = false;
        int hp_ = 1;
    };

private:
    std::vector<Enemy*> enemies;
    std::vector<WalkingEnemy*> walkingEnemies;

    LevelData* levelData;

    //モデルデータ
    std::map<std::string, Mesh*> models;    //モデルの格納
    std::unique_ptr <Mesh> modelSkydome;
    std::unique_ptr <Mesh> modelMoai;
    std::unique_ptr <Mesh> modelCube;
    std::unique_ptr <Mesh> modelRoad1;
    std::unique_ptr <Mesh> modelCam;
    std::unique_ptr<Mesh> modelWalkRobo;

    //オブジェクトの配列
    std::vector<Object3d> objects;
    std::vector<Object3d> camObjs;
    std::vector<Object3d> moaiObjs;
    std::vector<EnemyState> moaiState;

    //オブジェクト
    Object3d* objSkydome = nullptr;
    Object3d* objGround = nullptr;
    Object3d* objFighter = nullptr;
    Object3d* objSphere = nullptr;
    Object3d* objRoad = nullptr;

    //当たり判定
    std::vector<SphereCollider*> moaiSpCollider;
    std::vector<Vector3> moaiSpherePos = {};

    //その他
    int moaiDigRot = 0; //自機の回転用クロック変数
    bool isStand_ = false;  //待機フラグ
    int standTime_ = 0; //ポップデータの待機時間


    //csv
    std::stringstream walkingEnemyPopCommands_;


public:
    void StaticInit();

    void AddEnemy(int enemyNum);

    void UpdateAllObjects();

    void DrawAllEnemies(ID3D12GraphicsCommandList* cmdList);

    void DestroyAllEnemies();

    void LoadData(const char* filename, std::stringstream& stream);

    void ResetCommands(const char* filename, std::stringstream& stream);

    void UpdateWalkingEnemyPopCommands();


public:
    std::vector<Object3d>* GetCamObjsPtr() { return &camObjs; };

    std::vector<Enemy*>GetEnemies() { return enemies; }

    std::vector<WalkingEnemy*>GetWalkingEnemies() { return walkingEnemies; }



    // その他の敵の管理機能
};

