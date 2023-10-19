#include "SphereCollider.h"

void SphereCollider::Update()
{
	if (object3d_)
	{
		//ワールド行列から座標を抽出
		const Matrix4& matWorld = object3d_->GetMatWorld();

		//球のメンバ変数を更新
		Sphere::center =
		{
			matWorld.m[3][0] + offset_.x ,
			matWorld.m[3][1] + offset_.y ,
			matWorld.m[3][2] + offset_.z
		};
	}
	else if (basisPos_)
	{
		Sphere::center = *basisPos_ + offset_;
	}
	else
	{
		Sphere::center = {0 , 0 , 0};
	}


	Sphere::radius = radius_;

	isHit_ = false;
}