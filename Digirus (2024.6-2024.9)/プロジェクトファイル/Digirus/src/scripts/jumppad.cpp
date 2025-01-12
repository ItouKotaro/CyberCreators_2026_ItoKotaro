//=============================================================
//
// �W�����v�p�b�h [jumppad.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "jumppad.h"
#include "component/3d/collision.h"

//=============================================================
// [CJumppad] �X�V
//=============================================================
void CJumppad::OnTriggerStay(GameObject* other)
{
	if (CCollision::GetCollision(other)->GetRigidBody() != nullptr &&
		(other->GetTag() == "Player" || other->GetTag() == "Enemy"))
	{ // �v���C���[�̂Ƃ�
		CCollision::GetCollision(other)->GetRigidBody()->applyCentralImpulse(btVector3(0.0f, m_fPower, 0.0f));
	}
}