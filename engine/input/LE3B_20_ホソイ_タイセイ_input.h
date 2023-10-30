/**
 * @file Input.h
 * @brief インプット関連クラス
 */
#pragma once
#define DIRECTINPUT_VERSION	0x0800
#include <wrl.h>
#include <Windows.h>
#include <dinput.h>
#include "WinApp.h"

enum MOUSE_KEY {
	LEFT,
	RIGHT
};
class Input
{
public:
    // 初期化
    void Initialize(WinApp* winApp);

    // 更新
    void Update();

    // キーの押下をチェック
    bool PushKey(BYTE keyNumber);
    // キーのトリガーをチェック
    bool TriggerKey(BYTE keyNumber);
    bool ReleaseKey(BYTE keyNumber);

    // マウス操作
    bool PushMouseButton(unsigned char mouseButtons);
    bool TriggerMouseButton(unsigned char mouseButtons);
    bool ReleaseMouseButton(unsigned char mouseButtons);

    static Input* GetInstance();

private:
    // プライベートコンストラクタ（外部からのインスタンス化を禁止）
    Input();

    // プライベートデストラクタ（外部からの削除を禁止）
    ~Input();

    // プライベートコピーコンストラクタ（コピーを禁止）
    Input(const Input& other);

    // プライベートコピー代入演算子（コピー代入を禁止）
    Input& operator=(const Input& other);

    // windowsAPI
    WinApp* winApp_ = nullptr;

    template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
    // キーボードデバイスの生成
    ComPtr<IDirectInputDevice8> keyboard = nullptr;

    // DirectInputの初期化
    ComPtr<IDirectInput8> directInput = nullptr;

    // マウス入力初期化
    ComPtr<IDirectInputDevice8> mouse = nullptr;

    DIMOUSESTATE mouseButton;
    DIMOUSESTATE oldMouseButton;

    BYTE keys[256] = {};
    BYTE keysPre[256] = {};
};

