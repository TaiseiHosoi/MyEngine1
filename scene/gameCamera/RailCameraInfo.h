#pragma once
#include"vector"
#include"Vector3.h"
struct RailCameraInfo {
	int startIndex;
	int oldStartIndex;
	float timeRate;
	int nowCount;
	std::vector<Vector3>points;
};