//=============================================================
//
// �E�C���X [virus.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "virus.h"
#include "benlib.h"
#include "manager.h"
#include "component/3d/mesh.h"
#include "component/3d/collision.h"
#include "scripts/player.h"
#include "scripts/skillpt.h"

// �ÓI�����o�ϐ��̏�����
const float CVirus::ATTRACT_DISTANCE = 15.0f;
const float CVirus::ATTRACT_POWER = 0.1f;
const float CVirus::MIN_SIZE = 0.5f;
const float CVirus::MAX_SIZE = 3.5f;


//=============================================================
// [CVirus] �R���X�g���N�^
//=============================================================
CVirus::CVirus(const int& nPoint)
{
	m_nPoint = nPoint;
}

//=============================================================
// [CVirus] ������
//=============================================================
void CVirus::Init()
{
	// �ϐ��̏�����
	m_move = { 0.0f, 0.0f, 0.0f };

	SetPoint(m_nPoint);
}

//=============================================================
// [CVirus] �X�V
//=============================================================
void CVirus::Update()
{
	if (CManager::GetPause())
		return;

	// �v���C���[���擾����
	GameObject* pPlayer = GameObject::Find("Player");

	if (pPlayer != nullptr && pPlayer->GetComponent<CPlayer>()->enabled)
	{ // �v���C���[�����������Ƃ�
		// ���̋����ȓ��̏ꍇ�̓v���C���[�̕����֌�����
		float fDistance = PosPlaneDistance(pPlayer->transform->GetWPos(), transform->GetWPos());
		if (fDistance < ATTRACT_DISTANCE)
		{
			D3DXVECTOR3 vecAngle = PosRotation(transform->GetWPos(), pPlayer->transform->GetWPos() + D3DXVECTOR3(0.0f, 20.0f, 0.0f));
			m_move += vecAngle * ATTRACT_POWER;
		}

		// �ړ��ʂ̌���
		m_move -= m_move * 0.08f;

		// �ړ��ʂ𔽉f������
		gameObject->GetParent()->transform->Translate(m_move);
	}
}

//=============================================================
// [CVirus] �����蔻��
//=============================================================
void CVirus::OnTriggerEnter(GameObject* other)
{
	if (other->GetTag() == "Player")
	{ // �v���C���[�̂Ƃ�
		// �v���C���[�̃X�L���|�C���g�𑝉�������
		CSkillPoint* pSkillPt = other->GetComponent<CSkillPoint>();

		if (pSkillPt != nullptr)
		{
			pSkillPt->AddPoint(m_nPoint);

			// �j������
			gameObject->GetParent()->Destroy(true);
		}
	}
}

//=============================================================
// [CVirus] �|�C���g�̐ݒ�
//=============================================================
void CVirus::SetPoint(const int& nPoint)
{
	m_nPoint = nPoint;	// �|�C���g�̕ύX

	// ���f���̑傫����ύX����
	float fEveryScale = (MAX_SIZE - MIN_SIZE) * 0.01f;
	transform->SetScale(nPoint * fEveryScale + MIN_SIZE);
}