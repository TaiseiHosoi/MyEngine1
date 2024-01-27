#pragma once
#include"MathFunc.h"
#include"Object3d.h"

struct DeathActionInfo {
	bool isDeathAction;	//デスアクションするかどか
	int deathActionCount;	//現在の死亡までのカウント
	int maxDeathActionCount;	//アクションが終わって死亡時フラグがオンになるまでのフレーム
	float offsetBoundSpeed;	//地面にバウンドしてからスピードに代入する値(高さ)
	float nowFallSpeed;	//可変する降下スピードの現在
	float fallSpeedVel;	//バウンドするときの降下スピード
	float deathActionRotateVel; //前転するスピード
	float subtractTimeRateVel;	//転がりながら画面後ろにフェードアウトしていくための
	float lowestPosY;	//地上で一番低い位置

};


namespace EnemyActionFunc {
	/// <summary>
	/// 死亡前のアクション
	/// </summary>
	/// <param name="dInfo"><死亡アクション用変数>
	/// <param name="isDead"><死亡フラグ>
	/// <param name="object3d"><オブジェクト3D>
	/// <param name="posY"><明示的に決めるpositionY>
	/// <param name="nowSubtractTimeRate"><敵の前進した場所のtimerate>
	void DeathAction(DeathActionInfo& dInfo,bool& isDead,Object3d& object3d,float& posY, float& nowSubtractTimeRate);
}
