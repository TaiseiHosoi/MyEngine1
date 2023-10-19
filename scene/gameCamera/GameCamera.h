/**
 * @file GameCamera.h
 * @brief ゲーム用カメラクラス
 */

#pragma once
#include"Camera.h"
#include"Input.h"
#include "Object3d.h"
#include<vector>

enum CAM_MODE {
	title,
	battle,
	startDirection
};

struct RailCameraInfo {
	int startIndex;
	int oldStartIndex;
	float timeRate;
	int nowCount;
	std::vector<Vector3>points;
};

class GameCamera :
	public Camera
{
public:
	//コンストラクタ
	GameCamera(int window_width , int window_height , Input* input);

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// ターゲットワールドトランスフォームセッタ
	void SetTargetPos(WorldTransform* targetPos);

	// ターゲットワールドトランスフォームゲッタ
	WorldTransform* GetTargetPos();

	// ターゲット位置の破棄
	void RemoveTargetPos();

	// eyePosセッタ
	void SetEyePos(WorldTransform* eyePos);

	// followerのワールドトランスフォームセット
	void SetFollowerPos(WorldTransform* wtf);

	//  eyePosのワールドトランスフォームゲッタ
	WorldTransform* GetEyePos();

	// eyePosの破棄
	void RemoveEyePos();

	// マウス操作関数
	void ViewPointMovement();

	// ゲームカメラリセット
	void ResetGameCam();

	// プレイヤーを追従する
	void FollowPlayer();
	// シェイク用
	float randomFloat(float min , float max);	//ランダムを引き出す

	// 画面シェイク関数
	void ShakePrim();

	/// <summary>
	/// 画面シェイク関数情報セット
	/// </summary>
	/// <param name="dura"></時間>
	/// <param name="mag"></大きさ>
	/// <param name="isShakePrim"></シェイクさせるか>
	static void SetShakePrimST(float dura , float mag , bool isShakePrim);

	//カメラの向きを計算する関数
	void CulDirection();

	// 移動にかかる時間に基づいてt値を計算する関数
	float CalculateTValueBasedOnElapsedTime(float maxTime);

public:	// アクセッサ

	// シェイクのベクトルセット
	void SetShakeVec(Vector3 shakeVec);

	// シーン切り替え用
	void ChangeFollowFlag(bool flag);

	// ポーズ中かのセット
	void SetIsPause(bool isPause)
	{
		isPause_ = isPause;
	}
	
	// jsonで配置したオブジェクトのポインタ
	void SetJsonObj(std::vector<Object3d>* objs) { jsonObjsPtr = objs; };

	// スプライン曲線用変数のゲッタ
	// startIndex_ゲッタ
	int GetStartIndex() { return static_cast<int>(startIndex_); };
	// timeRate_ゲッタ
	float GetTimeRate() { return timeRate_; };
	// oldTimeRate_ゲッタ
	float GetOldTimeRate() { return timeRate_; };
	// points_ゲッタ
	std::vector<Vector3> GetPoints() { return points; };

	//カメラモードのセット
	void SetCamMode(int num);

	// oldStartIndex_のゲッタ
	int GetOldStartIndex() { return oldStartIndex_; };

	// レールカメラ情報ゲッタ
	RailCameraInfo* GetRailCameraInfo();	//レールカメラの進行状況とレール情報を渡す

	// isCountIncゲッㇳ
	bool GetIsCountInc();

	// isCountIncセット
	void SetIsCountInc(bool setArg);


	// 1-> target ,0-> eye
	WorldTransform swap_[2];
private:
	Input* input_ = nullptr;
	WorldTransform* targetPos_ = nullptr;
	WorldTransform* eyePos_ = nullptr;
	WorldTransform* followerPos_ = nullptr;
	bool isFollowPlayer_ = true;
	bool isTargetenemy_ = false;

	//カメラ距離関係
	const Vector3 PRYMARY_CAMERA_POS = { 0.f,10.f,0.f };
	const float  MAX_CAMERA_DISTANCE = 50.0f;
	const float  MIN_CAMERA_DISTANCE = 7.0f;
	float cameraDistance_ = 20.0f;
	const float MAX_CHANGE_TIMER = 30;
	int cameraModeChangeCountTimer = 30;
	float cameraHeight_ = 6;
	
	//ShakePrim用変数
	static float magnitude_;	//シェイクの強さ
	static float duration_;//シェイクを起こす時間
	static bool isShake_;
	float elapsedTime_ = 0.0f;
	const float deltaTime_ = 0.016f;	//1.0fを1秒(60FPS)と考えた際の1F
	Vector3 loolAtPos_ = {};
	Vector3 shakeVec_ = {0 , 0 , 0};

	//マウスカメラ用
	Vector3 rotation_ = {0 , 0 , 0};
	Vector3 dir_ = {0 , 0 , 1.f};
	Vector2 mouseMove_;
	float mouseSensitivity_ = 0.05f;	//マウス感度
	bool isPause_;

	//レールカメラ用
	Vector3 basePos_ = {};
	Vector3 railTargetPos_ = {};
	//シーンタイトル用
	Vector3 directionLoot_ = {};
	Vector3 titleScOffsetPos_ = {10.f,0,20.f};
	const float gamepartCamPosY_ = 6.f;
	//シーンスタート演出用
	int startDirectionNowCount_ = 0;
	const int maxStartDirectionNowCount_ = 120;
	const float startDirectionSAFStrength_ = 6.f;
	float startDirectionFOV_ = 0.5f;
	const float offsetStartDirectionFOV_ = 0.5f;
	bool isCountInc_ = false;
	
	//制御店の集合(vectorコンテナ),補完する区間の添字、時間経過率
	Vector3 splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t);
	//補間で使うデータ
	//start → end を5秒で完了させる
	Vector3 p0 = { -100.0f, 0, -50.0f };			//スタート地点
	Vector3 p1 = { -50.0f, 50.0f, -50.0f };	//制御点その1
	Vector3 p2 = { 50.0f, -30.0f, -50.0f };	//制御点その2
	Vector3 p3 = { 100.0f, 0.0f, -50.0f };		//ゴール地点

	std::vector<Vector3>points{};
	std::vector<Object3d>* jsonObjsPtr = nullptr;
	
	//　レールカメラ変数	
	float maxTime_ = 1.2f;				//全体時間[s]
	float timeRate_;			//何％時間が進んだか
	float oldTimeRate_;
	float targetTimeRate_;	//ターゲット用
	int startIndex_ = 1;
	size_t targetStartIndex_ = 1;
	uint32_t startCount_ = 0;
	uint32_t targetStartCount_ = 0;
	uint32_t nowCount_ = 0;
	uint32_t elapsedCount_ = 0;
	Vector3 oldPos_ = {};
	int oldStartIndex_ = 0;

	std::unique_ptr<RailCameraInfo> railCameraInfo_;

	//camMode
	int camMode_ = 0;

};

