#include "Field.h"
#include <cassert>


void Field::Initialize(Mesh* model, Lane lane)
{

	assert(model);

	model_ = model;
	worldTransform_.SetModel(model);

	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();

	//テクスチャ読み込み
	//textureHandle_ = textureHandle;

	worldTransform_.Initialize(true);

	worldTransform_.scale = { 0.3f,0.2f,length };

	//x座標に応じて現在のレーンを判定
	lane_ = lane;
	
	if (lane == Left)
	{
		worldTransform_.position = { -laneWidth,0.0f,zLen_ };
	}
	else if (lane == Center)
	{
		worldTransform_.position = { 0.0f,0.0f,zLen_ };
	}
	else if (lane == Right)
	{
		worldTransform_.position = { laneWidth,0.0f,zLen_ };
	}
	
	worldTransform_.Update();

	//イージング
	ease_ = new Ease;
	
}

void Field::Draw()
{
	worldTransform_.Draw();
}

void Field::Update()
{

	/*if (input_->PushKey(DIK_LEFT)) {
		changeMode_ = 1;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		changeMode_ = 2;
	}*/

	//キー入力に応じてLaneを変更
	//if (changeMode_ == 1 && input_->TriggerKey(DIK_SPACE)
	//	&& isChangeLeft_ == false&& isChangeRight_ == false)
	//{
	//	/*if (lane_ == Left)lane_ = Center;
	//	else if (lane_ == Center)lane_ = Left;*/
	//	isChangeLeft_ = true;

	//}else if (changeMode_ ==2 && input_->TriggerKey(DIK_SPACE)
	//	&& isChangeLeft_ == false && isChangeRight_ == false)
	//{
	//	/*if (lane_ == Right)lane_ = Center;
	//	else if (lane_ == Center)lane_ = Right;*/
	//	isChangeRight_ = true;
	//}

	if (time_ >= maxTime_) {	//タイムリセット
		time_ = 0;
		if (isChangeLeft_ == true) {
			isChangeLeft_ = false;
			if (lane_ == Left) {
				lane_ = Center;
			}
			else if (lane_ == Center) {
				lane_ = Left;
			}
		}
		if (isChangeRight_ == true) {
			isChangeRight_ = false;
			if (lane_ == Right) {
				lane_ = Center;
			}
			else if (lane_ == Center) {
				lane_ = Right;
			}
		}

		if (lane_ == Left)
		{
			worldTransform_.position = { -laneWidth,0.0f,zLen_ };
		}
		else if (lane_ == Center)
		{
			worldTransform_.position = { 0.0f,0.0f,zLen_ };
		}
		else if (lane_ == Right)
		{
			worldTransform_.position = { laneWidth,0.0f,zLen_ };
		}
	}

	if (isChangeLeft_ == false && isChangeRight_ == false) {

		if (lane_ == Left && input_->TriggerKey(DIK_RIGHT)) {
			isChangeLeft_ = true;
		}
		else if (lane_ == Right && input_->TriggerKey(DIK_LEFT)) {
			isChangeRight_ = true;
		}
		else if (lane_ == Center && input_->TriggerKey(DIK_RIGHT)) {
			isChangeRight_ = true;
		}
		else if (lane_ == Center && input_->TriggerKey(DIK_LEFT)) {
			isChangeLeft_ = true;
		}
	}

	if (time_ >= maxTime_) {	//タイムリセット
		time_ = 0;
		if (isChangeLeft_ == true) {
			isChangeLeft_ = false;
			if (lane_ == Left) {
				lane_ = Center;
			}
			else if (lane_ == Center) {
				lane_ = Left;
			}
		}
		if (isChangeRight_ == true) {
			isChangeRight_ = false;
			if (lane_ == Right) {
				lane_ = Center;
			}
			else if (lane_ == Center) {
				lane_ = Right;
			}
		}

		if (lane_ == Left)
		{
			worldTransform_.position = { -laneWidth,0.0f,zLen_ };
		}
		else if (lane_ == Center)
		{
			worldTransform_.position = { 0.0f,0.0f,zLen_ };
		}
		else if (lane_ == Right)
		{
			worldTransform_.position = { laneWidth,0.0f,zLen_ };
		}
	}


	if (isChangeLeft_ == true) {
		time_++;
		if (lane_ == Left) {
			worldTransform_.position.x = ease_->InOutQuad(laneWidth, -laneWidth,maxTime_,time_);
		}
		if (lane_ == Center) {
			worldTransform_.position.x = ease_->InOutQuad(-laneWidth, 0.0f, maxTime_, time_);
		}
		
	}
	else if (isChangeRight_ == true) {
		time_++;
		if (lane_ == Right) {
			worldTransform_.position.x = ease_->InOutQuad(-laneWidth, laneWidth, maxTime_, time_);
		}
		if (lane_ == Center) {
			worldTransform_.position.x = ease_->InOutQuad(laneWidth, 0.0f, maxTime_, time_);
		}
	}
	
	
	worldTransform_.Update();
	
}

void Field::LaneChange()
{

}
