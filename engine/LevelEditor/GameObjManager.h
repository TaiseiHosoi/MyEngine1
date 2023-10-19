/**
 * @file GameObjManager.h
 * @brief 読み込みデータでロードしたゲーム内の出現オブジェクトを一括管理する
 */
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
#include<list>

struct LevelData;

enum ENEMY_NUM {
    WALKING_ENEMY,
};



class GameObjManager
{
public:
    GameObjManager();
    ~GameObjManager();

private:
    //std::vector<std::unique_ptr<Enemy>> enemies;
    std::list<std::unique_ptr<WalkingEnemy>> walkingEnemies;

    LevelData* levelData;

    //モデルデータ
    std::map<std::string, Mesh*> models_;    //モデルの格納
    std::unique_ptr <Mesh> modelSkydome_;
    std::unique_ptr <Mesh> modelMoai_;
    std::unique_ptr <Mesh> modelCube_;
    std::unique_ptr <Mesh> modelRoad1_;
    std::unique_ptr <Mesh> modelCam_;
    std::unique_ptr<Mesh> modelWalkRobo_;
    std::unique_ptr<Mesh> modelTower1_;
    std::unique_ptr<Mesh> modelBill1_;

    //オブジェクトの配列
    std::vector<Object3d> objects_;
    std::vector<Object3d> camObjs_;
    std::vector<Object3d> moaiObjs_;
    std::vector<EnemyState> moaiState_;

    //オブジェクト
    Object3d* objSkydome_ = nullptr;
    Object3d* objGround_ = nullptr;
    Object3d* objFighter_ = nullptr;
    Object3d* objSphere_ = nullptr;
    Object3d* objRoad_ = nullptr;

    //当たり判定
    std::vector<SphereCollider*> moaiSpCollider_;
    std::vector<Vector3> moaiSpherePos_ = {};


    //その他
    int moaiDigRot_ = 0; //自機の回転用クロック変数
    bool isStand_ = false;  //待機フラグ
    int standTime_ = 0; //ポップデータの待機時間
    int gameTime_ = 0;

    //csv
    std::stringstream walkingEnemyPopCommands_;

    //レールカメラ情報
    RailCameraInfo* railCameraInfo_ = nullptr;

    //プレイヤー情報
    WorldTransform* playerWorldTF_ = nullptr;


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

    // 歩兵敵ポップコマンド
    void UpdateWalkingEnemyPopCommands();

    // レールカメラ情報セット
    void SetRailCamInfo(RailCameraInfo* info) { railCameraInfo_ = info; };

    // プレイヤーのworldTFのセット
    void SetPlayerWorldTF(WorldTransform* worldTF) { playerWorldTF_ = worldTF; };


public:
    // camObjsのゲッタ
    std::vector<Object3d>* GetCamObjsPtr() { return &camObjs_; };

    // enemiesのゲッタ
    //std::vector<std::unique_ptr<Enemy>>*GetEnemies() { return &enemies; }

    // 歩兵敵のゲッタ
    std::list<std::unique_ptr<WalkingEnemy>>*GetWalkingEnemies() { return &walkingEnemies; }

    

    // その他の敵の管理機能
};

