//=============================================================
//
// ’e [bullet.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "component.h"

// ’eƒNƒ‰ƒX
class CBullet : public Component
{
public:
	void Init() override;
	void Update() override;
	void OnTriggerEnter(GameObject* other) override;
	void SetLife(const int& nLife);					// õ–½‚Ìİ’è
	void SetAttack(const int& nAttack);			// UŒ‚—Í‚Ìİ’è
	void SetMove(const D3DXVECTOR3& move);
	static const std::string BULLET_MESH;		// ’e‚ÌƒƒbƒVƒ…
	static const float SPEED;							// ˆÚ“®‘¬“x
private:
	int m_nLife;						// õ–½
	int m_nAttack;					// UŒ‚—Í
	D3DXVECTOR3 m_move;	// ˆÚ“®—Ê
};

#endif // !_BULLET_H_
