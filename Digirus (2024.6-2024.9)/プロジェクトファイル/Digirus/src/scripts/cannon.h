//=============================================================
//
// ƒLƒƒƒmƒ“–C [cannon.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _CANNON_H_
#define _CANNON_H_

#include "component.h"

// ƒLƒƒƒmƒ“
class CCannon : public Component
{
public:
	void Init() override;
	void Update() override;
private:
	float m_fSpeed;		// ‘¬‚³
	int m_nInterval;		// •p“x
	int m_nLife;			// õ–½

	int m_nIntervalCounter;		// •p“xƒJƒEƒ“ƒ^[
};

// ƒLƒƒƒmƒ“‚Ì–C’e
class CCannonBullet : public Component
{
public:
	void Init() override;
	void Update() override;
	void OnTriggerEnter(GameObject* other) override;

	void SetLife(const int& nLife) { m_nLife = nLife; }
	void SetMove(const D3DXVECTOR3& move) { m_move = move; }

	static const int DAMAGE;
private:
	int m_nLife;							// õ–½
	D3DXVECTOR3 m_move;		// ˆÚ“®—Ê
};

#endif // !_CANNON_H_
