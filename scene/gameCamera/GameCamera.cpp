#include "GameCamera.h"
#include "Ease.h"
#include "GameObjManager.h"
#include "SplineCurve.h"

#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include"WinApp.h"
#include <WinUser.h>

GameCamera::GameCamera(int window_width , int window_height , Input* input)
	: Camera(window_width , window_height)
{
	//入力インスタンス取得
	input_ = Input::GetInstance();

	//カメラの初期化
	Vector3 eyeVal = {0.0f , 10.0f , -5.0f};
	Vector3 upVal = {0 , 1 , 0};
	Vector3 targetVal = {0 , 0 , 300.0f};
	this->SetEye(eye);
	this->SetUp(up);
	this->SetTarget(target);

	isPause_ = false;

	input_ = input;
	
}

void GameCamera::GameInfoInitialize()
{
	startCount_ = 0;
	targetStartCount_ = 1;
	nowCount_ = startCount_;
	basePos_ = {};
	

	//新しいvectorセット
	for (int i = 0; i < jsonObjsPtr->size(); i++) {
		Vector3 newVec = jsonObjsPtr[0][i].worldTransform.translation_;
		points.push_back(newVec);
	}
	oldStartIndex_ = 0;

	railCameraInfo_ = std::make_unique<RailCameraInfo>();
	railCameraInfo_->startIndex = startIndex_;
	railCameraInfo_->oldStartIndex = oldStartIndex_;
	railCameraInfo_->timeRate = timeRate_;
	railCameraInfo_->points = points;
	
}

