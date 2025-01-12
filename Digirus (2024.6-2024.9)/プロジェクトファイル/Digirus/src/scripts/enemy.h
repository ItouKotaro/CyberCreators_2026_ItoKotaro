//=============================================================
//
// �G [enemy.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "component.h"

// �G�i�x�[�X�j
class CEnemy : public Component
{
public:
	// ���
	typedef enum
	{
		SEARCH = 0,			// �{���i�����͈͂ɓ���܂őҋ@�j
		FOUND,					// ����
		ATTACK,				// �U��
		DAMAGE_MOVE		// �U�����󂯂������Ɉړ�
	}STATE;

	virtual void Init() override;								// ������
	virtual void Uninit() override;							// �I��
	virtual void Update() override;							// �X�V�i��ԊǗ��j

	// �C�x���g
	virtual void OnDamage();								// �_���[�W���󂯂��Ƃ��̏���
	virtual void OnDeath();									// ���S���̏���
	virtual void OnFoundPlayer();							// �v���C���[���������Ƃ��̏���
	virtual void OnLostPlayer();								// �v���C���[�����������Ƃ��̏���

	// ����
	virtual void SearchAction();								// �{������
	virtual void FoundAction();								// ��������
	virtual void AttackAction();								// �U������
	virtual void DamagedAction();							// �U�����󂯂��Ƃ��̓���
	virtual void AttackDecision();							// �U������
	
	void DamageEnemy(const int& nDamage,							// �_���[�W��^����
		const	 D3DXVECTOR3& damagePos = {0.0f, 0.0f, 0.0f});		
	GameObject* GetSearchRange();										// �@�m�͈̓I�u�W�F�N�g�̎擾
	void AttractPlayer();										// �v���C���[�Ɉ����t����

	// �ݒ荀��
	void SetLife(const int& nLife);											// �̗͂̐ݒ�
	int GetLife();																	// �̗͂̎擾
	void SetSpeed(const float& fSpeed);								// �����̐ݒ�
	float GetSpeed();															// �����̎擾
	void SetAttack(const int& nAttack);									// �U���͂̐ݒ�
	int GetAttack();																// �U���͂̎擾
	void SetAttackRange(const float& fRange);						// �U���͈͂̐ݒ�
	void SetFoundDuration(const int& nDuration);					// �����̌p�����Ԃ̐ݒ�
	void SetAttackDuration(const int& nDuration);					// �U���̌p�����Ԃ̐ݒ�
	void SetDamagedMoveDuration(const int& nDuration);		// �U�����󂯂������Ɉړ����鎞�Ԃ̐ݒ�
	int GetStateCounter() { return m_nStateCounter; }			// ��ԃJ�E���^�[�̎擾

	static const float KNOCKBACK_POWER;
	static const float ROTATION_SPEED;
private:
	void StateManager();										// ��ԊǗ�
	void UpdateAngle();										// �����̍X�V

	// ��{���
	int m_nLife;													// �̗�
	float m_fSpeed;												// ����
	int m_nAttack;												// �U����
	float m_fObjectiveAngle;									// �ڕW�̌���
	float m_fCurrentAngle;									// ���݂̌���

	// �@�m�֌W�̃p�����[�^
	GameObject* m_pSearchRange;						// �@�m�͈�
	float m_fAttackRange;									// �U���͈�
	int m_nFoundDuration;									// ������Ԃ̌p������
	float m_damagedAngle;									// �_���[�W���󂯂�����
	int m_nDamagedMoveDuration;						// �U�����󂯂������Ɉړ����鎞��
	int m_nAttackDuration;									// �U����Ԃ̌p������

	// ���
	STATE m_state;												// ���
	int m_nStateCounter;										// ��ԃJ�E���^�[
	GameObject* m_pStateUI;								// ��ԕ\��
};

// �f�o�b�O�G
class CDebugEnemy : public CEnemy
{
public:
	void Init() override;				// ������
	void FoundAction() override;	// ��������
	void AttackAction() override;	// �U������
private:

};

// �X���C��
class CSlimeEnemy : public CEnemy
{
public:
	void Init() override;				// ������
	void FoundAction() override;	// ��������
	void AttackAction() override;	// �U������
private:
	int m_nJumpCounter;				// �W�����v�J�E���^�[
};

#endif // !_ENEMY_H_
