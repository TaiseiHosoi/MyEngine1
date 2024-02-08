#pragma once
#include"EnemyBullet.h"
#include"CollisionAttribute.h"
class EnemyBoundBall : public EnemyBullet
{
public:
    ~EnemyBoundBall();

    // 初期化
    void Initialize(Mesh* model, const Vector3& setPos, const Vector3& setRot);
    // 更新
    void Update();
    // 描画
    void Draw(ID3D12GraphicsCommandList* cmdList);

public:
    // 球コライダーゲッタ
    SphereCollider* GetSphereCollider() { return sphere.get(); };

    // 死亡時フラグゲッタ
    bool ReturnIsDead() { return isDead_; };

    // 死亡時フラグセッタ
    void SetIsDead(bool isDead) { isDead_ = isDead; };

    void SetRadius(float rad);


private:
    CollisionManager* collider = nullptr;
    std::unique_ptr<SphereCollider> sphere;
    std::unique_ptr<Object3d> object_;
    Mesh* model_ = nullptr;
    bool isDead_ = false;
    Vector3 colPos_ = {};

    //生まれてから何Fか
    const int maxCountSinceBirth_ = 200;
    int countSinceBirth_ = 0;   

    //挙動
    const float offsetBoundSpeed_ = 1.2f;
    const float gravityVel_ = 0.04f;
    const float lowestPos_ = 0.1f;
    float nowSpeed_ = 0.0f;
    int boundCount_ = 0;    //バウンドした回数
    const float rollSpeed_ = -0.06f;


};