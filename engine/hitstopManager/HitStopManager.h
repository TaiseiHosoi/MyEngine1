/**
 * @file HitStop.h
 * @brief ヒットストップの管理クラス
 */
#pragma once
#include "GameCamera.h"
#include "Vector3.h"
#include <random>

class HitStopManager
{
public:

	// インスタンス取得
	static HitStopManager* GetInstance();

	// 更新
	void Update();
	
	/// <summary>
	/// ヒットストップ情報セット
	/// </summary>
	/// <param name="hitStopFlag"></動作フラグ>
	/// <param name="timer"></持続時間>
	void SetHitStop(bool* hitStopFlag , int timer);

	// カメラセット
	void SetGameCamera(GameCamera* gameCamera);

	// カメラゲッタ
	GameCamera* GetGameCamera();

private:
	// シェイクのアングル計算
	void CulShakeAngle();
	
	// 動作処理
	void Execute();
	
	// カメラのシェイク
	void CameraShake();
	
	// 終了時処理
	void Finalize();

private:
	HitStopManager() = default;
	HitStopManager(const HitStopManager&) = delete;
	~HitStopManager() = default;
	HitStopManager& operator= (const HitStopManager&) = delete;

private:
	bool* hitStopFlag_ = nullptr;
	int timer_;
	Vector3 shakeAngle_;
	GameCamera* gameCamera_;
};

