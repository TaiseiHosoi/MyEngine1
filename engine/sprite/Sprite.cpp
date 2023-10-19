#include"Sprite.h"

void Sprite::Initialize(SpriteCommon* spritecommon_, uint32_t textureIndex)
{

	spritecommon_ = spritecommon_;


	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用



	// 頂点バッファの生成

	result_ = spritecommon_->GetDxCommon()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&spritecommon_->GetResourceDesc(), // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result_));
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得

	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result_));
	// 全頂点に対して
	for (int i = 0; i < _countof(vertices_); i++) {
		vertMap_[i] = vertices_[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);

	// GPU仮想アドレス
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView_.SizeInBytes = spritecommon_->GetSizeVB();
	// 頂点1つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(vertices_[0]);

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
		result_ = spritecommon_->GetDxCommon()->GetDevice()->CreateCommittedResource(
			&cbHeapProp, // ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc, // リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffTransform_));
		assert(SUCCEEDED(result_));

		// 定数バッファのマッピング
		result_ = constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform_); // マッピング
		assert(SUCCEEDED(result_));
	}

	//並行投影行列の計算
	constMapTransform_->mat = XMMatrixIdentity();

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
	matProjection_ = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)WinApp::window_width,
		(float)WinApp::window_height, 0.0f,
		0.0f, 1.0f);

	// 定数バッファの生成
	result_ = spritecommon_->GetDxCommon()->GetDevice()->CreateCommittedResource(
		&cbHeapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial_));
	assert(SUCCEEDED(result_));

	// 定数バッファのマッピング

	result_ = constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial_); // マッピング
	assert(SUCCEEDED(result_));

	// 値を書き込むと自動的に転送される
	constMapMaterial_->color = XMFLOAT4(1, 0, 0, 0.5f);              // RGBAで半透明の赤

	//テクスチャサイズをイメージに合わせる
	if (textureIndex != UINT32_MAX) {
		textureIndex_ = textureIndex;
		AdjustTextureSize();
		//テクスチャサイズをスプライトのサイズに適応
		size_ = textureSize_;
	}

}

void Sprite::Draw()
{
	matRot_ = XMMatrixIdentity();
	matRot_ *= XMMatrixRotationZ(XMConvertToRadians(rotation_));//Z軸周りに0度回転してから
	matTrans_ = XMMatrixTranslation(position_.x, position_.y, 0.0f);//(-50,0,0)平行移動

	matWorld_ = XMMatrixIdentity();//変形をリセット
	//matWorld *= matScale;//ワールド行列にスケーリングを反映
	matWorld_ *= matRot_;//ワールド行列にスケーリングを反映
	matWorld_ *= matTrans_;


	// 定数バッファにデータ転送
	if (SUCCEEDED(result_)) {
		constMapTransform_->mat = matWorld_ * matProjection_;	// 行列の合成	
	}
	result_ = constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial_);
	if (SUCCEEDED(result_)) {
		constMapMaterial_->color = color_;
	}

	spritecommon_->SetTextureCommands(textureIndex_);

	//頂点バッファビューの設定コマンド
	spritecommon_->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	// 定数バッファビュー(CBV)の設定コマンド
	spritecommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());



	// 定数バッファビュー(CBV)の設定コマンド
	spritecommon_->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());

	// 描画コマンド
	spritecommon_->GetDxCommon()->GetCommandList()->DrawInstanced(_countof(vertices_), 1, 0, 0); // 全ての頂点を使って描画
}

void Sprite::Update()
{
	ComPtr<ID3D12Resource> textureBuffer = spritecommon_->GetTextureBuffer(textureIndex_);

	float left = (0.0f - anchorpoint_.x) * size_.x;
	float right = (1.0f - anchorpoint_.x) * size_.x;
	float top = (0.0f - anchorpoint_.x) * size_.y;
	float bottom = (1.0f - anchorpoint_.x) * size_.y;

	if (isFlipX_)
	{// 左右入れ替え
		left = -left;
		right = -right;
	}

	if (isFlipY_)
	{// 上下入れ替え
		top = -top;
		bottom = -bottom;
	}

	vertices_[LB].pos = { left,	bottom,	0.0f }; // 左下
	vertices_[LT].pos = { left,	top,	0.0f }; // 左上
	vertices_[RB].pos = { right,	bottom,	0.0f }; // 右下
	vertices_[RT].pos = { right,	top,	0.0f }; // 右上

	//指定番号の画像が読み込み済みなら
	if (textureBuffer) {
		//テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

		float tex_left = textureLeftTop_.x / resDesc.Width;
		float tex_right = (textureLeftTop_.x + textureSize_.x) / resDesc.Width;
		float tex_top = textureLeftTop_.y / resDesc.Height;
		float texbottom = (textureLeftTop_.y + textureSize_.y) / resDesc.Height;
		//頂点UVに反映する
		vertices_[LB].uv = { tex_left,texbottom };
		vertices_[LT].uv = { tex_left,tex_top };
		vertices_[RB].uv = { tex_right,texbottom };
		vertices_[RT].uv = { tex_right,tex_top };

	}
	


	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap_);
	if (SUCCEEDED(result_)) {
		memcpy(vertMap_, vertices_, sizeof(vertices_));
		vertBuff_->Unmap(0, nullptr);
	}

}

void Sprite::SetPozition(const XMFLOAT2& position_)
{
	position_ = position_;
	Update();
}

void Sprite::SetRotation(float rotation_)
{
	rotation_ = rotation_;
	Update();
}

void Sprite::SetSize(XMFLOAT2 size)
{
	size_ = size;
	Update();
}

void Sprite::SetIsFlipY(bool isFlipYArg)
{
	this->isFlipY_ = isFlipYArg;

	Update();
}

void Sprite::SetIsFlipX(bool isFlipXArg)
{
	this->isFlipX_ = isFlipXArg;

	Update();
}

void Sprite::AdjustTextureSize()
{
	ComPtr<ID3D12Resource> textureBuffer = spritecommon_->GetTextureBuffer(textureIndex_);
	assert(textureBuffer);

	//テクスチャ情報取得
	D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

	textureSize_.x = static_cast<float>(resDesc.Width);
	textureSize_.y = static_cast<float>(resDesc.Height);
}
