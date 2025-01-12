//=============================================================
//
// �E�C���X [virus.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _VIRUS_H_
#define _VIRUS_H_

#include "component.h"

class CVirus : public Component
{
public:
	CVirus(const int& nPoint = 1);										// �R���X�g���N�^
	void Init() override;													// ������
	void Update() override;												// �X�V
	void OnTriggerEnter(GameObject* other) override;		// �����蔻��
	void SetPoint(const int& nPoint);									// �|�C���g�̐ݒ�
	static const float ATTRACT_DISTANCE;						// �����t���鋗��
	static const float ATTRACT_POWER;								// �����t�����
	static const float MIN_SIZE;										// �ŏ��T�C�Y
	static const float MAX_SIZE;										//	�ő�T�C�Y
private:
	int m_nPoint;							// �|�C���g
	D3DXVECTOR3 m_move;		// �ړ���
};

#endif // !_VIRUS_H_
