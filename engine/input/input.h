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
#include"GamePad.h"

using namespace MyEngine;

enum MOUSE_KEY {
	LEFT,
	RIGHT
};


class Input
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号（DIK_0 等）</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);
	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号（DIK_0 等）</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber);
	bool ReleaseKey(BYTE keyNumber);

	/// <summary>
	/// マウス操作
	/// </summary>
	/// <param name="mouseButtons"></param>
	/// <returns></returns>
	bool PushMouseButton(unsigned char mouseButtons);
	bool TriggerMouseButton(unsigned char mouseButtons);
	bool ReleaseMouseButton(unsigned char mouseButtons);

	void SetIsDontInput(bool isDontInput);

	static Input* GetInstance();

public:


	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//----- コントローラ- ------//

	/// <summary>
	/// コントローラーボタンのトリガー入力
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>押したか</returns>
	bool PButtonTrigger(GamePadButton button);

	/// <summary>
	/// コントローラースティックのトリガー入力
	/// </summary>
	/// <param name="stickInput">コントローラースティック方向</param>
	/// <param name="deadRange">デッドゾーンの範囲</param>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns>倒したかどうか</returns>
	bool PStickTrigger(GamePadStick stickInput, const float& deadRange = 0.3f, const Vector2& deadRate = { 1.0f,1.0f });

	/// <summary>
	/// コントローラーボタンの入力
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>押したか</returns>
	bool ButtonInput(GamePadButton button);

	/// <summary>
	/// コントローラースティックの入力
	/// </summary>
	/// <param name="stickInput">コントローラースティック方向</param>
	/// <param name="deadRange">デッドゾーンの範囲初期値0.3f</param>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns>倒したかどうか</returns>
	bool StickInput(GamePadStick stickInput, const float& deadRange = 0.3f, const Vector2& deadRate = { 1.0f,1.0f });


	/// <summary>
	/// コントローラー左スティックの入力
	/// </summary>
	/// <param name="deadRange">デッドゾーンの範囲初期値0.3f</param>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns>倒したかどうか</returns>
	bool LeftStickInput(const float& deadRange = 0.3f);

	/// <summary>
	/// コントローラーボタンの離した瞬間
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>離したか</returns>
	bool ButtonOffTrigger(GamePadButton button);

	/// <summary>
	/// コントローラースティックの離した瞬間
	/// </summary>
	/// <param name="stickInput">コントローラースティック方向</param>
	/// <param name="deadRange">デッドゾーンの範囲初期値0.3f</param>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns>離したか</returns>
	bool StickOffTrigger(GamePadStick stickInput, const float& deadRange = 0.3f, const Vector2& deadRate = { 1.0f,1.0f });

	/// <summary>
	/// コントローラーの左スティックのベクトル
	/// </summary>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns>ベクトル</returns>
	Vector2 GetLeftStickVec(const Vector2& deadRate = { 1.0f,1.0f });

	/// <summary>
	/// コントローラーの右スティックのベクトル
	/// </summary>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns>ベクトル</returns>
	Vector2 GetRightStickVec(const Vector2& deadRate = { 1.0f,1.0f });

	/// <summary>
	/// コントローラーを振動させる
	/// </summary>
	/// <param name="power">振動の強さ0.0f～1.0f</param>
	/// <param name="span">振動の時間フレーム</param>
	void ShakeGamePad(const float& power, const int& span);

private:
	// windowsAPI
	WinApp* winApp_ = nullptr;
	// キーボードデバイスの生成
	ComPtr <IDirectInputDevice8> keyboard = nullptr;
	// DirectInputの初期化
	ComPtr<IDirectInput8> directInput = nullptr;
	// マウス入力初期化
	ComPtr<IDirectInputDevice8> mouse = nullptr;

	DIMOUSESTATE mouseButton;
	DIMOUSESTATE oldMouseButton;

	BYTE keys[256] = {};
	BYTE keysPre[256] = {};

	bool isDontInput_ = false;	//キー入力を受け付けなくするか

	GamePad* gamePad_ = nullptr;
};

