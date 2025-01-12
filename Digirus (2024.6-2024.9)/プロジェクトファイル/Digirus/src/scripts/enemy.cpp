//=============================================================
//
// �G [enemy.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "enemy.h"
#include "benlib.h"
#include "manager.h"
#include "component/3d/collision.h"
#include "component/3d/billboard.h"
#include "scripts/player.h"
#include "scripts/break_effect.h"
#include "component/3d/particle.h"
#include "component/3d/motion.h"
#include "component/other/sound.h"

// �ÓI�����o�ϐ��̏�����
const float CEnemy::KNOCKBACK_POWER = 70.0f;
const float CEnemy::ROTATION_SPEED = 0.08f;

//=============================================================
// [CEnemy] ������
//=============================================================
void CEnemy::Init()
{
	m_nLife = 10;
	m_fSpeed = 1.0f;
	m_nAttack = 5;
	m_fAttackRange = 2.0f;
	m_nFoundDuration = 120;
	m_nDamagedMoveDuration = 60;
	m_nAttackDuration = 120;
	m_fObjectiveAngle = transform->GetWRotY();
	m_fCurrentAngle = transform->GetWRotY();

	m_nStateCounter = 0;
	m_state = STATE::SEARCH;

	// ���
	m_pStateUI = new GameObject();
	m_pStateUI->SetParent(gameObject);
	m_pStateUI->transform->Translate(0.0f, 20.0f, 0.0f);
	m_pStateUI->AddComponent<CBillboard>();
	m_pStateUI->GetComponent<CBillboard>()->Set(20.0f, 20.0f);

	m_pSearchRange = gameObject->FindNameChildren("Search");
}

//=============================================================
// [CEnemy] �I��
//=============================================================
void CEnemy::Uninit()
{
	// ���UI�̔j��
	m_pStateUI->Destroy();

	// �@�m�͈͂̃I�u�W�F�N�g��j��
	m_pSearchRange->Destroy();

	// ���̃I�u�W�F�N�g���j������
	gameObject->Destroy(true);
}

//=============================================================
// [CEnemy] �X�V
//=============================================================
void CEnemy::Update()
{
	if (CManager::GetPause())
		return;

	// ��Ԃ��Ƃ̓���
	switch (m_state)
	{
	case STATE::SEARCH:
		SearchAction();
		break;
	case STATE::FOUND:
		FoundAction();
		break;
	case STATE::ATTACK:
		AttackAction();
		break;
	case STATE::DAMAGE_MOVE:
		DamagedAction();
		break;
	}

	m_pStateUI->SetVisible(m_state != STATE::SEARCH || m_nStateCounter > 0);

	// ��ԊǗ�
	StateManager();

	// ��]����
	UpdateAngle();
}

//=============================================================
// [CEnemy] �_���[�W���󂯂��Ƃ��̏���
//=============================================================
void CEnemy::OnDamage()
{
	// �m�b�N�o�b�N
	btRigidBody* pRigidBody = CCollision::GetCollision(gameObject)->GetRigidBody();
	pRigidBody->activate(true);
	if (pRigidBody != nullptr)
	{ // ���W�b�h�{�f�B�����݂���Ƃ�
		btVector3 power = btVector3(sinf(m_damagedAngle + D3DX_PI) * KNOCKBACK_POWER, 0.0f, cosf(m_damagedAngle + D3DX_PI) * KNOCKBACK_POWER);

		// �͂�������
		pRigidBody->setLinearVelocity(power);
	}

	if (m_state == FOUND || m_state == ATTACK)
	{ // �{���ȊO�̂Ƃ�
		return; //�X�L�b�v
	}

	// �_���[�W���󂯂������Ɍ���
	m_fObjectiveAngle = m_damagedAngle + D3DX_PI;

	// �U���̎󂯂������Ɍ�����
	m_state = DAMAGE_MOVE;
	m_nStateCounter = m_nDamagedMoveDuration;

	// �������Ă��Ԃɂ���
	OnLostPlayer();
}

//=============================================================
// [CEnemy] ���S���̏���
//=============================================================
void CEnemy::OnDeath()
{
	// �G�̑傫��
	int nHeight = 20.0f;
	int nRange = 10.0f;

	// ���S�G�t�F�N�g�̕\��
	for (int i = 0; i < 12; i++)
	{
		GameObject* pBreakEffect = new GameObject();

		// �ʒu����
		pBreakEffect->transform->SetPos(transform->GetWPos());
		pBreakEffect->transform->Translate(rand() % (nRange * 2) - nRange, rand() % nHeight, rand() % (nRange * 2) - nRange);
		pBreakEffect->transform->Rotate(rand() % 314 * 0.01f, rand() % 314 * 0.01f, rand() % 314 * 0.01f);
		pBreakEffect->AddComponent<CBreakEffect>(rand() % 120 + 260, (rand() % 20 + 10)  * 0.03f);
	}

	// �I�u�W�F�N�g��j������
	gameObject->Destroy();
}

