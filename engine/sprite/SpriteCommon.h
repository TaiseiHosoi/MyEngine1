/**
 * @file SpriteCommon.h
 * @brief 2Dスプライトの管理クラス
 */
#pragma once
#include"DirectXCommon.h"
#include <DirectXTex.h>
#include <array>
#include <string>
#include <locale>
#include <codecvt>
#include<map>

using namespace DirectX;
using namespace MyEngine;

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
public:	//アクセッサ
	

	DirectXCommon* GetDxCommon() { return dxcommon_; }

	ComPtr<ID3D12RootSignature> GetRootSignature() { return rootSignature.Get(); }

	ComPtr<ID3D12PipelineState> GetPipelineState() { return pipelineState.Get(); }

	ID3D12DescriptorHeap* GetSrvHeap() { return srvHeap.Get(); }

	D3D12_CPU_DESCRIPTOR_HANDLE GetSrvHandle() { return srvHandle; }

	UINT GetSizeVB() { return sizeVB; }

	D3D12_RESOURCE_DESC& GetResourceDesc() { return resDesc; }

	std::vector<std::string>GetSpriteNames() { return spriteNames_; }

	// texBuff[index]ゲッタ
	Microsoft::WRL::ComPtr<ID3D12Resource> GetTexBuff(uint32_t index) { return texBuff[index]; }

	// texBuff[index].getゲッタ
	ComPtr<ID3D12Resource> GetTextureBuffer(uint32_t index)const { return texBuff[index].Get(); }




public://関数
	// 初期化
	void Initialize(DirectXCommon* dxcommon);

	/// <summary>
	/// テクスチャをロード
	/// </summary>
	/// <param name="texName"><識別用の名前>
	/// <param name="fileName"><画像のファイルネーム>
	void LoadTexture(const std::string& texName,const std::string& fileName);

	// テクスチャのコマンドセット
	void SetTextureCommands(uint32_t index);

	// テクスチャのファイルネームセパレート
	void SeparateFilePath(const std::string& filePath);

	// std::vector<std::string>から文字列を検索してインデックスを返す関数
	uint32_t FindIndex( const std::string& target);
	
	// スプライトの描画前処理
	void SpritePreDraw();

	// スプライトの描画後処理
	void SpritePostDraw();

	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap;
private:
	// 頂点データ
	Vertex vertices[4] = {
		// x      y     z       u     v
		{{-0.4f, -0.7f, 0.0f}, {0.0f, 1.0f}}, // 左下
		{{-0.4f, +0.7f, 0.0f}, {0.0f, 0.0f}}, // 左上
		{{+0.4f, -0.7f, 0.0f}, {1.0f, 1.0f}}, // 右下
		{{+0.4f, +0.7f, 0.0f}, {1.0f, 0.0f}}, // 右上
	};
	// SRVの最大個数
	static const size_t kMaxSRVCount = 2056;

	DirectXCommon* dxcommon_ = nullptr;
	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト
	HRESULT result;


	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;


	// パイプランステートの生成
	ComPtr<ID3D12PipelineState> pipelineState = nullptr;

	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount>texBuff;

	//横方向ピクセル数
	const size_t textureWidth = 256;
	//縦方向ピクセル数
	const size_t textureHeight = 256;
	//配列の要素数
	const size_t imageDataCount = textureWidth * textureHeight;

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	D3D12_RESOURCE_DESC textureResourceDesc{};

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};

	UINT sizeVB;

	D3D12_RESOURCE_DESC resDesc{};

	//デフォルトテクスチャ格納ディレクトリ
	static std::string kDefaultTextureDirectoryPath;

	UINT incrementSize;

	///セパレート用(一時保存)
	//ディレクトリパス
	std::string directoryPath_;
	//ファイル名
	std::string fileName_;
	//ファイル拡張子
	std::string fileExt_;

	//番号とスプライトをマップ
	std::map<std::string,int> mapSpriteNum_;

	//スプライトの引き出す名前
	std::vector<std::string> spriteNames_;

	//テクスチャバッファの最後尾番号
	int texBuffEndNum_ = 0;



};