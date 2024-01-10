#include "EnemyDirectionUtility.h"

void EnemyDirectionUtility::GetOffsetDirInfo(float& dirAngle, Vector3& nowOffset,const Vector3& originOffsetVec, const RailCameraInfo& railCameraInfo , float offsetAddTimeRate)
{
	//進行上の向いている方向(顔の向きではない)
	Vector3 directionLoot;
	directionLoot = MathFunc::TangentSplinePosition(railCameraInfo.points, railCameraInfo.startIndex, railCameraInfo.timeRate + offsetAddTimeRate)
		- MathFunc::TangentSplinePosition(railCameraInfo.points, railCameraInfo.startIndex, railCameraInfo.timeRate);
	directionLoot.nomalize();


	dirAngle = atan2f(directionLoot.x, directionLoot.z);


	nowOffset += originOffsetVec;
	nowOffset = MathFunc::RotateVecAngleY(nowOffset, dirAngle);
}
