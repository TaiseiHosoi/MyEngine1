#include "Ease.h"

double Ease::InQuad(double change, double base, double duration, double time) {	//イージングイン
	time /= duration;
	return change * time * time + base;	//changeは移動量。baseは最初の位置。durationは移動時間で、timeが現在の経過時間
}

double Ease::OutQuad(double change, double base, double duration, double time) {
	time /= duration;
	return -change * time * (time - 2) + base;
}

double Ease::InOutQuad(double change, double base, double duration, double time)
{	//イージングインアウト
	time /= duration / 2;
	if (time < 1) return change / 2 * time * time + base;
	return -change / 2 * ((--time) * (time - 2) - 1) + base;
}

float Ease::LinearEasing(float start, float end, int count, int maxCount)
{
    if (count <= 0) {
        return start;
    }
    else if (count >= maxCount) {
        return end;
    }
    else {
        float t = static_cast<float>(count) / maxCount;
        return start + t * (end - start);
    }
}

float Ease::LinearEaseOutEasing(float start, float end, int count, int maxCount)
{
    if (count <= 0) {
        return start;
    }
    else if (count >= maxCount) {
        return end;
    }
    else {
        float t = static_cast<float>(count) / maxCount;
        t = 1 - (1 - t) * (1 - t); // イージング関数を適用
        return start + t * (end - start);
    }
}

void Ease::Initialize()
{
}

void Ease::Update()
{
}


