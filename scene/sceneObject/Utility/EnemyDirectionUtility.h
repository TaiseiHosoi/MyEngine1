#pragma once
//敵をゲームの進行度合いに対して情報を更新したりする
#include"RailCameraInfo.h"
#include"MathFunc.h"



namespace EnemyDirectionUtility {
	
	
	/// <summary>
	/// フィールドの進行度に対して情報を更新した値を返す
	/// </summary>
	/// <param name="dirAngle"><x,z方向ベクトルからrotate.yに対応する値>
	/// <param name="nowOffset"><フィールドの進行情報を反映させる値>
	/// <param name="originOffsetVec"><進行度情報を反映させる前の元になるVector3pos>
	/// <param name="railCameraInfo"><進行度情報>
	/// <param name="offsetAddTimeRate"><方向ベクトルを求めるために進行した場所>
	void GetOffsetDirInfo(float& dirAngle, Vector3& nowOffset, const Vector3& originOffsetVec, const RailCameraInfo& railCameraInfo, float offsetAddTimeRate);
	
}
