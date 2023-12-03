/**
 * @file WalkingEnemy.h
 * @brief 歩兵っぽい敵
 */
#pragma once
#include "GameCamera.h"
#include "Enemy.h"
#include"CollisionAttribute.h"
#include"CollisionManager.h"
#include"SphereCollider.h"
#include"EnemyNormalBullet.h"

class WalkingEnemy : public Enemy {
public:
    WalkingEnemy();
    ~WalkingEnemy();

    // 初期化
    void Initialize(Mesh* model) override;
    
    // 更新
    void Update() override;
    
    // 描画
    void Draw(ID3D12GraphicsCommandList* cmdList) override;
    // 固有のメソッドやデータ

public: // アクセッサ
    // レールカメラ情報セッタ
    void SetRailCameraInfo(RailCameraInfo* info) override{ railCameraInfo_ = info; };
    
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

    // Shot
    void SetBulletModel(Mesh* model);

public: //　行動
    // 突進
    void Forward();
    // ゆっくり近づく
    void Deceleration();
    // 旋回
    void Turn();
    //攻撃
    void Atk();
    //射撃
    void ShotBullet();
    void AddBullet();

    //死亡アクション
    void DeadAction();



private:
    enum MOVE_PHASE {
        none,
        forward,
        turn,
        atk
    };

    enum ROT_MODE {
        straight,
        toPlayer
    };

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

    //Atk()用
    const int atkMaxMoveCount_ = 240;
    const int atkMaxFowardMoveCount_ = 160;
    int atkMoveCount_ = 0;
    float moveDifferenceValue_ = 0.0f;
    const float maxMoveDifferencePosTimeRate_ = 0.007f; // 前に動いたり後ろに動いたりする値
    const float minMoveDifferencePosTimeRate_ = -0.004f; // 前に動いたり後ろに動いたりする値

    

private:// 当たり判定
    CollisionManager* collider_ = nullptr;
    std::unique_ptr <SphereCollider> sphere_ = nullptr;
    Vector3 colliderPos_ = { 0,0,0 };
    
    //ステータス
    const float enemyScale_ = 5.0f;
    const float spownBattlePosTimeRate_ = -0.002f;
    const float offsetBattlePosTimeRate_ = 0.006f;

    //float battlePosTimeRate_ = offsetBattlePosTimeRate_;    //進行度


    //phase関係
    int oldFlamePhase_ = 0;
    int nowPhase_ = 0;
    int moveCount_ = 0;
    int turnCount_ = 0;
    float adjustFAngle_ = 0.0f; // 向いている方向を可変する為の変数
    const float maxAdjustFAngle_ = 180.f;
    const float minAdjustFAngle_ = 0;

    //射撃関連
    Mesh* bulletModel_ = nullptr;
    std::list< std::unique_ptr<EnemyNormalBullet>> bullets_;
    const int bulletShotDelay_ = 40;
    int nowShotDelay_ = 0;

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

    //その他
    int rotMode_ = ROT_MODE::straight;

    //固定値
    const float apparancePosY_ = 10.f;  // 登場時の
    const float offsetBattlePosY_ = 0.5f;
    const float forwardEaseStrength = 2.0f;
    const float turnEaseStrength = 4.0f;
    const int maxFowardTime_ = 60;
    const int maxTurnTime_ = 10;

    
};

