#pragma once
#include"Error.h"
#include"input.h"
#include <wrl.h>
#include"Vector3.h"

#include <d3d12.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <DirectXmath.h>
#include <d3dcompiler.h>

#include<wrl.h>

#include "DirectXCommon.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler")

#include <string.h>

using namespace std;

using namespace DirectX;







//struct Object3d {
//	ComPtr<ID3D12Resource> constBuffB0;
//	ComPtr<ID3D12Resource> constBuffB1;
//
//	ConstBufferDataTransform* constMapTransform = nullptr;
//
//	XMFLOAT3 scale = { 1,1,1 };
//	XMFLOAT3 rotation = { 0,0,0 };
//	XMFLOAT3 position = { 0,0,0 };
//
//	XMMATRIX matWorld{};
//
//	Object3d* parent = nullptr;
//
//
//};






class Mesh
{
public:
	// 定数バッファ用データ構造体B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;	//アンビエント係数
		float pad1;			//パディング
		XMFLOAT3 diffuse;	//ディフューズ係数
		float pad2;			//パディング
		XMFLOAT3 specular;	//スペキュラー係数
		float alpha;		//アルファ
	};
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

	//マテリアル
	struct Material {
		std::string name;	//マテリアル名
		XMFLOAT3 ambient;	//アンビエント影響度
		XMFLOAT3 diffuse;	//ディフューズ影響度
		XMFLOAT3 specular;	//スペキュラー影響度
		float alpha;		//アルファ
		std::string textureFilename;	//テクスチャファイル名
		//コンストラクタ
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

private:
	
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	

	//デバイス
	static ID3D12Device* device;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView;
	// 頂点データ配列
	std::vector<VertexPosNormalUv> vertices;
	// 頂点インデックス配列
	std::vector<unsigned short>indices;
	//マテリアル
	Material material;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff;
	// 定数バッファ(マテリアル)
	ComPtr<ID3D12Resource> constBuffB1;



	
public:
	//アクセッサ
	static void SetDevice(ID3D12Device* device);

public:

	Mesh();
	~Mesh();

	void StaticInit(ID3D12Device* DXDevice);
	//void Init();
	void Update( Input* input);
	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial);


	void LoadModel(const char* fileName);	//LoadFromObjに移行
	void LoadModel();

	//バッファの生成
	void CreateBuffer();
	//でスクリプタヒープの初期化
	void InitializeDescriptorHeap();
	//マテリアル読み込み
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	//テクスチャ読み込み
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);
	//モデル読み込み関連
	void LoadFromObjInternal(const std::string& modelname);
	static Mesh* LoadFromOBJ(const std::string& modelname);
};
