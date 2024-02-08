#pragma once
#include"EnemyBullet.h"
#include"CollisionAttribute.h"
class EnemyNormalBullet : public EnemyBullet
{
public:
    ~EnemyNormalBullet();

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

    void SetSpeed(float speed);

private:
    CollisionManager* collider = nullptr;
    std::unique_ptr<SphereCollider> sphere;
    std::unique_ptr<Object3d> object_;
    Mesh* model_ = nullptr;
    bool isDead_ = false;
    Vector3 colPos_ = {};
    float bulletSpeed_ = 2.f;
    const int maxCountSinceBirth_ = 60;
    int countSinceBirth_ = 0;




};

