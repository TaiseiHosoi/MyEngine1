#include "EnemyActionUtility.h"

void EnemyActionFunc::DeathAction(DeathActionInfo& dInfo,bool& isDead, Object3d& object3d, float& posY, float& nowSubtractTimeRate)
{
	if (dInfo.isDeathAction == true) {
		dInfo.deathActionCount++;
		if (dInfo.deathActionCount >= dInfo.maxDeathActionCount) {
			isDead = true;
		}


		if (object3d.worldTransform.translation_.y < dInfo.lowestPosY) {
			dInfo.nowFallSpeed = dInfo.offsetBoundSpeed;
			posY = 0;
		}
		else {
			dInfo.nowFallSpeed -= dInfo.fallSpeedVel;
		}
		posY += dInfo.nowFallSpeed;	//y変動
		object3d.worldTransform.rotation_.x += dInfo.deathActionRotateVel;
		nowSubtractTimeRate += dInfo.subtractTimeRateVel;	//タイムレートが落ちてくる

	}

}
