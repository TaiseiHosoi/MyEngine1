#pragma once

#include "DirectXCommon.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"
#include "Matrix4.h"
#include"input.h"
class MultiTex
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

    void Initialize(DirectXCommon* dxCommon);

    void Finalize();

    /// <summary>
    /// パイプライン生成
    /// </summary>
    void CreatGraphicsPipelineState();

    /// <summary>
    /// シーン描画前処理
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    void PreDrawScene(ID3D12GraphicsCommandList* cmdList, int num);

    void Draw(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// シーン描画後処理
    /// </summary>
    /// <param name="cmdList">コマンド処理</param>
    void PostDrawScene(int num);

private:

    const float clearColor[4] = { 0,0,0,0 };;

    ID3D12Device* device_;

    ID3D12GraphicsCommandList* commandList;

    VertexPosUv vertices[4];

    VertexPosUv* vertMap;

    Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;

    Microsoft::WRL::ComPtr<ID3D12Resource> constBuffResourceB1;

    //頂点バッファビューの作成
    D3D12_VERTEX_BUFFER_VIEW vbView;
    Microsoft::WRL::ComPtr<ID3D12Resource> texBuff[2];

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    //深度バッファ
    Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
    //RTV用のデスクリプタヒープ
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV用のデスクリプタヒープ
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

    Input* input_ = nullptr;
};