//=============================================================
//
// �e [bullet.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "bullet.h"
#include "component/3d/mesh.h"
#include "component/3d/collision.h"
#include "scripts/enemy.h"
#include "scripts/exploding_barrels.h"

// �ÓI�����o�ϐ��̏�����
const std::string CBullet::BULLET_MESH = "data\\MODEL\\test_bullet.x";
const float CBullet::SPEED = 1.5f;

//=============================================================
// [CBullet] ������
//=============================================================
void CBullet::Init()
{
	// �e�̃��f��
	gameObject->AddComponent<CMesh>(BULLET_MESH);
	gameObject->AddComponent<CSphereCollider>(3.0f);
	gameObject->transform->SetScale(0.6f);
	CCollision::GetCollision(gameObject)->IsTrigger(true);
}

//=============================================================
// [CBullet] �X�V
//=============================================================
void CBullet::Update()
{
	// ������
	transform->Translate(m_move);

	// ����
	m_nLife--;
	if (m_nLife <= 0)
	{ // �������Ȃ��Ȃ����Ƃ�
		gameObject->Destroy();
	}
}

//=============================================================
// [CBullet] �����蔻��ɓ������Ƃ�
//=============================================================
void CBullet::OnTriggerEnter(GameObject* other)
{
	if (other->GetTag() == "Enemy")
	{ // ���������I�u�W�F�N�g���G�̂Ƃ�
		// �G�R���|�[�l���g���擾����
		CEnemy* pEnemy = other->GetComponent<CEnemy>(true);

		if (pEnemy != nullptr)
		{ // �G�R���|�[�l���g�����݂���Ƃ�
			pEnemy->DamageEnemy(1, transform->GetWPos());		// �G�Ƀ_���[�W��^����
		}
	}
	else if (other->GetTag() == "Exploding")
	{
		// ��������M�̃R���|�[�l���g���擾����
		CExplodingBarrels* pExploding = other->GetComponent<CExplodingBarrels>();

		if (pExploding != nullptr)
		{ // ��������M�̃R���|�[�l���g�����݂���Ƃ�
			pExploding->Explosion();			// �����������N����
		}
	}

	// �j������
	if (other->GetTag() != "Player")
	{
		gameObject->Destroy();
	}
}

//=============================================================
// [CBullet] �����̐ݒ�
//=============================================================
void CBullet::SetLife(const int& nLife)
{
	m_nLife = nLife;
}

//=============================================================
// [CBullet] �U���͂̐ݒ�
//=============================================================
void CBullet::SetAttack(const int& nAttack)
{
	m_nAttack = nAttack;
}

//=============================================================
// [CBullet] �ړ��ʂ̐ݒ�
//=============================================================
void CBullet::SetMove(const D3DXVECTOR3& move)
{
	m_move = move;
}