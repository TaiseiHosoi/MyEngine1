/**
 * @file Sprite.h
 * @brief 2D画像クラス
 */
#pragma once
#include"SpriteCommon.h"
#include <DirectXMath.h>

using namespace DirectX;


// 頂点データ構造体
struct Vertex2
{
	XMFLOAT3 pos; // xyz座標
	XMFLOAT2 uv;  // uv座標
};
//スプライト
class Sprite {
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	// 定数バッファ用データ構造体（マテリアル）
	struct ConstBufferDataMaterial {
		XMFLOAT4 color; // 色 (RGBA)
	};
	//定数バッファ用構造体（３D変換行列）
	struct ConstBufferDataTransform {
		XMMATRIX mat;	//3D変換行列
	};

	//頂点番号
	enum VertexNumber {
		LB,//左下
		LT,//左上
		RB,//右下
		RT,//右上
	};
public:
	// 初期化
	void Initialize(SpriteCommon* spritecommon_, uint32_t texturerIndex = UINT32_MAX);

	// 描画
	void Draw();

	// 更新
	void Update();

	// 描画位置セット
	void SetPozition(const XMFLOAT2& position_);

	// 描画位置ゲッタ
	const XMFLOAT2& GetPosition() const { return position; }

	// 回転角セット
	void SetRotation(float rotation_);

	// 回転角ゲット
	float GetRotation() { return rotation; }

	// カラーセット
	void SetColor(const XMFLOAT4& color_) { color = color_; }

	// カラーゲット
	XMFLOAT4 GetColor() { return color; }

	// スケールをセット
	void SetScale(const XMFLOAT3& scale_) { scale = scale_; }

	// スケールをゲット
	XMFLOAT3 GetScale() { return scale; }

	//  アンカーポイントをセット
	void SetAnchorPoint(const XMFLOAT2& anchorPoint_) { anchorPoint = anchorPoint_; }

	// アンカーポイントをゲット
	XMFLOAT2 GetAnchorPoint() { return anchorPoint; }

	// テクスチャ番号セット
	void SetTextureIndex(uint32_t texNmb) { textureIndex_ = texNmb; }

	// テクスチャ番号ゲッタ
	uint32_t GetTextureIndex() { return textureIndex_; }

	// テクスチャサイズセット
	void SetTexSize(XMFLOAT2 texSize) { textureSize = texSize; }

	// テクスチャサイズゲット
	XMFLOAT2 GetTexSize() { return textureSize; }

	// サイズのゲッタ
	XMFLOAT2 GetSize() { return size_; }

	// サイズのセッタ
	void SetSize(XMFLOAT2 size);

	// アンカーポイントゲッタ
	XMFLOAT2 GetAnchorPonit() { return anchorpoint; }

	/// 上下反転の設定
	void SetIsFlipY(bool isFlipY);

	/// 左右反転の設定
	void SetIsFlipX(bool isFlipX);

	//テクスチャサイズをイメージに合わせる
	void AdjustTextureSize();

private:
	SpriteCommon* spritecommon;
	HRESULT result;
	// 頂点データ
	Vertex2 vertices[4] = {
		// x      y     z       u     v
		{{-0.4f, -0.7f, 0.0f}, {0.0f, 1.0f}}, // 左下
		{{-0.4f, +0.7f, 0.0f}, {0.0f, 0.0f}}, // 左上
		{{+0.4f, -0.7f, 0.0f}, {1.0f, 1.0f}}, // 右下
		{{+0.4f, +0.7f, 0.0f}, {1.0f, 0.0f}}, // 右上
	};
	//XMFLOAT3 vertices[] = {
	//{ -0.5f, -0.5f, 0.0f }, // 左下
	//{ -0.5f, +0.5f, 0.0f }, // 左上
	//{ +0.5f, -0.5f, 0.0f }, // 右下
	//};
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	XMMATRIX matScale;//スケーリング行列
	XMMATRIX matWorld;
	XMMATRIX matRot;//回転行列
	XMMATRIX  matTrans;//平行移動行列

	//座標

	XMFLOAT3 scale{ 0.5f, 0.5f, 1.0f };

	XMFLOAT2 size_ = { 100.0f,100.0f };

	float rotation = 0.0f;
	XMFLOAT2 position = { 0.0f, 0.0f };

	XMFLOAT4 color = { 1,1,1,1 };

	XMFLOAT2 anchorPoint = { 0.0f,0.0f };

	ComPtr<ID3D12Resource> constBuffTransform;
	ConstBufferDataTransform* constMapTransform = nullptr;

	ConstBufferDataMaterial* constMapMaterial = nullptr;

	ComPtr <ID3D12Resource> constBuffMaterial;

	Vertex2 vertices_[4];

	Vertex2* vertMap = nullptr;

	//テクスチャ番号
	uint32_t textureIndex_ = 0;

	//テクスチャ左上座標
	XMFLOAT2 textureLeftTop = { 0.0f,0.0f };
	//テクスチャ切り出しサイズ
	XMFLOAT2 textureSize = { 100.0f,100.0f };

	XMMATRIX matProjection;

	// アンカーポイント
	XMFLOAT2 anchorpoint = { 0, 0 };

	// 頂点バッファの生成
	ComPtr<ID3D12Resource> vertBuff;

	// 左右反転
	bool isFlipX = false;
	// 上下反転
	bool isFlipY = false;
};