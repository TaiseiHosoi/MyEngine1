#pragma once
#include"Mesh.h"
#include"Object3d.h"

class Enemy
{
public:
    virtual void Initialize(Mesh* Model) = 0;
    virtual void Update() = 0;
    virtual void Draw(ID3D12GraphicsCommandList* cmdList) = 0;
    // その他の敵の共通機能
};

