//=============================================================
//
// �o�O�A�^�b�N [bug_attack.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "bug_attack.h"
#include "component/3d/collision.h"
#include "scripts/enemy.h"
#include "manager.h"
#include "benlib.h"

// �ÓI�����o�ϐ��̏�����
const float CBugAttack::MOVE_SPEED = 50.0f;
const int CBugAttack::ATTACK_DAMAGE = 4;

//=============================================================
// [CBugAttack] ������
//=============================================================
void CBugAttack::Init()
{
	CCollision* pCollision = CCollision::GetCollision(gameObject);
	if (pCollision == nullptr ||
		pCollision->GetRigidBody() == nullptr)
	{ // ���W�b�h�{�f�B��������Ȃ��Ƃ�
		return;
	}

	gameObject->GetComponent<CRigidBody>()->SetAlwayActive(true);
}

//=============================================================
// [CBugAttack] �X�V
//=============================================================
void CBugAttack::Update()
{
	if (CManager::GetPause())
		return;

	CCollision* pCollision = CCollision::GetCollision(gameObject);
	if (pCollision == nullptr ||
		pCollision->GetRigidBody() == nullptr)
	{ // ���W�b�h�{�f�B��������Ȃ��Ƃ�
		return;
	}

	// �W�����v
	UpdateJump();

	// �|��Ȃ��悤�ɂ���
	transform->SetRot(0.0f, transform->GetRotY(), 0.0f);

	// �ړ��ʂ̕����ɐi��
	pCollision->GetRigidBody()->setLinearVelocity(btVector3(
		sinf(transform->GetWRotY() + D3DX_PI * 0.5f) * MOVE_SPEED,
		pCollision->GetRigidBody()->getLinearVelocity().getY(),
		cosf(transform->GetWRotY() + D3DX_PI * 0.5f) * MOVE_SPEED));

	// �G���擾����
	std::vector<CEnemy*> pEnemyList = Component::GetComponents<CEnemy>(true);
	for (int i = 0; i < pEnemyList.size(); i++)
	{		
		if (PosDistance(pEnemyList[i]->transform->GetWPos(), transform->GetWPos()) < 30.0f)
		{ // �G���߂��ɂ���Ƃ�
			float fEnemyAngle = PosAngle(transform->GetWPos(), pEnemyList[i]->transform->GetWPos());
			transform->SetRot(0.0f, fEnemyAngle - D3DX_PI * 0.5f, 0.0f);
			//transform->Rotate(0.0f, (fEnemyAngle - D3DX_PI * 0.5f - transform->GetWRot().y) * 0.3f, 0.0f);
			break;
		}
	}

	// �Փ˔���
	GameObject* pDetectionObj = gameObject->FindNameChildren("Detection");
	if (pDetectionObj != nullptr &&
		CCollision::GetCollision(pDetectionObj) != nullptr)
	{
		std::vector<GameObject*>& pDetectionOverlapping = CCollision::GetCollision(pDetectionObj)->GetOverlappingGameObjects();
		for (int i = 0; i < pDetectionOverlapping.size(); i++)
		{
			if (pDetectionOverlapping[i]->GetTag() == "Enemy")
			{ // �G�ƏՓ˂����Ƃ�
				pDetectionOverlapping[i]->GetComponent<CEnemy>(true)->DamageEnemy(ATTACK_DAMAGE, transform->GetWPos());	// �G�Ƀ_���[�W��^����
				gameObject->Destroy(true);		// �����j������
			}
		}
	}

	// ����
	m_nLife--;
	if (m_nLife <= 0)
	{
		gameObject->Destroy(true);
	}
}

//=============================================================
// [CBugAttack] �W�����v
//=============================================================
void CBugAttack::UpdateJump()
{
	// ���C���΂��Ēn�ʂ����邩���肷��
	btVector3 btRayFrom = btVector3(transform->GetWPos().x, transform->GetWPos().y, transform->GetWPos().z);
	btVector3 btRayTo = btVector3(transform->GetWPos().x, transform->GetWPos().y - 5.0f, transform->GetWPos().z);
	btCollisionWorld::ClosestRayResultCallback rayCallback(btRayFrom, btRayTo);
	CManager::GetPhysics()->GetDynamicsWorld().rayTest(btRayFrom, btRayTo, rayCallback);
	if (rayCallback.hasHit())
	{ // �n�ʂɂ��Ă���Ƃ�
		btRayTo = btVector3(transform->GetWPos().x, transform->GetWPos().y, transform->GetWPos().z + 15.0f);
		btCollisionWorld::ClosestRayResultCallback rayJumpCallback(btRayFrom, btRayTo);
		CManager::GetPhysics()->GetDynamicsWorld().rayTest(btRayFrom, btRayTo, rayJumpCallback);
		// �W�����v
		if (rayJumpCallback.hasHit())
		{
			CCollision::GetCollision(gameObject)->GetRigidBody()->setLinearVelocity(
				CCollision::GetCollision(gameObject)->GetRigidBody()->getLinearVelocity() + btVector3(0.0f, 5.0f, 0.0f)
			);
		}
	}
}