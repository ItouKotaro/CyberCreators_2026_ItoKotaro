//=============================================================
//
// �e [bullet.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "component.h"

// �e�N���X
class CBullet : public Component
{
public:
	void Init() override;
	void Update() override;
	void OnTriggerEnter(GameObject* other) override;
	void SetLife(const int& nLife);					// �����̐ݒ�
	void SetAttack(const int& nAttack);			// �U���͂̐ݒ�
	void SetMove(const D3DXVECTOR3& move);
	static const std::string BULLET_MESH;		// �e�̃��b�V��
	static const float SPEED;							// �ړ����x
private:
	int m_nLife;						// ����
	int m_nAttack;					// �U����
	D3DXVECTOR3 m_move;	// �ړ���
};

#endif // !_BULLET_H_
