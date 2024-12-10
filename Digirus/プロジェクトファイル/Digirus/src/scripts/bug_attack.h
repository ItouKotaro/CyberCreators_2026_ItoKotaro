//=============================================================
//
// バグアタック [bug_attack.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _BUG_ATTACK_H_
#define _BUG_ATTACK_H_

#include "component.h"

// バグアタック
class CBugAttack : public Component
{
public:
	CBugAttack(const int& nLife = 360) { m_nLife = nLife; }
	void Init() override;
	void Update() override;
	void SetMove(const D3DXVECTOR3& move) { m_move = move; }

	static const float MOVE_SPEED;		// 移動速度
	static const int ATTACK_DAMAGE;	// 攻撃力
private:
	void UpdateJump();		// ジャンプ

	D3DXVECTOR3 m_move;
	int m_nLife;
};

#endif // !_BUG_ATTACK_H_
