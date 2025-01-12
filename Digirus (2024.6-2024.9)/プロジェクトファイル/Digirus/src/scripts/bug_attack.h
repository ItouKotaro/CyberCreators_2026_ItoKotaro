//=============================================================
//
// �o�O�A�^�b�N [bug_attack.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _BUG_ATTACK_H_
#define _BUG_ATTACK_H_

#include "component.h"

// �o�O�A�^�b�N
class CBugAttack : public Component
{
public:
	CBugAttack(const int& nLife = 360) { m_nLife = nLife; }
	void Init() override;
	void Update() override;
	void SetMove(const D3DXVECTOR3& move) { m_move = move; }

	static const float MOVE_SPEED;		// �ړ����x
	static const int ATTACK_DAMAGE;	// �U����
private:
	void UpdateJump();		// �W�����v

	D3DXVECTOR3 m_move;
	int m_nLife;
};

#endif // !_BUG_ATTACK_H_