//=============================================================
// [CEnemy] �v���C���[�𔭌������Ƃ��̏���
//=============================================================
void CEnemy::OnFoundPlayer()
{
	// �r���{�[�h��\������
	CBillboard* pBillboard = m_pStateUI->GetComponent<CBillboard>();
	pBillboard->SetTexture("data\\TEXTURE\\ENEMY_STATE\\found.png");
}

//=============================================================
// [CEnemy] �v���C���[�����������Ƃ��̏���
//=============================================================
void CEnemy::OnLostPlayer()
{
	// �p�[�e�B�N����\������
	CBillboard* pBillboard = m_pStateUI->GetComponent<CBillboard>();
	pBillboard->SetTexture("data\\TEXTURE\\ENEMY_STATE\\not_found.png");
}

//=============================================================
// [CEnemy] �v���C���[�������t����
//=============================================================
void CEnemy::AttractPlayer()
{
	// ���W�b�h�{�f�B���擾����
	btRigidBody* pRigidBody = CCollision::GetCollision(gameObject)->GetRigidBody();

	// �v���C���[���擾����
	GameObject* pPlayer = GameObject::Find("Player");

	if (pRigidBody != nullptr && pPlayer != nullptr)
	{ // ���W�b�h�{�f�B�����݂���Ƃ�
		// �������ׂ��p�x���擾����
		float fAngle = PosAngle(transform->GetWPos(), pPlayer->transform->GetWPos());
		btVector3 power = btVector3(sinf(fAngle) * m_fSpeed, 0.0f, cosf(fAngle) * m_fSpeed);

		// ������ύX����
		m_fObjectiveAngle = fAngle + D3DX_PI;

		// �͂�������
		pRigidBody->translate(power);
	}
}

//=============================================================
// [CEnemy] �_���[�W��^����
//=============================================================
void CEnemy::DamageEnemy(const int& nDamage, const	 D3DXVECTOR3& damagePos)
{
	// �̗͂���_���[�W�����Z
	m_nLife -= nDamage;
	m_damagedAngle = PosAngle(transform->GetWPos(), damagePos);

	OnDamage();

	// ���S����
	if (m_nLife <= 0)
	{ // �̗͂������Ȃ����Ƃ�
		// ���S���̏���
		OnDeath();
	}
}

//=============================================================
// [CEnemy] �̗͂̐ݒ�
//=============================================================
void CEnemy::SetLife(const int& nLife)
{
	m_nLife = nLife;
}

//=============================================================
// [CEnemy] �̗͂̎擾
//=============================================================
int CEnemy::GetLife()
{
	return m_nLife;
}

//=============================================================
// [CEnemy] �����̐ݒ�
//=============================================================
void CEnemy::SetSpeed(const float& fSpeed)
{
	m_fSpeed = fSpeed;
}

//=============================================================
// [CEnemy] �����̎擾
//=============================================================
float CEnemy::GetSpeed()
{
	return m_fSpeed;
}

//=============================================================
// [CEnemy] �U���͂̐ݒ�
//=============================================================
void CEnemy::SetAttack(const int& nAttack)
{
	m_nAttack = nAttack;
}

//=============================================================
// [CEnemy] �U���͂̎擾
//=============================================================
int CEnemy::GetAttack()
{
	return m_nAttack;
}

//=============================================================
// [CEnemy] �U���͈͂̐ݒ�
//=============================================================
void CEnemy::SetAttackRange(const float& fRange)
{
	m_fAttackRange = fRange;
}

//=============================================================
// [CEnemy] �����̌p������
//=============================================================
void CEnemy::SetFoundDuration(const int& nDuration)
{
	m_nFoundDuration = nDuration;
}

//=============================================================
// [CEnemy] �U���̌p������
//=============================================================
void CEnemy::SetAttackDuration(const int& nDuration)
{

}

//=============================================================
// [CEnemy] �_���[�W���󂯂��Ƃ��̈ړ�����
//=============================================================
void CEnemy::SetDamagedMoveDuration(const int& nDuration)
{
	m_nDamagedMoveDuration = nDuration;
}

//=============================================================
// [CEnemy] �@�m�͈̓I�u�W�F�N�g�̎擾
//=============================================================
GameObject* CEnemy::GetSearchRange()
{
	return m_pSearchRange;
}

