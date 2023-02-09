#include "CollisionPrimitive.h"

using namespace DirectX;

bool CollisionPrimitive::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter)
{
	//���W�n�̌��_���狉�̒��S���Ђ傤�ւ̋���
	float distV = sphere.center.dot(plane.normal);

	//���ʂ������_���������Z���邱�ƂŁA���ʂƋ��̒��S�Ƃ̋������o��
	float dist = distV - plane.distance;

	//�����̐�Βl�����a���傫����Γ������Ă��Ȃ�
	if (fabs(dist) > sphere.radius) return false;

	//�^����_
	if (inter) {
		//���ʏ�̍ŋߐړ_���^����_�Ƃ���
		*inter = -dist * plane.normal + sphere.center;
	}

	return true;
}
