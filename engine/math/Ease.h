
/**
 * @file Ease.h
 * @brief イージング管理クラス
 */#pragma once
#include "math.h"
#include "Vector3.h"


class Ease {	//イージングクラス定義
public:
	// in
	static double InQuad(double change, double base, double duration, double time);
	// out
	static double OutQuad(double change, double base, double duration, double time);
	// inOut
	static double InOutQuad(double change, double base, double duration, double time) ;
	// 始点終点から求めるEaseIn
	static float LinearEasing(float start, float end, int count, int maxCount);
	// 始点終点から求めるEaseOut
	static float LinearEaseOutEasing(float start, float end, int count, int maxCount);
	//↓ここから下は使いません。。↓
	// 初期化
	void Initialize();
	// 更新
	void Update();

private:
	int maxTime_ = 50;	//最大時間
	int time_ = 0;//経過時間
	Vector3 startVec_ = {};
	Vector3 endVec_ = {};
	double start_ = 0;
	double end_ = 0;
	bool isChange_ = false;
};
