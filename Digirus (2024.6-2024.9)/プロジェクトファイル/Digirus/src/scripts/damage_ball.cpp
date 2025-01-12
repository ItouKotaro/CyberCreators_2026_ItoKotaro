//=============================================================
//
// �_���[�W�{�[�� [damage_ball.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "damage_ball.h"
#include "scripts/player.h"
#include "manager.h"

//=============================================================
// [CDamageBallEmitter] ������
//=============================================================
void CDamageBallEmitter::Init()
{
	CDataField* pDataField = CDataField::GetDataField(gameObject);
	m_nLength = pDataField->GetInt("Length");
	m_nInterval = pDataField->GetInt("Interval");
	m_nIntervalCounter = 0;
	m_nBetweenRand = pDataField->GetInt("BetweenRand");
	m_nLife = pDataField->GetInt("Life");
}

//=============================================================
// [CDamageBallEmitter] �X�V
//=============================================================
void CDamageBallEmitter::Update()
{
	if (CManager::GetPause())
		return;

	// �����^�C�~���O
	m_nIntervalCounter--;
	if (m_nIntervalCounter <= 0)
	{ // �^�C�~���O�������Ƃ�
		// �I�u�W�F�N�g�𐶐�����
		GameObject* pBall = GameObject::LoadPrefab("data\\PREFAB\\iron_ball.pref");
		pBall->FindNameChildren("Damage")->AddComponent<CDamageBall>();
		pBall->FindNameChildren("Damage")->GetComponent<CDamageBall>()->SetLife(m_nLife);

		// �ʒu�����߂�
		D3DXVECTOR3 vecPos = { (float)(rand() % m_nLength - m_nLength / 2), 0.0f, 0.0f };
		D3DXMATRIX mtxRot = transform->GetRotationMatrix();
		D3DXVec3TransformCoord(&vecPos, &vecPos, &mtxRot);
		vecPos += transform->GetWPos();

		// �ړ�����
		pBall->transform->SetPos(vecPos);

		// ���̎��Ԃ����߂�
		m_nIntervalCounter = m_nInterval + rand() % (m_nBetweenRand * 2) - m_nBetweenRand;
	}
}

//=============================================================
// [CDamageBall] ������
//=============================================================
void CDamageBall::Init()
{
	m_nLife = 120;
}

//=============================================================
// [CDamageBall] �X�V
//=============================================================
void CDamageBall::Update()
{
	if (CManager::GetPause())
		return;

	// ����
	m_nLife--;
	if (m_nLife <= 0)
	{ // �s�����Ƃ�
		gameObject->GetParent()->Destroy(true);		// �j������
	}
}

//=============================================================
// [CDamageBall] �v���C���[�ɂ��������Ƃ�
//=============================================================
void CDamageBall::OnTriggerEnter(GameObject* other)
{
	if (other->GetName() == "Player" &&
		other->GetComponent<CPlayer>() != nullptr)
	{ // �v���C���[�ɐG�ꂽ�Ƃ�
		other->GetComponent<CPlayer>()->DamagePlayer(20);	// �_���[�W��^����
		gameObject->GetParent()->Destroy(true);		// �j������
	}
}