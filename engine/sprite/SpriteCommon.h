/**
 * @file SpriteCommon.h
 * @brief 2Dスプライトの管理クラス
 */
#pragma once
#include"DirectXCommon.h"
#include <DirectXTex.h>
#include <array>
#include <string>

using namespace DirectX;


//スプライト共通部分
class SpriteCommon {
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	struct Vertex
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT2 uv;  // uv座標
	};
	// 定数バッファ用データ構造体（マテリアル）
	struct ConstBufferDataMaterial {
		XMFLOAT4 color; // 色 (RGBA)
	};
	//定数バッファ用構造体（３D変換行列）
	struct ConstBufferDataTransform {
		XMMATRIX mat;	//3D変換行列
	};
public:
	// 初期化
	void Initialize(DirectXCommon* dxcommon);

	DirectXCommon* GetDxCommon() { return dxcommon_; }

	ComPtr<ID3D12RootSignature> GetRootSignature() { return rootSignature_.Get(); }

	ComPtr<ID3D12PipelineState> GetPipelineState() { return pipelineState_.Get(); }

	ID3D12DescriptorHeap* GetSrvHeap() { return srvHeap.Get(); }

	D3D12_CPU_DESCRIPTOR_HANDLE GetSrvHandle() { return srvHandle_; }

	UINT GetSizeVB() { return sizeVB_; }

	D3D12_RESOURCE_DESC& GetResourceDesc() { return resDesc_; }

	// テクスチャをロード
	void LoadTexture(uint32_t index, const std::string& fileName);

	// テクスチャのコマンドセット
	void SetTextureCommands(uint32_t index);

	// texBuff[index]ゲッタ
	Microsoft::WRL::ComPtr<ID3D12Resource> GetTexBuff(uint32_t index) { return texBuff[index]; }

	// texBuff[index].getゲッタ
	ComPtr<ID3D12Resource> GetTextureBuffer(uint32_t index)const { return texBuff[index].Get(); }

	// スプライトの描画前処理
	void SpritePreDraw();

	// スプライトの描画後処理
	void SpritePostDraw();

	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap;
private:
	// 頂点データ
	Vertex vertices_[4] = {
		// x      y     z       u     v
		{{-0.4f, -0.7f, 0.0f}, {0.0f, 1.0f}}, // 左下
		{{-0.4f, +0.7f, 0.0f}, {0.0f, 0.0f}}, // 左上
		{{+0.4f, -0.7f, 0.0f}, {1.0f, 1.0f}}, // 右下
		{{+0.4f, +0.7f, 0.0f}, {1.0f, 0.0f}}, // 右上
	};
	// SRVの最大個数
	static const size_t kMaxSRVCount_ = 2056;

	DirectXCommon* dxcommon_ = nullptr;
	ID3DBlob* vsBlob_ = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob_ = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob_ = nullptr; // エラーオブジェクト
	HRESULT result_;


	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_;


	// パイプランステートの生成
	ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;

	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount_>texBuff;

	//横方向ピクセル数
	const size_t textureWidth_ = 256;
	//縦方向ピクセル数
	const size_t textureHeight_ = 256;
	//配列の要素数
	const size_t imageDataCount_ = textureWidth_ * textureHeight_;

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle_;

	TexMetadata metadata_{};
	ScratchImage scratchImg_{};
	D3D12_HEAP_PROPERTIES textureHeapProp_{};
	D3D12_RESOURCE_DESC textureResourceDesc_{};

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc_{};
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_{};
	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc_ = {};

	UINT sizeVB_;

	D3D12_RESOURCE_DESC resDesc_{};

	//デフォルトテクスチャ格納ディレクトリ
	static std::string kDefaultTextureDirectoryPath_;

	UINT incrementSize_;


};