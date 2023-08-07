#pragma once
#include"WinApp.h"
#include"DirectXCommon.h"
#include"Error.h"
#include"Mesh.h"
#include"input.h"
#include"Object3d.h"
#include"Sprite.h"
#include"Audio.h"
#include"GameScene.h"
#include"ImGuiManager.h"
#include"fbxsdk.h"

const float PI = 3.14f;

//using namespace Microsoft::WRL;
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

class SummarizeEngine {
public:

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:
	WinApp* winApp_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	Audio* audio_ = nullptr;
	Input* input_ = Input::GetInstance();
	GameMain* gameScene_ = nullptr;
	ImGuiManager* imGuiManager_ = nullptr;

public:
	bool isRunningGame = true;
};