/**
 * @file WinApp.h
 * @brief Window設定など管理
 */
#pragma once
#include <Windows.h>

class WinApp
{
public:// 静的メンバ関数
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
public:
	// 初期化
	void Initialize();

	// 更新
	bool ProcessMessage();

	//ウィンドウクラスを登録解除
	void Finalize();

	// getter
	// hwndゲッタ
	HWND GetHwnd()const { return hwnd; }
	// hInstanceゲッタ
	HINSTANCE GetHInstance() const { return w.hInstance; }

	// シングルトンインスタンスの取得
	static WinApp* GetInstance();

private:
	// ウィンドウハンドル
	HWND hwnd = nullptr;
	//ウィンドウクラス
	WNDCLASSEX w{};

	
public:

	//ウィンドウサイズ
	static const int window_width = 1280;	//横幅
	static const int window_height = 720;	//縦幅
	//シングルトン
	static WinApp* WinApp_;
};

