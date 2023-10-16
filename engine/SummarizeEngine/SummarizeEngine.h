/**
 * @file SummarizeEngine.h
 * @brief エンジン内の必要クラスやメソッドを一括管理するクラス
 */
#pragma once
#include"WinApp.h"
#include"DirectXCommon.h"
#include"Error.h"
#include"Mesh.h"
#include"input.h"
#include"Object3d.h"
#include"Sprite.h"
#include"Audio.h"
#include"Application.h"
#include"ImGuiManager.h"
#include"fbxsdk.h"

const float PI = 3.14f;

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

class SummarizeEngine {
public:

	// 初期化
	void Initialize();

	// 更新
	void Update();
	
	//描画
	void Draw();

	// 終了時実行処理
	void Finalize();

private:
	WinApp* winApp_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	Audio* audio_ = nullptr;
	Input* input_ = Input::GetInstance();
	Application* application_ = nullptr;
	ImGuiManager* imGuiManager_ = nullptr;

public:
	bool isRunningGame = true;
};