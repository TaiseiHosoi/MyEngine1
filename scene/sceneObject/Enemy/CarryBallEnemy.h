#pragma once
#include "Enemy.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "GameCamera.h"
#include "EnemyBoundBall.h"
#include "EnemyActionUtility.h"

class CarryBallEnemy : public Enemy {
public:
    CarryBallEnemy();
    ~CarryBallEnemy();

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
    EnemyState* GetState()override;

    //強制true
    bool compultionTrue();

    // 弾モデルセット
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
    float posY_ = 23.f;    //yのみ明示的に位置決定
    float advancedValue_ = 0.0f;    //進んだ値

    //Atk()用
    const int atkMaxMoveCount_ = 400;
    const int atkMaxFowardMoveCount_ = 150;
    int atkMoveCount_ = 0;
    float moveDifferenceValue_ = 0.0f;
    const float maxMoveDifferencePosTimeRate_ = 0.009f; // 前に動いたり後ろに動いたりする値
    const float minMoveDifferencePosTimeRate_ = -0.014f; // 前に動いたり後ろに動いたりする値



private:// 当たり判定
    CollisionManager* collider_ = nullptr;
    std::unique_ptr <SphereCollider> sphere_ = nullptr;
    Vector3 colliderPos_ = { 0,0,0 };

    //ステータス
    const float enemyScale_ = 4.5f;
    const float spownBattlePosTimeRate_ = -0.004f;
    const float offsetBattlePosTimeRate_ = 0.023f;


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
    std::list< std::unique_ptr<EnemyBoundBall>> bullets_;
    const int bulletShotDelay_ = 80;
    const int maxShotNum_ = 2;  //最大発射回数
    int nowShotNum_ = 0;    //発射回数
    const float bulRad_ = 7.f;
    const float bulSpeed_ = 0.15f;
    int nowShotDelay_ = 0;
    const float directlyBelow_ = 0.5f;

    //その他
    int rotMode_ = ROT_MODE::straight;

    //死亡時用変数
    DeathActionInfo deathActionInfo_;

    

    
    const float deadDecelerationAcceleration_ = 0.05f;
    const float deadAtTheStartAcceleration_ = 2.f;

    float deadDecelerationSpeed_ = 0.0f;
    float nowSubtractTimeRate_ = 0;


    //固定値
    const float apparancePosY_ = 10.f;  // 登場時の
    const float offsetBattlePosY_ = 0.5f;
    const float forwardEaseStrength = 2.0f;
    const float turnEaseStrength = 4.0f;
    const int maxFowardTime_ = 60;
    const int maxTurnTime_ = 10;


};
