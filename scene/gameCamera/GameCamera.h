#pragma once
#include"Camera.h"
#include"Input.h"
#include "Object3d.h"
#include<vector>

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


	//シェイク用
	float randomFloat(float min , float max);	//ランダムを引き出す
	void ShakePrim();
	static void SetShakePrimST(float dura , float mag , bool isShakePrim);

	void SetShakeVec(Vector3 shakeVec);

	//カメラの向きを計算する関数
	void CulDirection();

	//プレイヤーを追従する
	void FollowPlayer();

	// シーン切り替え用
	void ChangeFollowFlag(bool flag);

	void SetIsPause(bool isPause)
	{
		isPause_ = isPause;
	}

	void SetJsonObj(std::vector<Object3d>* objs) { jsonObjsPtr = objs; };

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
	float timeRate;						//何％時間が進んだか
	size_t startIndex = 1;
	size_t targetStartIndex = 1;
	uint32_t startCount = 0;
	uint32_t targetStartCount = 0;
	uint32_t nowCount = 0;
	uint32_t elapsedCount = 0;

	Vector3 oldPos_ = {};

};