//=============================================================
// [CEnemy] �{������
//=============================================================
void CEnemy::SearchAction()
{
	if (!CCollision::isCollisionExist(m_pSearchRange))
	{ // �����蔻�肪���݂��Ȃ��Ƃ�
		return;
	}

	// �{���͈͓��Ƀv���C���[�����Ȃ������m�F����
	std::vector<GameObject*>& pOverlappingObjs = CCollision::GetCollision(m_pSearchRange)->GetOverlappingGameObjects();
	for (int i = 0; i < pOverlappingObjs.size(); i++)
	{
		if (pOverlappingObjs[i]->GetTag() == "Player")
		{ // �v���C���[���͈͓��ɓ����Ă���Ƃ�
			m_state = STATE::FOUND;						// ������Ԃɐ؂�ւ���
			m_nStateCounter = m_nFoundDuration;	// ������Ԃ̌p�����Ԃ�ݒ肷��
			OnFoundPlayer();
			return;
		}
	}
}

//=============================================================
// [CEnemy] ��������
//=============================================================
void CEnemy::FoundAction()
{
	// �U���͈͂ɓ����Ă��邩
	GameObject* pPlayer = GameObject::Find("Player");	// �v���C���[�̎擾
	if (pPlayer != nullptr)
	{
		if (PosDistance(transform->GetWPos(), pPlayer->transform->GetWPos()) <= m_fAttackRange)
		{ // �U���͈͓��̂Ƃ�
			m_nStateCounter = m_nAttackDuration;	// �U�����Ԃ̐ݒ�
			AttackDecision();									// �U������
			m_state = STATE::ATTACK;					// �U���Ɉڍs
			return;
		}
	}

	// �ēx�A�{���͈͓��Ƀv���C���[���������Ƃ�
	std::vector<GameObject*>& pOverlappingObjs = CCollision::GetCollision(m_pSearchRange)->GetOverlappingGameObjects();
	for (int i = 0; i < pOverlappingObjs.size(); i++)
	{
		if (pOverlappingObjs[i]->GetTag() == "Player")
		{ // �v���C���[���͈͓��ɓ����Ă���Ƃ�
			m_nStateCounter = m_nFoundDuration;	// �p�����Ԃ̍Đݒ���s��
			return;
		}
	}

	// �v���C���[���������Ƃ�
	if (m_nStateCounter == 1)
	{
		OnLostPlayer();	// �������C�x���g

		// �{����ԂɈȍ~
		m_nStateCounter = 120;
		m_state = STATE::SEARCH;
	}
}

//=============================================================
// [CEnemy] �U������
//=============================================================
void CEnemy::AttackAction()
{
	// �U���^�C�~���O
	GameObject* pPlayer = GameObject::Find("Player");	// �v���C���[�̎擾
	if (PosDistance(transform->GetWPos(), pPlayer->transform->GetWPos()) <= m_fAttackRange)
	{ // �U���͈͓��̂Ƃ�
		pPlayer->GetComponent<CPlayer>()->DamagePlayer(m_nAttack);		// �_���[�W��^����
	}
}

//=============================================================
// [CEnemy] �_���[�W���󂯂��Ƃ��̓���
//=============================================================
void CEnemy::DamagedAction()
{
	// ���W�b�h�{�f�B���擾����
	btRigidBody* pRigidBody = CCollision::GetCollision(gameObject)->GetRigidBody();
	if (pRigidBody != nullptr)
	{
		// �������ׂ��p�x���擾����
		btVector3 power = btVector3(sinf(m_damagedAngle) * m_fSpeed, 0.0f, cosf(m_damagedAngle) * m_fSpeed);

		// �͂�������
		pRigidBody->translate(power);
	}

	// �{�����s��
	SearchAction();
}

//=============================================================
// [CEnemy] �U������i�U����i�̌���j
//=============================================================
void CEnemy::AttackDecision()
{

}

//=============================================================
// [CEnemy] ��ԊǗ�
//=============================================================
void CEnemy::StateManager()
{
	// �J�E���^�[�̃f�N�������g
	m_nStateCounter--;

	// �J�E���^�[��0�ɂȂ����Ƃ�
	if (m_nStateCounter <= 0)
	{
		// �U����Ԃ���{���Ɉڍs����Ƃ�
		if (m_state == STATE::ATTACK)
		{
			OnLostPlayer();
			m_nStateCounter = 120;
		}

		m_state = STATE::SEARCH;	// �{���ɖ߂�
	}
}

