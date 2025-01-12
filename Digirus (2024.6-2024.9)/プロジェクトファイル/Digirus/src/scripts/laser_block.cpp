//=============================================================
//
// ���[�U�[�u���b�N [laser_block.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "laser_block.h"
#include "component/3d/collision.h"
#include "component/3d/mesh.h"
#include "scripts/player.h"

//=============================================================
// [CLaserBlock] ������
//=============================================================
void CLaserBlock::Init()
{
	// �ϐ��̏�����
	m_nDamage = 1;
	m_move = { 0.0f, 0.0f, 0.0f };
	m_nMoveCount = 0;
	m_nMoveCounter = 0;

	// �����蔻����쐬����
	m_pLaserCollision = new GameObject;
	m_pLaserCollision->SetParent(gameObject);
	m_pLaserCollision->AddComponent<CBoxCollider>();
	CCollision::GetCollision(m_pLaserCollision)->IsTrigger(true);

	// �o�[�𔼓����ɂ���
	gameObject->FindNameChildren("bar")->GetComponent<CMesh>()->SetAlpha(0.1f);
}

//=============================================================
// [CLaserBlock] �X�V
//=============================================================
void CLaserBlock::Update()
{
	// �o�[����]������
	gameObject->FindNameChildren("bar")->transform->Rotate(0.0f, 0.0f, 0.1f);

	// �v���C���[�ɂ��������Ƃ��̃_���[�W
	std::vector<GameObject*> pOverlapping = CCollision::GetCollision(m_pLaserCollision)->GetOverlappingGameObjects();
	for (int i = 0; i < pOverlapping.size(); i++)
	{
		if (pOverlapping[i]->GetName() == "Player")
		{ // �v���C���[�̂Ƃ�
			CPlayer* pPlayer = pOverlapping[i]->GetComponent<CPlayer>();
			if (pPlayer != nullptr)
			{
				pPlayer->DamagePlayer(m_nDamage);	// �_���[�W��^����
			}
		}
	}

	// �ړ�
	transform->Translate(m_move);

	// �J�E���^�[�ł̔��]
	if (m_nMoveCount != 0)
	{ // 0�ȊO�̂Ƃ�
		m_nMoveCounter++;
		if (m_nMoveCounter >= m_nMoveCount)
		{ // �J�E���g���������Ƃ�
			m_move *= -1;				// �ړ��ʔ��]
			m_nMoveCounter = 0;	// �J�E���^�[���Z�b�g
		}
	}
}

//=============================================================
// [CLaserBlock] ������
//=============================================================
void CLaserBlock::SetLength(const float& fLength)
{
	m_fLength = fLength;

	// �����蔻����X�V����
	m_pLaserCollision->GetComponent<CBoxCollider>()->SetSize({ 1.0f, 1.0f, fLength * 0.5f });
	m_pLaserCollision->GetComponent<CBoxCollider>()->SetOffset({0.0f, 0.0f, -fLength * 0.5f});
	CCollision::GetCollision(m_pLaserCollision)->UpdateCollision();

	// �o�[��L�΂�
	gameObject->FindNameChildren("bar")->transform->SetScale(0.8f, 0.8f, m_fLength);
}