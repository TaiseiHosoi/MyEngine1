﻿#include "WinApp.h"
#include"imgui_impl_win32.h"

#pragma comment(lib, "winmm.lib")

using namespace MyEngine;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wPalam, LPARAM lParam);
WinApp* WinApp::WinApp_  = nullptr;
void WinApp::Initialize() {
#pragma region WindousAPI初期化処理


	//ウィンドウクラスの設定
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;		//ウィンドウプロシージャを設定
	w.lpszClassName = "キンミラシューター";			//ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr);		//ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW);	//カーソル指定

	//ウィンドウクラスをOSに登録する
	RegisterClassEx(&w);
	//ウィンドウサイズ｛　X座標　Y座標　横幅　縦幅　｝」
	RECT wrc = { 0,0,window_width,window_height };
	//自動でサイズを補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウオブジェクトの生成
	hwnd = CreateWindow(w.lpszClassName,	//クラス名
		"KINMIRA Shooter",			//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	//標準的なウィンドウスタイル
		CW_USEDEFAULT,			//表示X座標（OS任せ）
		CW_USEDEFAULT,			//表示Y座標（OS任せ）
		wrc.right - wrc.left,	//ウィンドウ横幅
		wrc.bottom - wrc.top,	//ウィンドウ縦幅
		nullptr,				//親ウィンドウハンドル
		nullptr,				//メニュウハンドル
		w.hInstance,			//呼び出しアプリケーションハンドル
		nullptr);				//オプション

	//ウィンドウを表示状態にする
	ShowWindow(hwnd, SW_SHOW);

	timeBeginPeriod(1);

#pragma endregion WindousAPI初期化処理
}

// 更新
bool  WinApp::ProcessMessage() {

#pragma region ウィンドウメッセージ処理
	//メッセージがある？
	MSG msg{};	//メッセージ
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);	//キー入力メッセージの処理
		DispatchMessage(&msg);	//プロシージャにメッセージを送る		
	}

	//Xボタンで終了メッセージが来たらゲームループを抜ける
	if (msg.message == WM_QUIT) {
		return true;
	}

	return false;
#pragma endregion ウィンドウメッセージ処理
}

//ウィンドウクラスを登録解除
void WinApp::Finalize() {

	UnregisterClass(w.lpszClassName, w.hInstance);
}

WinApp* WinApp::GetInstance()
{
	if (WinApp_ == nullptr)
	{
		WinApp_ = new WinApp();
	}

	return WinApp_;
}

LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	//ImGui用ウィンドウプロシージャ呼び出し
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) { return true; }

	//メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		//ウィンドウが破棄された
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}