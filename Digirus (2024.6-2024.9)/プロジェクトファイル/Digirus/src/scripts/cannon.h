//=============================================================
//
// �L���m���C [cannon.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _CANNON_H_
#define _CANNON_H_

#include "component.h"

// �L���m��
class CCannon : public Component
{
public:
	void Init() override;
	void Update() override;
private:
	float m_fSpeed;		// ����
	int m_nInterval;		// �p�x
	int m_nLife;			// ����

	int m_nIntervalCounter;		// �p�x�J�E���^�[
};

// �L���m���̖C�e
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
	int m_nLife;							// ����
	D3DXVECTOR3 m_move;		// �ړ���
};

#endif // !_CANNON_H_
