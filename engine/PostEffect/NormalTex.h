#pragma once
#include "DirectXCommon.h"
#include"DirectXTex.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"
#include "Matrix4.h"
#include"input.h"
#include<array>
#include"IPostTexture.h"

class NormalTex final : IPostTexture
{
public:

    /// <summary>
    /// 頂点データ構造体
    /// </summary>
    struct VertexPosUv {
        Vector3 pos; // xyz座標
        Vector2 uv;  // uv座標
    };

    /// <summary>
    /// 定数バッファ用データ構造体
    /// </summary>
    struct ConstBufferData {
        Vector4 color; // 色 (RGBA)
        Matrix4 mat;   // ３Ｄ変換行列
    };

    struct ConstBufferDataB1
    {
        float yShiftVal;    //広さ
    };

    void Initialize(DirectXCommon* dxCommon) override;

    void Finalize() override;

    /// <summary>
    /// パイプライン生成
    /// </summary>
    void CreatGraphicsPipelineState() override;

    /// <summary>
    /// シーン描画前処理
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    void PreDrawScene(ID3D12GraphicsCommandList* cmdList) override;

    void Draw(ID3D12GraphicsCommandList* cmdList) override;

    /// <summary>
    /// シーン描画後処理
    /// </summary>
    /// <param name="cmdList">コマンド処理</param>
    void PostDrawScene() override;

private:

    std::array < VertexPosUv, 4 > vertices;

    VertexPosUv* vertMap = nullptr;

    ConstBufferDataB1* constBuffDataB1;

    Microsoft::WRL::ComPtr<ID3D12Resource> constBuffResourceB1;

    float yShiftVal_ = 0.00001f;
};

