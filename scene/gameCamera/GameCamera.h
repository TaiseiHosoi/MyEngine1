#pragma once
#include"Camera.h"
#include"Input.h"
#include "Object3d.h"
#include<vector>

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
	void Initialize();
	void Update();
	void SetTargetPos(WorldTransform* targetPos);
	WorldTransform* GetTargetPos();
	void RemoveTargetPos();
	void SetEyePos(WorldTransform* eyePos);
	void SetFollowerPos(WorldTransform* wtf);
	WorldTransform* GetEyePos();
	void RemoveEyePos();
	void ViewPointMovement();
	void ResetGameCam();

	//プレイヤーを追従する
	void FollowPlayer();
	//シェイク用
	float randomFloat(float min , float max);	//ランダムを引き出す
	void ShakePrim();
	static void SetShakePrimST(float dura , float mag , bool isShakePrim);


	//カメラの向きを計算する関数
	void CulDirection();

	// 移動にかかる時間に基づいてt値を計算する関数
	float CalculateTValueBasedOnElapsedTime(float maxTime);

public:	// アクセッサ

	void SetShakeVec(Vector3 shakeVec);
	// シーン切り替え用
	void ChangeFollowFlag(bool flag);

	void SetIsPause(bool isPause)
	{
		isPause_ = isPause;
	}
	
	// jsonで配置したオブジェクトのポインタ
	void SetJsonObj(std::vector<Object3d>* objs) { jsonObjsPtr = objs; };

	// スプライン曲線用変数のゲッタ
	int GetStartIndex() { return static_cast<int>(startIndex_); };
	float GetTimeRate() { return timeRate_; };
	float GetOldTimeRate() { return timeRate_; };
	std::vector<Vector3> GetPoints() { return points; };

	int GetOldStartIndex() { return oldStartIndex_; };

	RailCameraInfo* GetRailCameraInfo();	//レールカメラの進行状況とレール情報を渡す


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
	static float magnitude;	//シェイクの強さ
	static float duration;//シェイクを起こす時間
	static bool isShake;
	float elapsedTime = 0.0f;
	const float deltaTime = 0.016f;	//1.0fを1秒(60FPS)と考えた際の1F
	Vector3 loolAtPos = {};
	Vector3 shakeVec_ = {0 , 0 , 0};

	//マウスカメラ用
	Vector3 rotation_ = {0 , 0 , 0};
	Vector3 dir_ = {0 , 0 , 1.f};
	Vector2 mouseMove;
	float mouseSensitivity_ = 0.05f;	//マウス感度
	bool isPause_;

	//レールカメラ用
	Vector3 basePos_ = {};
	Vector3 railTargetPos_ = {};
	
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
	

	float maxTime = 1.2f;				//全体時間[s]
	float timeRate_;			//何％時間が進んだか
	float oldTimeRate_;
	float targetTimeRate;	//ターゲット用
	int startIndex_ = 1;
	size_t targetStartIndex_ = 1;
	uint32_t startCount_ = 0;
	uint32_t targetStartCount_ = 0;
	uint32_t nowCount_ = 0;
	uint32_t elapsedCount_ = 0;
	Vector3 oldPos_ = {};
	int oldStartIndex_ = 0;

	std::unique_ptr<RailCameraInfo> railCameraInfo_;

};

