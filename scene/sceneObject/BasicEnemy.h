#pragma once
#include "Enemy.h"
class BasicEnemy :
    public Enemy
{
    void Initialize(Mesh* Model) override;

    void Update() override;

    void Draw(ID3D12GraphicsCommandList* cmdList) override;
    // ��{�I�ȓG�ɓ��L�̒ǉ��@�\
};

