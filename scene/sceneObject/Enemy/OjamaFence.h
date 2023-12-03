/**
 * @file OjamaFence.h
 * @brief 歩兵っぽい敵
 */
#pragma once
#include "GameCamera.h"
#include "Enemy.h"
#include"CollisionAttribute.h"
#include"CollisionManager.h"
#include"SphereCollider.h"
#include"EnemyNormalBullet.h"

class OjamaFence : public Enemy {
public:
    OjamaFence();
    ~OjamaFence();

    // 初期化
    void Initialize(Mesh* model) override;

    // 更新
    void Update() override;

    // 描画
    void Draw(ID3D12GraphicsCommandList* cmdList) override;
    // 固有のメソッドやデータ

public: // アクセッサ
    // レールカメラ情報セッタ
    void SetRailCameraInfo(RailCameraInfo* info) override { railCameraInfo_ = info; };

    // playerWorldTransformセット
    void SetPlayerWorldTransform(WorldTransform* worldTransform);

    // offsetPos_セッタ
    void SetOffsetVec3(const Vector3& v) { offsetPos_ = v; };

    // object3d_ゲッタ
    Object3d* GetObject3d() { return object3d_.get(); }

    // sphere_ゲッタ
    SphereCollider* GetSphereCollider() { return sphere_.get(); };

    // isDead_ゲッタ
    bool ReturnIsDead() { return state_.isDead_; };

    // isDead_セッタ
    void SetIsDead(bool isDead) { state_.isDead_ = isDead; };

    // 敵情報構造体ゲッタ
    EnemyState* GetState();

    // 強制true
    bool compultionTrue();

    void SetWorldTransform(const WorldTransform& arg);



    //死亡アクション
    void DeadAction();





private:// メンバ変数
    Mesh* model_;
    std::unique_ptr<Object3d> object3d_;
    RailCameraInfo* railCameraInfo_ = nullptr;
    EnemyState state_ = {};
    Vector3 primaryPos_;    //ゲームの進行度から求める進行点
    Vector3 battleLinePos_; //敵が居座って戦闘するライン
    WorldTransform* playerWorldTransform = nullptr;
    Vector3 directionLoot_ = {};
    Vector3 offsetPos_ = {};
    float posY_ = 0;    //yのみ明示的に位置決定
    float advancedValue_ = 0.0f;    //進んだ値

   


private:// 当たり判定
    CollisionManager* collider_ = nullptr;
    std::unique_ptr <SphereCollider> sphere_ = nullptr;
    Vector3 colliderPos_ = { 0,0,0 };

    //ステータス
    const float enemyScale_ = 3.0f;
    const float spownBattlePosTimeRate_ = -0.002f;
    const float offsetBattlePosTimeRate_ = 0.006f;

    //float battlePosTimeRate_ = offsetBattlePosTimeRate_;    //進行度


    

  

    //死亡時用変数
    bool isDeathAction_ = false;
    int deathActionCount_ = 0;

    const float deathActionRotateVel_ = 0.2f;
    const float lowestPosY_ = 0.0f;
    const float fallSpeedVel_ = 0.1f;
    const float offsetBoundSpeed_ = 1.4f;
    const float deadDecelerationAcceleration_ = 0.05f;
    const float deadAtTheStartAcceleration_ = 2.f;
    const float subtractTimeRateVel_ = -0.0001f;
    const int maxDeathActionCount_ = 180;

    float deadDecelerationSpeed_ = 0.0f;
    float nowFallSpeed_ = offsetBoundSpeed_;
    float nowSubtractTimeRate_ = 0;

  

    //固定値
    const float apparancePosY_ = 10.f;  // 登場時の
    const float offsetBattlePosY_ = 0.5f;
 


};

