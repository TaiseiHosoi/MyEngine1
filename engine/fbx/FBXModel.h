/**
 * @file FbxModel.h
 * @brief FBXSDKのモデルクラス
 */

#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>



#pragma warning(disable: 4324)
// ノード
struct Node
{
	// 名前
	std::string name;
	// ローカルスケール
	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	// ローカル回転角
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	// ローカル移動
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	// ローカル変形行列
	DirectX::XMMATRIX transform;
	// グローバル変形行列
	DirectX::XMMATRIX globalTransform;
	// 親ノード
	Node* parent = nullptr;
	
};

class FBXModel
{
public:
	// フレンドクラス
	friend class FbxLoader;

private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	// std::を省略
	using string = std::string;
	template <class T> using vector = std::vector<T>;


public: // サブクラス
	struct Bone
	{
		//名前
		std::string name;
		//初期姿勢の逆行列
		DirectX::XMMATRIX invInitialPose;
		//クラスター(FBX側のボーン情報)
		FbxCluster* fbxCluster;
		//コンストラクタ
		Bone(const std::string& name) {
			this->name = name;
		}
	};

	//ボーンインデックスの最大数
	static const int MAX_BONE_INDICES = 4;


	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos; // xyz座標
		DirectX::XMFLOAT3 normal; // 法線ベクトル
		DirectX::XMFLOAT2 uv;  // uv座標
		UINT boneIndex[MAX_BONE_INDICES];//ボーン 番号
		float boneWeight[MAX_BONE_INDICES];//ボーン 重み
		std::vector<std::pair<UINT, float>> boneData;
	};

public:
	//デストラクタ
	~FBXModel();
	// バッファ生成
	void CreateBuffers(ID3D12Device* device);
	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	// モデルの変形行列取得
	const XMMATRIX& GetModelTransform() { return meshNode_->globalTransform; }
	//getter
	FbxScene* GetFbxScene() { return fbxScene_; }

	//ボーン配列
	std::vector<Bone> bones;
	//getter
	std::vector<Bone>& GetBones() { return bones; }

private:
	// モデル名
	std::string name;
	// ノード配列
	std::vector<Node> nodes_;
	// メッシュを持つノード
	Node* meshNode_ = nullptr;
	// 頂点データ配列
	std::vector<VertexPosNormalUv> vertices_;
	// 頂点インデックス配列
	std::vector<unsigned short> indices_;
	// アンビエント係数
	DirectX::XMFLOAT3 ambient_ = { 1,1,1 };
	// ディフューズ係数
	DirectX::XMFLOAT3 diffuse_ = { 1,1,1 };
	// テクスチャメタデータ
	DirectX::TexMetadata metadata_ = {};
	// スクラッチイメージ
	DirectX::ScratchImage scratchImg_ = {};
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff_;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff_;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff_;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_ = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_ = {};
	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV_;

	//FBXシーン
	FbxScene* fbxScene_ = nullptr;

};

