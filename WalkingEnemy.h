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
    Vector3 detailAdjustV;  //いるべき位置を調整する値A
    WorldTransform* playerWorldTransform = nullptr;

    int oldFlamePhase_ = 0;
    int nowPhase_ = 0;
    int moveCount_ = 0;

    
};

