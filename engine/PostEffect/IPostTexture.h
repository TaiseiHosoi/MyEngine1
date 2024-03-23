#pragma once
#include "DirectXCommon.h"
#include "DirectXTex.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"
#include "Matrix4.h"
#include <array>
#include"input.h"
class IPostTexture
{
public:


	virtual void Initialize(DirectXCommon* dxCommon) = 0;

	virtual void Finalize() = 0;

	/// <summary>
	/// パイプライン生成
	/// </summary>
	virtual void CreatGraphicsPipelineState() = 0;

	/// <summary>
	/// シーン描画前処理
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	virtual void PreDrawScene(ID3D12GraphicsCommandList* cmdList) = 0;

	virtual void Draw(ID3D12GraphicsCommandList* cmdList) = 0;

	/// <summary>
	/// シーン描画後処理
	/// </summary>
	/// <param name="cmdList">コマンド処理</param>
	virtual void PostDrawScene() = 0;

protected:

	const float clearColor[4] = { 0,0,0,0 };;

	ID3D12Device* device_;

	ID3D12GraphicsCommandList* commandList;


	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;

	//頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView;
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;

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