//=============================================================
// [CEnemy] �����̍X�V
//=============================================================
void CEnemy::UpdateAngle()
{
	// ��]�̐���
	if (m_fCurrentAngle > D3DX_PI)
	{
		m_fCurrentAngle -= D3DX_PI * 2;
	}
	else if (m_fCurrentAngle < -D3DX_PI)
	{
		m_fCurrentAngle += D3DX_PI * 2;
	}

	if (m_fObjectiveAngle > D3DX_PI)
	{
		m_fObjectiveAngle -= D3DX_PI * 2;
	}
	else if (m_fObjectiveAngle < -D3DX_PI)
	{
		m_fObjectiveAngle += D3DX_PI * 2;
	}

	// ���R�ȉ�]����
	if (m_fCurrentAngle < m_fObjectiveAngle)
	{
		if (m_fCurrentAngle + D3DX_PI > m_fObjectiveAngle)
		{
			m_fCurrentAngle += fabsf(m_fObjectiveAngle - m_fCurrentAngle) * ROTATION_SPEED;
		}
		else
		{
			m_fCurrentAngle -= fabsf((m_fObjectiveAngle - D3DX_PI * 2) - m_fCurrentAngle) * ROTATION_SPEED;
		}
	}
	else if (m_fCurrentAngle > m_fObjectiveAngle)
	{
		if (m_fCurrentAngle - D3DX_PI < m_fObjectiveAngle)
		{
			m_fCurrentAngle -= fabsf(m_fObjectiveAngle - m_fCurrentAngle) * ROTATION_SPEED;
		}
		else
		{
			m_fCurrentAngle += fabsf((m_fObjectiveAngle + D3DX_PI * 2) - m_fCurrentAngle) * ROTATION_SPEED;
		}
	}

	// �p�x��ύX����
	transform->SetRot(0.0f, m_fCurrentAngle, 0.0f);
}



//=============================================================
// [CDebugEnemy] ������
//=============================================================
void CDebugEnemy::Init()
{
	CEnemy::Init();
	SetAttack(30);
	SetLife(10);
	SetFoundDuration(300);
	SetAttackRange(20.0f);
	SetSpeed(0.4f);
	SetDamagedMoveDuration(120);
}

//=============================================================
// [CDebugEnemy] ��������
//=============================================================
void CDebugEnemy::FoundAction()
{
	CEnemy::FoundAction();

	// �v���C���[�̕����Ɍ�����
	CEnemy::AttractPlayer();
}

//=============================================================
// [CDebugEnemy] �U������
//=============================================================
void CDebugEnemy::AttackAction()
{
	if (GetStateCounter() == 60)
	{ // �_���[�W�^�C�~���O
		CEnemy::AttackAction();
	}
}

//=============================================================
// [CSlimeEnemy] ������
//=============================================================
void CSlimeEnemy::Init()
{
	CEnemy::Init();
	SetLife(3);
	SetFoundDuration(300);
	SetAttackRange(15.0f);
	SetSpeed(0.6f);
	SetDamagedMoveDuration(120);
	SetAttack(10);
	SetAttackDuration(30);
}

//=============================================================
// [CSlimeEnemy] ��������
//=============================================================
void CSlimeEnemy::FoundAction()
{
	CEnemy::FoundAction();

	// ���W�b�h�{�f�B���擾
	auto pRigidbody = CCollision::GetCollision(gameObject)->GetRigidBody();

	if (fabsf(pRigidbody->getLinearVelocity().getY()) > 0.3f)
	{
		// �ړ�
		CEnemy::AttractPlayer();
	}

	// �W�����v�J�E���^�[
	m_nJumpCounter--;
	if (m_nJumpCounter <= 0)
	{
		// �W�����v����
		btVector3 vecJump = pRigidbody->getLinearVelocity() + btVector3(0.0f, 60.0f, 0.0f);
		pRigidbody->setLinearVelocity(vecJump);

		// ���[�V����
		gameObject->GetComponent<CMotionManager>()->Play("jump");

		// ��
		GameObject* pSound = new GameObject;
		pSound->SetParent(gameObject);
		pSound->AddComponent<CSound>();
		pSound->GetComponent<CSound>()->LoadWav("data\\SOUND\\SE\\slime.wav");
		pSound->GetComponent<CSound>()->SetVolume(18);
		pSound->GetComponent<CSound>()->Play();
		pSound->GetComponent<CSound>()->IsStoppedDestroy();

		// ���̃W�����v
		if (m_nJumpCounter <= 0)
		{
			m_nJumpCounter = 120 + rand() % 80;
		}
	}
}

//=============================================================
// [CSlimeEnemy] �U������
//=============================================================
void CSlimeEnemy::AttackAction()
{
	if (GetStateCounter() == 10)
	{ // �_���[�W�^�C�~���O
		CEnemy::AttackAction();
	}
}