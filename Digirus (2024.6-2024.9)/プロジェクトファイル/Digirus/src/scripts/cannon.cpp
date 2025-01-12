//=============================================================
//
// �L���m���C [cannon.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "cannon.h"
#include "benlib.h"
#include "component/other/sound.h"
#include "scripts/player.h"
#include "manager.h"

// �ÓI�����o�ϐ��̏�����
const int CCannonBullet::DAMAGE = 30;

//=============================================================
// [CCannon] ������
//=============================================================
void CCannon::Init()
{
	// �ݒ肷��
	m_fSpeed = CDataField::GetDataField(gameObject)->GetFloat("Speed");
	m_nInterval = CDataField::GetDataField(gameObject)->GetInt("Interval");
	m_nIntervalCounter = m_nInterval;
	m_nLife = CDataField::GetDataField(gameObject)->GetInt("LifeTime");
}

//=============================================================
// [CCannon] �X�V
//=============================================================
void CCannon::Update()
{
	if (CManager::GetPause())
	{
		return;
	}

	// �ˏo�p�x
	m_nIntervalCounter--;
	if (m_nIntervalCounter <= 0)
	{ // �ˏo�^�C�~���O
		// �C�e�𐶐�����
		Transform trans(transform->GetWPos());
		GameObject* pBullet = GameObject::LoadPrefab("data\\PREFAB\\bullet.pref", trans);

		// �������������v�Z����
		D3DXVECTOR3 direction = { 0.0f, 0.0f, -1.0f };
		D3DXMATRIX mtxRot = transform->GetRotationMatrix();
		D3DXVec3TransformCoord(&direction, &direction, &mtxRot);

		// �ݒ肷��
		pBullet->AddComponent<CCannonBullet>();
		pBullet->GetComponent<CCannonBullet>()->SetLife(m_nLife);
		pBullet->GetComponent<CCannonBullet>()->SetMove(direction * m_fSpeed);

		// ����炷
		GameObject* pPopSound = new GameObject;
		pPopSound->transform->SetPos(transform->GetWPos());
		pPopSound->AddComponent<CSound>();
		pPopSound->GetComponent<CSound>()->LoadWav("data\\SOUND\\SE\\pop.wav");
		pPopSound->GetComponent<CSound>()->SetVolume(20);
		pPopSound->GetComponent<CSound>()->Play();
		pPopSound->GetComponent<CSound>()->IsStoppedDestroy();

		// ���Ԃ����Z�b�g
		m_nIntervalCounter = m_nInterval;
	}
}

//=============================================================
// [CCannonBullet] ������
//=============================================================
void CCannonBullet::Init()
{
	m_nLife = 120;
	m_move = {0.0f, 0.0f, 0.0f};
}

//=============================================================
// [CCannonBullet] �X�V
//=============================================================
void CCannonBullet::Update()
{
	if (CManager::GetPause())
	{
		return;
	}

	// �ړ�
	transform->Translate(m_move);

	// ����
	m_nLife--;
	if (m_nLife <= 0)
	{ // �������Ȃ��Ȃ����Ƃ�
		gameObject->Destroy();		// ���g��j������
	}
}

//=============================================================
// [CCannonBullet] �v���C���[�ɐG�ꂽ�Ƃ��̏���
//=============================================================
void CCannonBullet::OnTriggerEnter(GameObject* other)
{
	if (other->GetName() == "Player" &&
		other->GetComponent<CPlayer>() != nullptr)
	{
		// �v���C���[�Ƀ_���[�W��^����
		other->GetComponent<CPlayer>()->DamagePlayer(DAMAGE);
	}

	// �I�u�W�F�N�g��j������
	gameObject->Destroy(true);
}