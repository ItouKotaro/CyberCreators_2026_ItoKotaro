//=============================================================
//
// �X�L�� [skill.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "skill.h"
#include "manager.h"
#include "scripts/player.h"
#include "scripts/skillpt.h"
#include "scripts/bug_attack.h"

// �ÓI�����o�ϐ��̏�����
const int CHealSkill::HEAL_VALUE = 5;

//=============================================================
// [CSkillManager] ������
//=============================================================
void CSkillManager::Init()
{
	// �ϐ��̏�����
	for (int i = 0; i < MAX_SLOT; i++)
	{
		m_pSkillSlot[i] = nullptr;
	}


}

//=============================================================
// [CSkillManager] �I��
//=============================================================
void CSkillManager::Uninit()
{
	// �Z�b�g����Ă���X�L����j������
	for (int i = 0; i < MAX_SLOT; i++)
	{
		delete m_pSkillSlot[i];
		m_pSkillSlot[i] = nullptr;
	}
}

//=============================================================
// [CSkillManager] �X�V
//=============================================================
void CSkillManager::Update()
{
	CSkillPoint* pSkillPt = gameObject->GetComponent<CSkillPoint>();

	bool bPushSkillButton = false;
	for (int i = 0; i < MAX_SLOT; i++)
	{
		// �X�L���X���b�g�ɃZ�b�g����Ă��邩
		if (m_pSkillSlot[i] == nullptr)
		{
			continue;
		}

		if (CInputSystem::GetInstance()->onInput("skill_" + std::to_string(i)))
		{ // �X�L���g�p�{�^�������͂��ꂽ�Ƃ�
			// �X�L���|�C���g������邩
			if (m_pSkillSlot[i]->GetUsePoint() > pSkillPt->GetPoint())
			{ // �s��
				continue;
			}

			if (m_nPushCounter >= m_pSkillSlot[i]->GetPushTime())
			{ // ���������ԂɒB�����Ƃ�
				// �|�C���g����
				pSkillPt->AddPoint(-m_pSkillSlot[i]->GetUsePoint());

				// �X�L�����s
				m_pSkillSlot[i]->Use();
				m_nPushCounter = 0;	// �J�E���^�[���Z�b�g
			}
			else
			{
				m_nPushCounter++;
			}

			bPushSkillButton = true;		// �X�L���{�^���������Ă���
			break;
		}
	}

	// �X�L���{�^���������Ă��Ȃ������Ƃ�
	if (!bPushSkillButton)
	{
		m_nPushCounter = 0;	// �J�E���^�[���Z�b�g
	}

}

//=============================================================
// [CSkillManager] �X�L���ݒ�
//=============================================================
void CSkillManager::SetSkill(const int& nSlotID, CSkill* skill)
{
	if (0 <= nSlotID && nSlotID < MAX_SLOT)
	{
		m_pSkillSlot[nSlotID] = skill;
	}
}

//=============================================================
// [CSkillManager] �X�L���擾
//=============================================================
CSkill* CSkillManager::GetSkill(const int& nSlotID)
{
	if (0 <= nSlotID && nSlotID < MAX_SLOT)
	{
		return m_pSkillSlot[nSlotID];
	}
	return nullptr;
}



//=============================================================
// [CSkill] �R���X�g���N�^
//=============================================================
CSkill::CSkill() : m_nUsePoint(0), m_nPushTime(0)
{
	// �v���C���[��T��
	m_pPlayer = GameObject::Find("Player");
}

//=============================================================
// [CHealSkill] �R���X�g���N�^
//=============================================================
CHealSkill::CHealSkill()
{
	m_nUsePoint = 20;
	m_nPushTime = 20;
}

//=============================================================
// [CHealSkill] �g�p��
//=============================================================
void CHealSkill::Use()
{
	m_pPlayer->GetComponent<CPlayer>()->HealPlayer(HEAL_VALUE);
}

//=============================================================
// [CBugAttackSkill] �R���X�g���N�^
//=============================================================
CBugAttackSkill::CBugAttackSkill()
{
	m_nUsePoint = 20;
	m_nPushTime = 10;
}

//=============================================================
// [CBugAttackSkill] �g�p��
//=============================================================
void CBugAttackSkill::Use()
{
	D3DXVECTOR3 spawnPos = { 0.0f, 0.0f, 10.0f };
	D3DXVec3TransformCoord(&spawnPos, &spawnPos, &m_pPlayer->transform->GetMatrix());

	GameObject* pBugAttack = GameObject::LoadPrefab("data\\PREFAB\\mushi.pref",
		Transform(spawnPos,
			D3DXVECTOR3(0.0f, m_pPlayer->transform->GetWRotY(), 0.0f))
	);
	CBugAttack* pBug = pBugAttack->AddComponent<CBugAttack>();
}