#pragma once
#include"Mesh.h"
#include"Object3d.h"
#include"GameCamera.h"
#include"SphereCollider.h"
#include"CollisionManager.h"

class EnemyBullet
{
public:
    // 初期化
    virtual void Initialize(Mesh* model, const Vector3& setPos, const Vector3& setRot) = 0;
    // 更新
    virtual void Update() = 0;
    // 描画
    virtual void Draw(ID3D12GraphicsCommandList* cmdList) = 0;



};

