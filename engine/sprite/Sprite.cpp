#include"Sprite.h"

using namespace MyEngine;
void Sprite::Initialize(SpriteCommon* spritecommon_, std::string texStr)
{

	spritecommon = spritecommon_;


	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用



	// 頂点バッファの生成

	result = spritecommon->GetDxCommon()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&spritecommon->GetResourceDesc(), // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得

	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);

	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = spritecommon->GetSizeVB();
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);

	Update();

	{
		// ヒープ設定
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;                   // GPUへの転送用
		// リソース設定
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;   // 256バイトアラインメント
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 定数バッファの生成
		result = spritecommon->GetDxCommon()->GetDevice()->CreateCommittedResource(
			&cbHeapProp, // ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc, // リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffTransform));
		assert(SUCCEEDED(result));

		// 定数バッファのマッピング
		result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform); // マッピング
		assert(SUCCEEDED(result));
	}

	//並行投影行列の計算
	constMapTransform->mat = XMMatrixIdentity();

	// ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;                   // GPUへの転送用
	// リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;   // 256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 射影行列計算
	matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)WinApp::window_width,
		(float)WinApp::window_height, 0.0f,
		0.0f, 1.0f);

	// 定数バッファの生成
	result = spritecommon->GetDxCommon()->GetDevice()->CreateCommittedResource(
		&cbHeapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));

	// 定数バッファのマッピング

	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial); // マッピング
	assert(SUCCEEDED(result));

	// 値を書き込むと自動的に転送される
	constMapMaterial->color = XMFLOAT4(1, 0, 0, 0.5f);              // RGBAで半透明の赤
	
	//テクスチャサイズをイメージに合わせる
	if (spritecommon_->FindIndex(texStr) != UINT32_MAX) {
		
		textureIndex_ = spritecommon_->FindIndex(texStr);
		AdjustTextureSize();
		//テクスチャサイズをスプライトのサイズに適応
		size_ = textureSize;
	}

}

void Sprite::Draw()
{
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation));//Z軸周りに0度回転してから
	matTrans = XMMatrixTranslation(position.x, position.y, 0.0f);//(-50,0,0)平行移動

	matWorld = XMMatrixIdentity();//変形をリセット
	//matWorld *= matScale;//ワールド行列にスケーリングを反映
	matWorld *= matRot;//ワールド行列にスケーリングを反映
	matWorld *= matTrans;


	// 定数バッファにデータ転送
	if (SUCCEEDED(result)) {
		constMapTransform->mat = matWorld * matProjection;	// 行列の合成	
	}
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);
	if (SUCCEEDED(result)) {
		constMapMaterial->color = color;
	}

	spritecommon->SetTextureCommands(textureIndex_);

	//頂点バッファビューの設定コマンド
	spritecommon->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	// 定数バッファビュー(CBV)の設定コマンド
	spritecommon->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());



	// 定数バッファビュー(CBV)の設定コマンド
	spritecommon->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	// 描画コマンド
	spritecommon->GetDxCommon()->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0); // 全ての頂点を使って描画
}

void Sprite::Update()
{
	ComPtr<ID3D12Resource> textureBuffer = spritecommon->GetTextureBuffer(textureIndex_);

	float left = (0.0f - anchorpoint.x) * size_.x;
	float right = (1.0f - anchorpoint.x) * size_.x;
	float top = (0.0f - anchorpoint.x) * size_.y;
	float bottom = (1.0f - anchorpoint.x) * size_.y;

	if (isFlipX)
	{// 左右入れ替え
		left = -left;
		right = -right;
	}

	if (isFlipY)
	{// 上下入れ替え
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left,	bottom,	0.0f }; // 左下
	vertices[LT].pos = { left,	top,	0.0f }; // 左上
	vertices[RB].pos = { right,	bottom,	0.0f }; // 右下
	vertices[RT].pos = { right,	top,	0.0f }; // 右上

	//指定番号の画像が読み込み済みなら
	if (textureBuffer) {
		//テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

		float tex_left = textureLeftTop.x / resDesc.Width;
		float tex_right = (textureLeftTop.x + textureSize.x) / resDesc.Width;
		float tex_top = textureLeftTop.y / resDesc.Height;
		float texbottom = (textureLeftTop.y + textureSize.y) / resDesc.Height;
		//頂点UVに反映する
		vertices[LB].uv = { tex_left,texbottom };
		vertices[LT].uv = { tex_left,tex_top };
		vertices[RB].uv = { tex_right,texbottom };
		vertices[RT].uv = { tex_right,tex_top };

	}
	


	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

}

void Sprite::SetPozition(const XMFLOAT2& position_)
{
	position = position_;
	Update();
}

void Sprite::SetRotation(float rotation_)
{
	rotation = rotation_;
	Update();
}

void Sprite::SetTextureByName(std::string tex)
{
	SetTextureIndex(spritecommon->FindIndex(tex));
}

void Sprite::SetSize(const XMFLOAT2& size)
{
	size_ = size;
	Update();
}

void Sprite::SetIsFlipY(bool isFlipYArg)
{
	this->isFlipY = isFlipYArg;

	Update();
}

void Sprite::SetIsFlipX(bool isFlipXArg)
{
	this->isFlipX = isFlipXArg;

	Update();
}

void Sprite::AdjustTextureSize()
{
	ComPtr<ID3D12Resource> textureBuffer = spritecommon->GetTextureBuffer(textureIndex_);
	assert(textureBuffer);

	//テクスチャ情報取得
	D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

	textureSize.x = static_cast<float>(resDesc.Width);
	textureSize.y = static_cast<float>(resDesc.Height);
}
