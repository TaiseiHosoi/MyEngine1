﻿/**
 * @file GameObjManager.h
 * @brief 読み込みデータでロードしたゲーム内の出現オブジェクトを一括管理する
 */
#pragma once
#include<vector>
#include"Enemy.h"
#include"WalkingEnemy.h"
#include"FloatingEnemy.h"
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
#include<list>

struct LevelData;

enum ENEMY_NUM {
    WALKING_ENEMY,
    FLOATING_ENEMY
};

enum SPOWN_OFFSET_POS{
    SP_RIGHT = 0,
    SP_CENTER = 1,
    SP_LEFT = 2
};



class GameObjManager
{
public:
    GameObjManager();
    ~GameObjManager();

private:
    //std::vector<std::unique_ptr<Enemy>> enemies;
   

    LevelData* levelData;

    //モデルデータ
    std::map<std::string, Mesh*> models;    //モデルの格納
    std::unique_ptr <Mesh> modelSkydome;
    std::unique_ptr <Mesh> modelMoai;
    std::unique_ptr <Mesh> modelCube;
    std::unique_ptr <Mesh> modelRoad1;
    std::unique_ptr <Mesh> modelCam;
    std::unique_ptr<Mesh> modelWalkRobo;
    std::unique_ptr<Mesh> modelTower1;
    std::unique_ptr<Mesh> modelBill1;

    //オブジェクトの配列
    std::vector<Object3d> objects;
    std::vector<Object3d> camObjs;
    std::vector<Object3d> moaiObjs;
    std::vector<EnemyState> moaiState;
\

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
    const int offsetEnemyHp_ = 3;

    //歩兵用変数
    std::list<std::unique_ptr<WalkingEnemy>> walkingEnemies;
    std::stringstream walkingEnemyPopCommands_; //csv
    bool walkingEIsStand_ = false;  //待機フラグ
    int walkingEstandTime_ = 0; //ポップデータの待機時間
    const float adjustWalkingESpownLen_ = 15.f;

    //浮遊敵用変数
    std::list <std::unique_ptr<FloatingEnemy>> floatingEnemies;
    std::stringstream floatingEnemyPopCommands_; //csv
    bool floatingEIsStand_ = false;  //待機フラグ
    int floatingEstandTime_ = 0; //ポップデータの待機時間
    const float adjustFloatingESpownLenShort_ = 8.f;
    const float adjustFloatingESpownLenLong_ = 16.f;


    int gameTime_ = 0;

    //固定値



   

    //レールカメラ情報
    RailCameraInfo* railCameraInfo_ = nullptr;

    //プレイヤー情報
    WorldTransform* playerWorldTF_ = nullptr;

    //歩兵全削除
    void DeleteWalkingEnemy();


public:
    // ゲーム起動時の初期化
    void StaticInit();

    /// <summary>
    /// 敵の追加
    /// </summary>
    /// <param name="enemyNum"></敵の種類(Enum管理)>
    /// <param name="popTime"></ディレイ>
    /// <param name="offsetPos"></沸き位置ずらす値>
    void AddEnemy(int enemyNum,int popTime,Vector3 offsetPos);

    // 全オブジェクト更新
    void UpdateAllObjects();

    // 全敵オブジェ描画
    void DrawAllEnemies(ID3D12GraphicsCommandList* cmdList);

    // 全敵消滅
    void DestroyAllEnemies();

    // ファイル情報読み取り
    void LoadData(const char* filename, std::stringstream& stream);

    //ファイル読みとりリセット
    void ResetCommands(const char* filename, std::stringstream& stream);

    // ポップコマンド用変数更新
    void UpdatePopCommandsInfo();

    // 歩兵敵ポップコマンド更新
    void UpdateWalkingEnemyPopCommands();

    // 浮遊敵ポップコマンド更新
    void UpdateFloatingEnemyPopCommands();

    // レールカメラ情報セット
    void SetRailCamInfo(RailCameraInfo* info) { railCameraInfo_ = info; };

    // プレイヤーのworldTFのセット
    void SetPlayerWorldTF(WorldTransform* worldTF);


public:
    // camObjsのゲッタ
    std::vector<Object3d>* GetCamObjsPtr() { return &camObjs; };

    // enemiesのゲッタ
    //std::vector<std::unique_ptr<Enemy>>*GetEnemies() { return &enemies; }

    // 歩兵敵のゲッタ
    std::list<std::unique_ptr<WalkingEnemy>>*GetWalkingEnemies() { return &walkingEnemies; }

    

    // その他の敵の管理機能
};

