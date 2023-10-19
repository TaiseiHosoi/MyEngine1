/**
 * @file DirectXCommon.h
 * @brief DirectXコマンドなどを引き出す
 */
#pragma once

#include <Windows.h>
#include <cstdlib>
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include<chrono>

#include "WinApp.h"


class DirectXCommon
{
public:
	// 初期化 
	// クラス初期化
	void Initialize(WinApp* winApp);
	// デバイス初期化
	void InitializeDevice();
	// commandの初期化
	void InitializeCommand();
	// スワップチェイン初期化
	void InitializeSwapchain();
	// レンダーターゲットビュー初期化
	void InitializeRenderTargetView();
	// 深度バッファ初期化
	void InitializeDepthBuffer();
	// フェンス設定初期化
	void InitializeFence();

	// 描画前処理
	void PreDraw();
	// 描画後処理
	void PostDraw();
	// デバイスゲッタ
	ID3D12Device* GetDevice() const { return device_.Get(); }
	// コマンドリストヘッダ
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList_.Get(); }
	//バックバッハの数をw取得
	size_t GetBackBufferCount() const { return backBuffers_.size(); }
private:
	void InitializeFixFPS();	//初期化
	void UpdateFixFPS();	//FPS固定更新
	//記録時間
	std::chrono::steady_clock::time_point reference_;

private:
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
	Microsoft::WRL::ComPtr < IDXGISwapChain4> swapChain_;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	Microsoft::WRL::ComPtr < ID3D12CommandAllocator> commandAllocator_;
	Microsoft::WRL::ComPtr < ID3D12GraphicsCommandList> commandList_;
	Microsoft::WRL::ComPtr < ID3D12CommandQueue> commandQueue_;
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> rtvHeap_;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};
	Microsoft::WRL::ComPtr <ID3D12Resource> depthBuff_;
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> dsvHeap_;
	Microsoft::WRL::ComPtr <ID3D12Fence> fence_;
	UINT64 fenceVal_ = 0;
	D3D12_RESOURCE_BARRIER barrierDesc_{};


	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers_;


	WinApp* winApp_ = nullptr;

public:


};

