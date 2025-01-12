//=============================================================
//
// �v���C���[ [player.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "component.h"
#include "internal/physics.h"

// �v���C���[�N���X
class CPlayer : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DamagePlayer(const int& nDamage);
	void HealPlayer(const int& nHeal);
	int GetLife() { return m_nLife; }
	GameObject* GetHPBar() { return m_pHPBarObj; }
	GameObject* GetStaminaBar() { return m_pStaminaBarObj; }
	void SetSavePoint(const D3DXVECTOR3& pos) { m_savePoint = pos; }

	void OnDeath();											// ���S���̏���

	static const float MOVE_SPEED;					// �ړ����x
	static const float DASH_RATE;						// �_�b�V���ɂ��㏸��
	static const int REVIVAL;								// ������
	static const int DEF_LIFE;							// �����̗�
	static const int DEF_STAMINA;					// �����X�^�~�i
	static const float ROTATION_SPEED;			// ��]���x

	// ����X�^�~�i
	static const int DASH_STAMINA;					// �_�b�V���̏���X�^�~�i
	static const int ROLLING_STAMINA;				// ��]�̏���X�^�~�i
private:
	enum STATE
	{
		STAY,		// �ҋ@
		WALK,		// ����
		ATTACK,	// �U��
		ROLL,		// ��]
	};

	void MoveControl();									// �ړ�����
	float GetMoveAngle();									// ���������̊p�x���擾����
	void AttackControl();									// �U������
	void AttackAction();									// �U������
	void OnDamage();										// �_���[�W���󂯂��Ƃ��̏���
	void UpdateAngle();									// �����̍X�V
	void UpdateState();									// ��Ԃ̍X�V
	void FallJudge();											// ��������i�ǂɂ������Ȃ����߁j
	void RollingAction();									// ��]
	void UpdateStamina();								// �X�^�~�i�̍X�V

	GameObject* m_pAttackCollision;				// �U���͈�
	GameObject* m_pReachCollision;				// ���[�`�͈�
	GameObject* m_pWeaponObj;					// ����
	GameObject* m_pHPBarObj;						// �̗̓o�[
	GameObject* m_pStaminaBarObj;				// �X�^�~�i�o�[
	GameObject* m_pRevivalText;					// ������UI
	int m_nLife;												// �̗�
	int m_nStamina;										// �X�^�~�i
	float m_fObjectiveAngle;								// �ڕW�̌���
	float m_fCurrentAngle;								// ���݂̌���
	int m_nParticleCounter;								// �p�[�e�B�N���̃J�E���^�[
	bool m_bDash;											// �_�b�V��
	bool m_bIsFloor;										// �n�ʂ�
	int m_nRevival;											// �������C�t
	D3DXVECTOR3 m_savePoint;						// �Z�[�u�|�C���g

	// ���[�V����
	STATE m_state;											// ���
	int m_nStateCounter;									// ��ԃJ�E���^�[

	// �U���֌W
	int m_nChainAttack;									// �A��

};

#endif // !_PLAYER_H_
