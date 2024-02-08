#pragma once
#pragma warning(push)
#include <windows.h>
#pragma warning(pop)
#include "Vector2.h"
#include <xinput.h>
#pragma comment (lib, "xinput.lib")

enum GamePadButton
{
	B = XINPUT_GAMEPAD_B,
	A = XINPUT_GAMEPAD_A,
	X = XINPUT_GAMEPAD_X,
	Y = XINPUT_GAMEPAD_Y,
	START = XINPUT_GAMEPAD_START,
	BACK = XINPUT_GAMEPAD_BACK,
	LB = XINPUT_GAMEPAD_LEFT_SHOULDER,
	RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,
	LT,
	RT
};

enum GamePadStick
{
	L_UP, L_DOWN, L_LEFT, L_RIGHT,
	R_UP, R_DOWN, R_LEFT, R_RIGHT, XBOX_STICK_NUM
};


class GamePad
{
private:

	XINPUT_STATE xinputState{};
	XINPUT_STATE oldXinputState{};

	//コントローラー振動強さ
	float shakePower = 0.0f;
	//コントローラー振動長さ(フレーム数)
	int shakeTimer = 0;

	//デッドゾーンに入っているか(DeadRate : デッドゾーン判定の度合い、1.0fだとデフォルト)
	bool StickInDeadZone(Vector2& thumb, const Vector2& deadRate);

	//コピーコンストラクタ・代入演算子削除
	GamePad& operator=(const GamePad&) = delete;
	GamePad(const GamePad&) = delete;

	//最高入力強度
	const float STICK_INPUT_MAX = 32768.0f;

public:
	//コンストラクタ・デストラクタ
	GamePad();
	~GamePad();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// コントローラーボタンのトリガー入力
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>押したか</returns>
	bool ButtonTrigger(GamePadButton button);

	/// <summary>
	/// コントローラースティックのトリガー入力
	/// </summary>
	/// <param name="stickInput">コントローラースティック方向</param>
	/// <param name="deadRange">デッドゾーンの範囲</param>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns>倒したかどうか</returns>
	bool StickTrigger(GamePadStick stickInput, const float& deadRange = 0.3f, const Vector2& deadRate = { 1.0f,1.0f });

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
	/// <param name="deadRange">デッドゾーンの範囲</param>
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
};