void GameCamera::Update()
{
#pragma region マウス処理
	//マウス処理
	if (GetActiveWindow() == WinApp::GetInstance()->GetHwnd())
	{
		ShowCursor(false);
		//ViewPointMovement();
		//CulDirection();
	}
#pragma endregion マウス処理

#pragma region レールカメラ処理

	oldStartIndex_ = startIndex_;	//前フレーム処理
	oldTimeRate_ = timeRate_;
	oldPos_ = basePos_;	//前フレームpos保存

	// カメラの位置を更新
	timeRate_ = CalculateTValueBasedOnElapsedTime(maxTimeVal); // maxTimeに基づいてt値を計算

	targetTimeRate = timeRate_ + targetTimeRateAdvancedVal_;	//ターゲット位置は少し進んだ場所
	if (targetTimeRate >= maxTimeRate_) {
		targetTimeRate -= maxTimeRate_;	//もし1を超えてたら-1
	}
	
	//if (input_->TriggerKey(DIK_N)) {
	//	gameOverDirectionNowCount_ = 0;
	//	camMode_ = CAM_MODE::gameOver;
	//}

	//進行上の向いている方向
	directionLoot_ = MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate + 0.005f)
		- MathFunc::TangentSplinePosition(railCameraInfo_->points, railCameraInfo_->startIndex, railCameraInfo_->timeRate);
	directionLoot_.nomalize();

	Vector3 nowOffset;
	float dirAngle = atan2(directionLoot_.x, directionLoot_.z);

	//プレイヤーの動きに合わせて動くカメラ挙動分のVec3
	Vector3 parallelVal = { 0,0,0 };
	if (playerParallelMoveVal_ != nullptr) {
		parallelVal.x = *playerParallelMoveVal_ / 2.f;
	}

	//進行状況に合わせて回転させる現在のVector3
	Vector3 nowParalellVec = MathFunc::RotateVecAngleY(parallelVal, dirAngle);
	Vector3 nowTargetPosVelueToAdd = MathFunc::RotateVecAngleY(*targetPosVelueToAdd_, dirAngle);


	if (camMode_ == CAM_MODE::title) {

		nowOffset = MathFunc::RotateVecAngleY(titleScOffsetPos_, dirAngle);

		

		//ワールド上の自機の回転量yを求める
		

		basePos_ = MathFunc::TangentSplinePosition(points, startIndex_, timeRate_);	//カメラの位置
		
		target = MathFunc::TangentSplinePosition(points, startIndex_, targetTimeRate);
		railTargetPos_ = target;	//ローカル変数に保存
		
		Vector3 minusVec =  nowOffset;
		minusVec.nomalize();
		minusVec *= titleMinusVecLen_;	

		basePos_ += minusVec;
		basePos_.y = gamepartCamPosY;

		Vector3 e = GetEye();
		Vector3 targ = GetTarget();
		FollowPlayer();


		
	}
	else if (camMode_ == CAM_MODE::battle) {



		//値を入力
		basePos_ = MathFunc::TangentSplinePosition(points, startIndex_, timeRate_);	//カメラの位置
		target = MathFunc::TangentSplinePosition(points, startIndex_, targetTimeRate);
		railTargetPos_ = target + nowParalellVec* targetParalellVecMag_ + nowTargetPosVelueToAdd;	//ローカル変数に保存


		Vector3 minusVec = railTargetPos_ - basePos_;
		minusVec.nomalize();
		float minusVal = battleSCMinusVal_;
		minusVec *= minusVal;	//引きカメラ

		basePos_ += minusVec + nowParalellVec;
		basePos_.y = gamepartCamPosY;

		FollowPlayer();
	}
	else if (camMode_ == CAM_MODE::startDirection) {
		if (isCountInc_ == true) {
			startDirectionNowCount_++;
		}

		// カメラpos処理
		basePos_ = MathFunc::TangentSplinePosition(points, startIndex_, timeRate_);	//カメラの位置

		target = MathFunc::TangentSplinePosition(points, startIndex_, targetTimeRate);
		railTargetPos_ = target + nowParalellVec * targetParalellVecMag_;	//ローカル変数に保存


		Vector3 minusVec = railTargetPos_ - basePos_;
		minusVec.nomalize();
		float minusVal = battleSCMinusVal_;
		Vector3 gamepartCamPos = basePos_ + minusVec * minusVal;	//引きカメラ
		gamepartCamPos.y = gamepartCamPosY;
		

		
		Vector3 directionPos = basePos_ - minusVec * directionMagnification;
		directionPos.y += directionShiftY;

		//basePos_ = directionPos;

		basePos_ = { Ease::LinearEaseOutEasing(directionPos.x,gamepartCamPos.x,startDirectionNowCount_,maxStartDirectionNowCount_,startDirectionSAFStrength_),
			Ease::LinearEaseOutEasing(directionPos.y,gamepartCamPos.y,startDirectionNowCount_,maxStartDirectionNowCount_,startDirectionSAFStrength_),
			Ease::LinearEaseOutEasing(directionPos.z,gamepartCamPos.z,startDirectionNowCount_,maxStartDirectionNowCount_,startDirectionSAFStrength_)
		};
		basePos_ += nowParalellVec;	//プレイヤー横移動プラス
		FollowPlayer();

		// fovY演出処理
		startDirectionFOV_ = Ease::LinearEaseOutEasing(offsetStartDirectionFOV_, GetOffsetFovAngleY(), startDirectionNowCount_, maxStartDirectionNowCount_, startDirectionSAFStrength_);
		SetFovAngleY(startDirectionFOV_);

		// 演出終了時処理
		if (startDirectionNowCount_ >= maxStartDirectionNowCount_) {
			camMode_ = CAM_MODE::battle;
			startDirectionNowCount_ = 0;
			SetIsCountInc(false);
			SetFovAngleY(GetOffsetFovAngleY());
		}

	}
	else if (camMode_ == CAM_MODE::gameOver) {
		
		if (gameOverDirectionNowCount_ < maxGameOverDirectionCount_) {
			gameOverDirectionNowCount_++;
		}
		else {
			
		}

		//値を入力
		basePos_ = MathFunc::TangentSplinePosition(points, startIndex_, timeRate_);	//カメラの位置

		target = MathFunc::TangentSplinePosition(points, startIndex_, targetTimeRate);
		railTargetPos_ = target + nowParalellVec;	//ローカル変数に保存


		Vector3 minusVec = railTargetPos_ - basePos_;
		minusVec.nomalize();
		float minusVal = battleSCMinusVal_;
		minusVec *= minusVal;	//引きカメラ
		
		//進行方向vec
		Vector3 plusVec = basePos_ - railTargetPos_;
		plusVec.nomalize();
		plusVec *= adjustGameOverDirectionLen_;

		basePos_ += minusVec + nowParalellVec;
		basePos_.y = gamepartCamPosY;

		Vector3 tempEye = { Ease::LinierEaseInOutEasing(basePos_.x,basePos_.x - plusVec.x,gameOverDirectionNowCount_,maxGameOverDirectionCount_,startDirectionSAFStrength_)
			,Ease::LinierEaseInOutEasing(basePos_.y,basePos_.y - plusVec.y,gameOverDirectionNowCount_,maxGameOverDirectionCount_,startDirectionSAFStrength_)
			,Ease::LinierEaseInOutEasing(basePos_.z,basePos_.z - plusVec.z,gameOverDirectionNowCount_,maxGameOverDirectionCount_,startDirectionSAFStrength_) };

		SetEye(tempEye);
		SetTarget(railTargetPos_);


	}
	else if (camMode_ == CAM_MODE::gameClear) {

		if (gameClearDirectionNowCount_ <= stopCamDirectionCount_) {
			if (gameClearDirectionNowCount_ == stopCamDirectionCount_){
				stopTimeRate_ = timeRate_ + 0.005f;
				if (stopTimeRate_ > 1.0f) {
					stopTimeRate_ = DecrimentTimeRate(stopTimeRate_);
				}
			}

			gameClearDirectionNowCount_++;
			//値を入力
			basePos_ = MathFunc::TangentSplinePosition(points, startIndex_, timeRate_);	//カメラの位置

			//値初期化
			isGameClearDirectionEnd_ = false;
 
		}
		else if(gameClearDirectionNowCount_ < maxGameClearDirectionCount_){

			gameClearDirectionNowCount_++;
			//値を入力
			basePos_ = MathFunc::TangentSplinePosition(points, startIndex_, timeRate_);	//カメラの位置
		}
		else {
			isGameClearDirectionEnd_ = true;
			basePos_ = MathFunc::TangentSplinePosition(points, startIndex_, stopTimeRate_);	//カメラの位置
		}



		target = MathFunc::TangentSplinePosition(points, startIndex_, targetTimeRate);
		target.y += adjustCamDirPos_.y;	//注視点微調整
		railTargetPos_ = target + nowParalellVec;	//ローカル変数に保存


		Vector3 minusVec = railTargetPos_ - basePos_;
		minusVec.nomalize();
		float minusVal = battleSCMinusVal_;
		minusVec *= minusVal;	//引きカメラ

		//進行方向vec
		Vector3 plusVec = basePos_ - railTargetPos_;
		plusVec.nomalize();
		plusVec *= adjustGameOverDirectionLen_;

		basePos_ += minusVec + nowParalellVec;
		basePos_.y = gamepartCamPosY;

		Vector3 tempEye = { Ease::LinierEaseInOutEasing(basePos_.x,basePos_.x - plusVec.x,gameClearDirectionNowCount_,maxGameOverDirectionCount_,startDirectionSAFStrength_)
			,Ease::LinierEaseInOutEasing(basePos_.y,basePos_.y - plusVec.y,gameClearDirectionNowCount_,maxGameClearDirectionCount_,startDirectionSAFStrength_)
			,Ease::LinierEaseInOutEasing(basePos_.z,basePos_.z - plusVec.z,gameClearDirectionNowCount_,maxGameClearDirectionCount_,startDirectionSAFStrength_) };

		tempEye += adjustCamDirPos_;

		SetEye(tempEye);
		SetTarget(railTargetPos_);


	}

	//infoの情報更新
	railCameraInfo_->startIndex = startIndex_;
	railCameraInfo_->oldStartIndex = oldStartIndex_;
	railCameraInfo_->timeRate = timeRate_;
	railCameraInfo_->nowCount = nowCount_;
	railCameraInfo_->points = points;
	

	Camera::Update();
	
}

