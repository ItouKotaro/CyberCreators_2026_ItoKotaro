//=============================================================
//
// ���[�U�[�u���b�N [laser_block.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _LASER_BLOCK_H_
#define _LASER_BLOCK_H_

#include "component.h"

// ���[�U�[�u���b�N
class CLaserBlock : public Component
{
public:
	void Init() override;
	void Update() override;
	void SetLength(const float& fLength);
	void SetDamage(const int& nDamage) { m_nDamage = nDamage; }
	void SetMove(const D3DXVECTOR3& move) { m_move = move; }
	void SetMoveCounter(const int& nCount) { m_nMoveCount = nCount; }
private:
	GameObject* m_pLaserCollision;
	float m_fLength;							// ���[�U�[�̒���
	int m_nDamage;							// �_���[�W
	D3DXVECTOR3 m_move;				// �ړ���
	int m_nMoveCount;						// �ړ��ʃJ�E���g
	int m_nMoveCounter;						// �ړ��ʃJ�E���^�[
};

#endif // !_LASER_BLOCK_H_
