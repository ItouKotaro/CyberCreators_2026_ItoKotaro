//=============================================================
//
// ジャンプパッド [jumppad.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "jumppad.h"
#include "component/3d/collision.h"

//=============================================================
// [CJumppad] 更新
//=============================================================
void CJumppad::OnTriggerStay(GameObject* other)
{
	if (CCollision::GetCollision(other)->GetRigidBody() != nullptr &&
		(other->GetTag() == "Player" || other->GetTag() == "Enemy"))
	{ // プレイヤーのとき
		CCollision::GetCollision(other)->GetRigidBody()->applyCentralImpulse(btVector3(0.0f, m_fPower, 0.0f));
	}
}