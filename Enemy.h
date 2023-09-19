#pragma once
#include"Mesh.h"
#include"Object3d.h"
#include"GameCamera.h"

class Enemy
{
public:
    virtual void Initialize(Mesh* Model) = 0;
    virtual void Update() = 0;
    virtual void Draw(ID3D12GraphicsCommandList* cmdList) = 0;
    virtual void SetRailCameraInfo(RailCameraInfo* info) = 0;
    // その他の敵の共通機能

};