void GameCamera::SetTargetPos(WorldTransform* targetPos)
{
	targetPos_ = targetPos;
}

WorldTransform* GameCamera::GetTargetPos()
{
	return targetPos_;
}

void GameCamera::RemoveTargetPos()
{
	targetPos_ = nullptr;
}

void GameCamera::SetEyePos(WorldTransform* eyePos)
{
	eyePos_ = eyePos;
}

void GameCamera::SetFollowerPos(WorldTransform* wtf)
{
	followerPos_ = wtf;
}

WorldTransform* GameCamera::GetEyePos()
{
	return eyePos_;
}

void GameCamera::RemoveEyePos()
{
	eyePos_ = nullptr;
}

void GameCamera::ViewPointMovement()
{
	//カメラの回転ベクトル
	Vector3 rotat = {0 , 0 , 0};
	//カメラの移動の速さ
	//const float cameraSpeed = 0.0005f;

	Vector2 windowWH = Vector2(WinApp::window_width / 2 , WinApp::window_height / 2);
	POINT mousePosition;
	//マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd , &mousePosition);

	int xPos_absolute , yPos_absolute;

	int xPos = static_cast<int>(windowWH.x);  //移動させたいｘ座標（ウィンドウ内の相対座標）
	int yPos = static_cast<int>(windowWH.y); //移動させたいｙ座標（ウィンドウ内の相対座標）

	WINDOWINFO windowInfo;
	//ウィンドウの位置を取得
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd , &windowInfo);

	//マウスの移動先の絶対座標（モニター左上からの座標）
	xPos_absolute = xPos + windowInfo.rcWindow.left + 8;//なんかずれてるから直す
	yPos_absolute = yPos + windowInfo.rcWindow.top + 31; //ウィンドウのタイトルバーの分（31px）をプラス

	SetCursorPos(xPos_absolute , yPos_absolute);//移動させる

	//マウスの移動量を取得
	mouseMove = Vector2(0 , 0);
	mouseMove = (Vector2(static_cast<float>(mousePosition.y) , static_cast<float>(mousePosition.x)) - Vector2(windowWH.y , windowWH.x));//座標軸で回転している関係でこうなる(XとYが入れ替え)

	if (isPause_ == false) {

		//マウスの移動量をカメラの回転を反映
		rotation_.y -= MathFunc::Dig2Rad(mouseMove.y * mouseSensitivity_);
		if (2 * MathFunc::PI <= rotation_.y)
		{
			rotation_.y -= 2 * MathFunc::PI;
		}
		else if (rotation_.y < 0)
		{
			rotation_.y += 2 * MathFunc::PI;
		}

		rotation_.x -= MathFunc::Dig2Rad(mouseMove.x * mouseSensitivity_);
		if (MathFunc::Dig2Rad(60) <= rotation_.x)
		{
			rotation_.x = MathFunc::Dig2Rad(60);
		}
		else if (rotation_.x < -MathFunc::Dig2Rad(15))
		{
			rotation_.x = -MathFunc::Dig2Rad(15);
		}
	}

}



