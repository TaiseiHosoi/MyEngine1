#pragma once
#include "Enemy.h"
class BasicEnemy :
    public Enemy
{
    void Initialize(Mesh* Model) override;

    void Update() override;

    void Draw(ID3D12GraphicsCommandList* cmdList) override;
    // 基本的な敵に特有の追加機能
};

