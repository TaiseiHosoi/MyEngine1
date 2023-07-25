#include "GameCamera.h"
#include "Ease.h"
#include "JsonManager.h"

#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include"WinApp.h"
#include <WinUser.h>

float GameCamera::magnitude = 0.0f;
float GameCamera::duration = 0.0f;
bool GameCamera::isShake = 0.0f;

GameCamera::GameCamera(int window_width , int window_height , Input* input)
	: Camera(window_width , window_height)
{
	//入力インスタンス取得
	input_ = Input::GetInstance();

	//カメラの初期化
	Vector3 eye = {0.0f , 15.0f , -5.0f};
	Vector3 up = {0 , 1 , 0};
	Vector3 target = {0 , 0 , 300.0f};
	this->SetEye(eye);
	this->SetUp(up);
	this->SetTarget(target);

	isPause_ = false;


	
}

void GameCamera::Initialize()
{
	startCount = 0;
	targetStartCount = 1;
	nowCount = startCount;
	basePos_ = {};
	//新しいvectorセット
	for (int i = 0; i < jsonObjsPtr->size(); i++) {
		Vector3 newVec = jsonObjsPtr[0][i].worldTransform.translation_;
		points.push_back(newVec);
	}
	
}

void GameCamera::Update()
{
#pragma region マウス処理
	//マウス処理
	//if (GetActiveWindow() == WinApp::GetInstance()->GetHwnd())
	//{
	//	ShowCursor(false);
	//	//ViewPointMovement();
	//	CulDirection();
	//}
#pragma endregion マウス処理

#pragma region レールカメラ処理
	//経過時間(elapsedTime[s])の計算
	nowCount++;
	elapsedCount = nowCount - startCount;
	float elapsedTime = static_cast<float> (elapsedCount) / 60.f;

	//ターゲット用
	int targetCount = nowCount + 1;
	int targetElapsedCount = targetCount - targetStartCount;
	float targetElapsedTime = static_cast<float> (targetElapsedCount) / 60.f;
	float targetTimeRate = targetElapsedTime / maxTime;

	//スタート地点		: start
	//エンド地点		: end
	//経過時間		: elapsed[s]
	//移動完了の率	(経過時間/全体時間) : timeRate(％)

	timeRate = elapsedTime / maxTime;



	if (timeRate >= 1.0f) {
		if (startIndex < points.size() - 3) {

			startIndex += 1;
			timeRate -= 1.0f;
			startCount = nowCount;
		}
		else if (startIndex >= points.size() - 3 && startIndex <= points.size()) {
			startIndex += 1;
			timeRate -= 1.0f;
			startCount = nowCount;
			
		}
		else {
			startIndex = 1;
			timeRate = elapsedTime / maxTime;
			elapsedCount = 0;
			nowCount = 0;
			startCount = nowCount;
		}
		
	}
	Vector3 target = basePos_ - oldPos_;
	target.nomalize();


	
	oldPos_ = basePos_;
	basePos_ = splinePosition(points, startIndex, timeRate);
	railTargetPos_ = basePos_ + target;

	Vector3 e = GetEye();
	Vector3 t = GetTarget();
	FollowPlayer();
#pragma endregion レールカメラ処理

	
	

	ImGui::Begin("eye");
	ImGui::InputFloat3("nowpos", &e.x);
	ImGui::InputFloat3("nowtarget", &t.x);

	ImGui::End();


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
	const float cameraSpeed = 0.0005f;

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

void GameCamera::ShakePrim()
{
	if (isShake == true)
	{

		if (elapsedTime < duration)
		{
			float offsetX = randomFloat(-1.0f , 1.0f) * magnitude;
			float offsetY = randomFloat(-1.0f , 1.0f) * magnitude;
			float offsetZ = randomFloat(-1.0f , 1.0f) * magnitude;

			loolAtPos = {offsetX , offsetY , offsetZ};

			elapsedTime += deltaTime;
		}
		else
		{
			elapsedTime = 0.0f;

			isShake = false;
		}

	}

}

void GameCamera::SetShakePrimST(float dura , float mag , bool isShakePrim)
{
	duration = dura;
	magnitude = mag;
	isShake = isShakePrim;
}

void GameCamera::SetShakeVec(Vector3 shakeVec)
{
	shakeVec_ = shakeVec;
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



void GameCamera::FollowPlayer()
{
	

	
		//Vector3 basePos = {0 , 10.f , followerPos_->translation_.z};

		Vector3 tempEye = { basePos_.x,basePos_.y,basePos_.z };
		//tempEye.z -= dir_.z * MAX_CAMERA_DISTANCE;
		//railTargetPos_.z -= dir_.z * MAX_CAMERA_DISTANCE;

		SetEye(tempEye);
		SetTarget(railTargetPos_);

		//SetEye(Vector3(0,500,-1));
		//SetTarget(Vector3(0,0,0));
		
		//SetTarget({ tempEye.x,tempEye.y,tempEye.z + 1.f });

	
	
}

void GameCamera::ChangeFollowFlag(bool flag)
{
	isFollowPlayer_ = flag;
}

Vector3 GameCamera::splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t)
{
	//補間すべき点の数
	size_t n = points.size() - 2;

	if (startIndex > n)return points[n];
	if (startIndex < 1)return points[1];

	Vector3 p0 = points[startIndex - 1];
	Vector3 p1 = points[startIndex];
	Vector3 p2 = points[startIndex + 1];
	Vector3 p3 = points[startIndex + 2];

	Vector3 position = 0.5 * (2 * p1 + (-p0 + p2) * t + (2 * p0 - 5 * p1 + 4 * p2 - p3) * (t * t) + (-p0 + 3 * p1 - 3 * p2 + p3) * (t * t * t));

	return position;
}