float GameCamera::randomFloat(float min , float max)
{
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}




void GameCamera::ResetGameCam()
{
	nowCount_ = 0;
}

void GameCamera::CulDirection()
{
	//LShiftを押すと敵をターゲットする
	if (input_->GetInstance()->TriggerKey(DIK_LSHIFT))
	{

		dir_ = targetPos_->translation_ - followerPos_->translation_;
		dir_.nomalize();

		

		Vector2 vec1 = {dir_.x , dir_.z};
		Vector2 vec2 = {0 , 1};

		vec1.normalize();

		float dot = vec1.dot(vec2);

		rotation_.y = acosf(dot);

		//rotation_.y += MathFunc::Dig2Rad(90);

		if (0 < dir_.x)
		{
			rotation_.y = -rotation_.y;
			rotation_.y += MathFunc::Dig2Rad(180);
		}
	}

	//カメラの回転量から向きを計算
	dir_ = {cos(rotation_.y) , sin(rotation_.x) , sin(rotation_.y)};
	dir_.nomalize();

}

float GameCamera::CalculateTValueBasedOnElapsedTime(float maxTimeArg)
{
	// 経過時間(elapsedTime[s])の計算
	nowCount_++;
	elapsedCount_ = nowCount_ - startCount_;
	float elapsedTimeVal = static_cast<float>(elapsedCount_) / 60.f;

	// t値を計算
	float t = elapsedTimeVal / maxTimeArg;

	// tが1.0を超える場合の処理
	if (t >= 1.0f)
	{
		t -= 1.0f;
		nowCount_ = 0;
	}

	return t;
}

