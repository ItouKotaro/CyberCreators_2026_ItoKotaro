//=============================================================
//
// �_���[�W�{�[�� [damage_ball.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _DAMAGE_BALL_H_
#define _DAMAGE_BALL_H_

#include "component.h"

// �_���[�W�{�[���̏o��
class CDamageBallEmitter : public Component
{
public:
	void Init() override;
	void Update() override;
private:
	int m_nLength;				// ����
	int m_nBetweenRand;		// �����_����
	int m_nInterval;					// �Ԋu
	int m_nIntervalCounter;		// �Ԋu�J�E���^�[
	int m_nLife;						// ����
};

// �_���[�W�{�[��
class CDamageBall : public Component
{
public:
	void Init() override;
	void Update() override;
	void OnTriggerEnter(GameObject* other) override;
	void SetLife(const int& nLife) { m_nLife = nLife; }
private:
	int m_nLife;		// ����
};

#endif // !_DAMAGE_BALL_H_
