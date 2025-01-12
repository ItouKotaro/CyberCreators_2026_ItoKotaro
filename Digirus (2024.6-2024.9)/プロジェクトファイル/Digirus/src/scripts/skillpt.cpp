//=============================================================
//
// �X�L���|�C���g [skillpt.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "skillpt.h"

// �ÓI�����o�ϐ��̏�����
const int CSkillPoint::MAX_POINT = 100;

//=============================================================
// [CSkillPoint] ������
//=============================================================
void CSkillPoint::Init()
{
	// UI�p�̃Q�[���I�u�W�F�N�g���쐬
	m_pUI = new GameObject();
	m_pUI->transform->SetPos(150.0f, 150.0f, 0.0f);
	m_pUI->SetName("SkillPointUI");
	m_pUI->SetTag("UI");
	m_pUI->AddComponent<CSkillPointUI>();

	// �����|�C���g��ݒ�
	SetPoint(0);
}

//=============================================================
// [CSkillPoint] �X�V
//=============================================================
void CSkillPoint::Update()
{
	
}

//=============================================================
// [CSkillPoint] �|�C���g�̐ݒ�
//=============================================================
void CSkillPoint::AddPoint(const int& nAddPoint)
{
	int nAddedPoint = m_nPoint + nAddPoint;	// ���Z��̃|�C���g

	if (nAddedPoint > MAX_POINT)
	{ // �ő�|�C���g�𒴂��Ă���Ƃ�
		SetPoint(MAX_POINT);
	}
	else if (nAddedPoint < 0)
	{ // 0��������Ă���Ƃ�
		SetPoint(0);
	}
	else
	{ // �ʏ���Z
		SetPoint(nAddedPoint);
	}
}

//=============================================================
// [CSkillPoint] �|�C���g�̐ݒ�
//=============================================================
void CSkillPoint::SetPoint(const int& nPoint)
{
	if (0 <= nPoint && nPoint <= MAX_POINT)
	{
		m_nPoint = nPoint;
	}

	// UI���X�V����
	m_pUI->GetComponent<CSkillPointUI>()->SetPoint(m_nPoint);
}