#include "CollisionManager.h"
#include "BaseCollider.h"
#include "MeshCollider.h"
#include "Collision.h"
#include "Vector3.h"
#include "MathFunc.h"

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

void CollisionManager::CheckAllCollisions()
{
	std::forward_list<BaseCollider*>::iterator itA;
	std::forward_list<BaseCollider*>::iterator itB;

	itA = colliders.begin();
	for (; itA != colliders.end(); ++itA)
	{

		itB = itA;
		++itB;

		for (; itB != colliders.end(); ++itB)
		{
			BaseCollider* colA = *itA;
			BaseCollider* colB = *itB;

			if (colA->attribute == colB->attribute)
			{
				continue;
			}

			if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
				colB->GetShapeType() == COLLISIONSHAPE_SPHERE)
			{

				Sphere* sphereA = dynamic_cast<Sphere*>(colA);
				Sphere* sphereB = dynamic_cast<Sphere*>(colB);
				Vector3 inter;

				if (Collision::CheckSphere2Sphere(*sphereA , *sphereB , &inter))
				{
					colA->OnCllision(CollisionInfo(colB->GetObject3d() , colB , inter));
					colB->OnCllision(CollisionInfo(colA->GetObject3d() , colA , inter));
				}

			}
			else if (colA->GetShapeType() == COLLISIONSHAPE_MESH &&
					 colB->GetShapeType() == COLLISIONSHAPE_SPHERE)
			{
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
				Sphere* sphere = dynamic_cast<Sphere*>(colB);
				Vector3 inter;

				if (meshCollider->CheckCollisionSphere(*sphere , &inter))
				{
					colA->OnCllision(CollisionInfo(colB->GetObject3d() , colB , inter));
					colB->OnCllision(CollisionInfo(colA->GetObject3d() , colA , inter));
				}

			}

			else if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
					 colB->GetShapeType() == COLLISIONSHAPE_MESH)
			{
				Sphere* sphere = dynamic_cast<Sphere*>(colA);
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
				Vector3 inter;

				if (meshCollider->CheckCollisionSphere(*sphere , &inter))
				{
					colA->OnCllision(CollisionInfo(colB->GetObject3d() , colB , inter));
					colB->OnCllision(CollisionInfo(colA->GetObject3d() , colA , inter));
				}

			}


		}
	}
}

bool CollisionManager::Raycast(const Ray& ray , unsigned short attribute , RaycastHit* hitInfo , float maxDistance)
{
	bool result = false;
	//走査用のイテレータ
	std::forward_list<BaseCollider*>::iterator it;
	//今までで最も近いコライダーを記録するためのイテレータ
	std::forward_list<BaseCollider*>::iterator it_hit;
	//今までで最も近いコライダーの距離を記録する変数
	float distance = maxDistance;
	//今までで最も近いコライダーとの交点を記録する変数
	Vector3 inter;

	it = colliders.begin();
	for (; it != colliders.end(); ++it)
	{
		BaseCollider* colA = *it;
		//球の場合
		if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE)
		{
			Sphere* sphere = dynamic_cast<Sphere*>(colA);
			float tempDistance;
			Vector3 tempInter;
			//当たらなければ除外
			if (!Collision::CheckRay2Sphere(ray , *sphere , &tempDistance , &tempInter))
			{
				continue;
			}
			//距離が最小でなければ除外
			if (tempDistance >= distance)
			{
				continue;
			}
			if (!(colA->attribute & attribute))
			{
				continue;
			}
			//今までで最も近いので記録をとる
			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
		else if (colA->GetShapeType() == COLLISIONSHAPE_MESH)
		{
			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);

			float tempDistance;
			Vector3 tempInter;
			if (!meshCollider->CheckCollisionRay(ray , &tempDistance , &tempInter))
			{
				continue;
			}
			if (tempDistance >= distance)
			{
				continue;
			}
			if (!(colA->attribute & attribute))
			{
				continue;
			}
			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
	}
	if (result && hitInfo)
	{
		hitInfo->distance = distance;
		hitInfo->inter = inter;
		hitInfo->collider = *it_hit;
		hitInfo->object = hitInfo->collider->GetObject3d();
	}
	return result;
}

bool CollisionManager::Raycast(const Ray& ray , RaycastHit* hitInfo , float maxDistance)
{
	return Raycast(ray , 0xffff , hitInfo , maxDistance);
}