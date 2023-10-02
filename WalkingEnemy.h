#pragma once
#include "GameCamera.h"
#include "Enemy.h"

class WalkingEnemy : public Enemy {
public:
    WalkingEnemy();
    ~WalkingEnemy();

    void Initialize(Mesh* model) override;
    void Update() override;
    void Draw(ID3D12GraphicsCommandList* cmdList) override;
    // 固有のメソッドやデータ

public: // アクセッサ
    void SetRailCameraInfo(RailCameraInfo* info) override{ railCameraInfo_ = info; };
    void SetPlayerWorldTransform(WorldTransform* worldTransform) { playerWorldTransform = worldTransform; };
    void SetOffsetVec3(const Vector3 v) { offsetPos_ = v; };

public:
    void Forward();
    void Turn();
    void Atk();

private:
    enum MOVE_PHASE {
        none,
        forward,
        turn,
        atk
    };

private:// メンバ変数
    Mesh* model_;
    std::unique_ptr<Object3d> object3d_;
    RailCameraInfo* railCameraInfo_ = nullptr;
    Vector3 primaryPos_;    //ゲームの進行度から求める進行点
    Vector3 battleLinePos_; //敵が居座って戦闘するライン
    WorldTransform* playerWorldTransform = nullptr;
    Vector3 directionLoot_ = {};
    Vector3 offsetPos_ = {};

    //phase関係
    int oldFlamePhase_ = 0;
    int nowPhase_ = 0;
    int moveCount_ = 0;

    //
    float advancedValue_ = 0.0f;    //進んだ値
    
};