float GameCamera::DecrimentTimeRate(float arg)
{

	float answer = arg;
	answer -= 1.0f;

	return answer;
}



void GameCamera::FollowPlayer()
{

		Vector3 tempEye = { basePos_.x,basePos_.y,basePos_.z };


		SetEye(tempEye);
		SetTarget(railTargetPos_);
}

void GameCamera::ChangeFollowFlag(bool flag)
{
	isFollowPlayer_ = flag;
}

void GameCamera::SetCamMode(int num)
{
	camMode_ = num;
}

RailCameraInfo* GameCamera::GetRailCameraInfo()
{
	return railCameraInfo_.get();
}


bool GameCamera::GetIsCountInc()
{
	return isCountInc_;
}

void GameCamera::SetIsCountInc(bool setArg)
{
	isCountInc_ = setArg;
}

bool GameCamera::GetIsGameClearDirectionEnd()
{
	return isGameClearDirectionEnd_;
}

void GameCamera::SetIsGameClearDirectionEnd(bool arg)
{
	isGameClearDirectionEnd_ = arg;
}

void GameCamera::GoGameOver()
{
	gameOverDirectionNowCount_ = 0;
	camMode_ = CAM_MODE::gameOver;
}

void GameCamera::GoGameClear()
{
	gameClearDirectionNowCount_ = 0;
	camMode_ = CAM_MODE::gameClear;
}

void GameCamera::SetPlayerParallelMoveVal_(float* val)
{
	playerParallelMoveVal_ = val;
}

void GameCamera::SetTargetPosVelueToAdd(Vector3* vec)
{
	targetPosVelueToAdd_ = vec;
}

Vector3* GameCamera::GetRailTargetPosPtr()
{
	return &railTargetPos_;
}



Vector3 GameCamera::splinePosition(const std::vector<Vector3>& pointsArg, size_t startIndex, float t)
{
	//補間すべき点の数
	size_t n = pointsArg.size() - 2;

	if (startIndex > n)return points[n];
	if (startIndex < 1)return points[1];

	Vector3 p0Val = pointsArg[startIndex - 1];
	Vector3 p1Val = pointsArg[startIndex];
	Vector3 p2Val = pointsArg[startIndex + 1];
	Vector3 p3Val = pointsArg[startIndex + 2];

	Vector3 position = 0.5 * (2 * p1Val + (-p0Val + p2Val) * t + (2 * p0Val - 5 * p1Val + 4 * p2Val - p3Val) * (t * t) + (-p0Val + 3 * p1Val - 3 * p2Val + p3Val) * (t * t * t));

	return position;
}
