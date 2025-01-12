//=============================================================
//
// �X�L�� [skill.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _SKILL_H_
#define _SKILL_H_

#include "component.h"

// �X�L�����N���X
class CSkill
{
public:
	CSkill();								// �R���X�g���N�^
	virtual void Use() = 0;		// �X�L���g�p���̏���
	int GetUsePoint() {				// ����|�C���g�̎擾
		return m_nUsePoint; 
	}
	int GetPushTime() {			// ���������Ԃ̎擾
		return m_nPushTime;
	}
protected:
	GameObject* m_pPlayer;	// �v���C���[
	int m_nUsePoint;				// �����|�C���g��
	int m_nPushTime;				// �g�p���ԁi�������������鎞�ԁj
};

// �q�[���X�L��
class CHealSkill : public CSkill
{
public:
	CHealSkill();
	void Use() override;					// �X�L���g�p���̏���
	static const int HEAL_VALUE;		// �񕜒l
};


// �o�O��
class CBugAttackSkill : public CSkill
{
public:
	CBugAttackSkill();
	void Use() override;					// �X�L���g�p���̏���
};


// �X�L���Ǘ�
class CSkillManager : public Component
{
public:
	void Init() override;											// ������
	void Uninit() override;											// �I��
	void Update() override;										// �X�V
	void SetSkill(const int& nSlotID, CSkill* skill);		// �X�L���̐ݒ�
	CSkill* GetSkill(const int& nSlotID);						// �X�L���̎擾

	static const int MAX_SLOT = 1;							// �ő�X�L���X���b�g
private:
	CSkill* m_pSkillSlot[MAX_SLOT];							// �X�L���X���b�g
	int m_nPushCounter;											// �������J�E���^�[
};

#endif // !_SKILL_H_
