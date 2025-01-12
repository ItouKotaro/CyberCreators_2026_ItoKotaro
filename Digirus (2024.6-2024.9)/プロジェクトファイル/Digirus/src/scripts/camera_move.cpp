//=============================================================
//
// �J�����̓��� [camera_move.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "camera_move.h"
#include "component/3d/camera.h"
#include "component/3d/mesh.h"
#include "component/other/sound.h"
#include "scene/game.h"
#include "scripts/player.h"
#include "manager.h"
#include "benlib.h"

// �ÓI�����o�ϐ��̏�����
const int CCameraMove::CHANGING_TIME = 90;
const int CCameraMove::CHANGE_TIMING = 40;

//=============================================================
// [CCameraMove] ������
//=============================================================
void CCameraMove::Init()
{
	m_focus = FOCUS::PLAYER;
	m_nChangedCounter = 0;
	m_playerCameraOffset = { 0.0f, 40.0f, -80.0f };
	m_nShakePower = 0;
	m_nShakeTime = 0;
}

//=============================================================
// [CCameraMove] �X�V
//=============================================================
void CCameraMove::Update()
{
	if (CManager::GetPause())
		return;

	switch (m_focus)
	{
	case FOCUS::PLAYER:
		PlayerFocus();
		break;
	case FOCUS::PORTAL:
		PortalFocus();
		break;
	}

	// �U��
	if (m_nShakeTime > 0)
	{
		D3DXVECTOR3 shake = { 
			(float)(rand() % (m_nShakePower * 2) - m_nShakePower) * 0.1f,
			(float)(rand() % (m_nShakePower * 2) - m_nShakePower) * 0.1f,
			(float)(rand() % (m_nShakePower * 2) - m_nShakePower) * 0.1f
		};
		transform->SetPos(transform->GetPos() + shake);

		// �J�E���^�[��i�߂�
		m_nShakeTime--;
	}
}

//=============================================================
// [CCameraMove] �t�H�[�J�X�Ώۂ̐ݒ�
//=============================================================
void CCameraMove::SetFocus(FOCUS focus)
{
	m_focus = focus;
	m_portalProgress = PORTAL_PROGRESS::INIT_POS;
}

//=============================================================
// [CCameraMove] �v���C���[�ւ̃t�H�[�J�X
//=============================================================
void CCameraMove::PlayerFocus()
{
	// �v���C���[���擾����
	GameObject* pPlayer = GameObject::Find("Player");

	if (pPlayer == nullptr)
	{
		return;	// �擾�ł��Ȃ������Ƃ��͏������X���[
	}

	// �J�����̖ړI�n���v�Z����
	D3DXVECTOR3 objectivePos;
	objectivePos = pPlayer->transform->GetWPos();
	if (m_nChangedCounter > CHANGE_TIMING)
		objectivePos += m_playerCameraOffset;
	else
		objectivePos += {0.0f, 40.0f, -80.0f};
	
	
	// ����������
	D3DXVECTOR3 cameraPos = transform->GetWPos();
	cameraPos += (objectivePos - transform->GetWPos()) * 0.08f;

	// �ʒu��K�p����
	transform->SetPos(cameraPos);

	// �w��̕����Ɍ�������
	transform->LookAt(pPlayer->transform->GetWPos() + D3DXVECTOR3(0.0f, 10.0f, 10.0f));

	if (m_nChangedCounter > 0)
	{
		m_nChangedCounter--;
	}
}

//=============================================================
// [CCameraMove] �|�[�^���ւ̃t�H�[�J�X
//=============================================================
void CCameraMove::PortalFocus()
{
	// �|�[�^�����ݒ�ς݂�
	if (m_portal == nullptr)
	{ // �ݒ肳��Ă��Ȃ�
		return;	// �X�L�b�v
	}

	if (GameObject::Find("Player") == nullptr)
	{ // �v���C���[�����݂��Ȃ��Ƃ�
		return;	// �X�L�b�v
	}

	// �i�s�x���Ƃ̏���
	D3DXVECTOR3 objectivePos, frontPos, cameraPos, targetPos;;
	D3DXVECTOR3 cameraRot, objectiveRot;
	std::vector<GameObject*> pAllChildren = GameObject::Find("Player")->GetChildren(true);

	// �S�[�����̍X�V����
	if (m_portal->GetIsGoal() && m_portalProgress != PORTAL_PROGRESS::GOAL)
	{
		CGameScene* pGameScene = (CGameScene*)CSceneManager::GetInstance()->GetScene("game")->pScene;
		pGameScene->UpdateGoal();
	}

	switch (m_portalProgress)
	{
	case PORTAL_PROGRESS::INIT_POS:
		// �v���C���[���\���ɂ���
		for (int i = 0; i < (int)pAllChildren.size(); i++)
		{
			if (pAllChildren[i]->GetComponent<CMesh>() != nullptr)
			{
				pAllChildren[i]->SetActive(false);
			}
		}
		GameObject::Find("Player")->GetComponent<CPlayer>()->enabled = false;

		// �|�[�^���̕����Ɍ�������
		transform->LookAt(m_portal->GetDisplayObj()->transform->GetWPos());

		// �R���s���[�^�[�̈ʒu���珉���n������o��
		objectivePos = { 0.0f, 100.0f, 0.0f };
		D3DXVec3TransformCoord(&objectivePos, &objectivePos, &m_portal->GetDisplayObj()->transform->GetMatrix());

		if (m_portal->GetIsGoal())
		{
			CGameScene* pGameScene = (CGameScene*)CSceneManager::GetInstance()->GetScene("game")->pScene;
			pGameScene->onInitGoal();
		}

		// ����������
		cameraPos = transform->GetWPos();
		cameraPos += (objectivePos - transform->GetWPos()) * 0.08f;
		transform->SetPos(cameraPos);		// �ʒu��K�p����

		// ���̒i�K��
		if (PosDistance(transform->GetWPos(), objectivePos) < 0.4f)
		{
			// �����ʒu�Ɉړ�
			transform->SetPos(objectivePos);

			// �v���C���[��]�ڐ�Ɉړ�����
			if (!m_portal->GetIsGoal())
			{ // �S�[������Ȃ��Ƃ�
				CCollision::GetCollision(GameObject::Find("Player"))->GetRigidBody()->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
				CCollision::GetCollision(GameObject::Find("Player"))->GetRigidBody()->getWorldTransform().getOrigin() = btVector3(
					m_portal->GetNextPos().x, m_portal->GetNextPos().y, m_portal->GetNextPos().z);

				// �v���C���[��L���ɂ���
				GameObject::Find("Player")->GetComponent<CPlayer>()->enabled = true;

				// �v���C���[�̃Z�[�u�|�C���g��ݒ肷��
				GameObject::Find("Player")->GetComponent<CPlayer>()->SetSavePoint(m_portal->GetNextPos());
			}

			// ����炷
			GameObject* pGotoSound = new GameObject;
			pGotoSound->AddComponent<CSound>()->LoadWav("data\\SOUND\\SE\\goto_display.wav");
			pGotoSound->GetComponent<CSound>()->IsStoppedDestroy();
			pGotoSound->GetComponent<CSound>()->Play();

			m_portalProgress = PORTAL_PROGRESS::GO_MOVE;		// ����
		}
		break;

	case PORTAL_PROGRESS::GO_MOVE:

		// �ړ���J�����̃t�H�[�J�X���v���C���[�ɂ���
		m_portal->GetNextCamera()->GetComponent<CCameraMove>()->enabled = true;

		// �v���C���[��\������
		if (!m_portal->GetIsGoal())
		{
			for (int i = 0; i < (int)pAllChildren.size(); i++)
			{
				if (pAllChildren[i]->GetComponent<CMesh>() != nullptr)
				{
					pAllChildren[i]->SetActive(true);
				}
			}
		}

		// �|�[�^���̕����Ɍ�������
		targetPos = m_portal->GetDisplayObj()->transform->GetWPos();
		float fTargetAngle, fTargetHeightAngle;
		fTargetAngle = -atan2f(targetPos.x - transform->GetWPos().x, targetPos.z - transform->GetWPos().z);
		fTargetHeightAngle = atan2f(sqrtf(fabsf(targetPos.x - transform->GetWPos().x) * fabsf(targetPos.x - transform->GetWPos().x) +
			fabsf(targetPos.z - transform->GetWPos().z) * fabsf(targetPos.z - transform->GetWPos().z)), (targetPos.y - transform->GetWPos().y));
		objectiveRot = D3DXVECTOR3(-fTargetHeightAngle + D3DX_PI * 0.5f, fTargetAngle, 0.0f);
		
		objectiveRot.z = m_portal->GetDisplayObj()->transform->GetWRotZ() * -1;

		// �������錸��
		cameraRot = transform->GetWRot();
		cameraRot += (objectiveRot - transform->GetWRot()) * 0.08f;
		transform->SetRot(cameraRot);

		// �R���s���[�^�[�̈ʒu�����ʂ�����o��
		frontPos = { m_portal->GetDisplayAdjust().x, m_portal->GetFrontStop(), m_portal->GetDisplayAdjust().y };
		D3DXVec3TransformCoord(&frontPos, &frontPos, &m_portal->GetDisplayObj()->transform->GetMatrix());

		// ����������
		cameraPos = transform->GetWPos();
		cameraPos += (frontPos - transform->GetWPos()) * 0.08f;
		transform->SetPos(cameraPos);		// �ʒu��K�p����

		// ���̒i�K��
		if (PosDistance(transform->GetWPos(), frontPos) < 0.001f)
		{
			if (m_portal->GetIsGoal())
			{ // �S�[���̂Ƃ�
				// �f�[�^���L�^����
				CGameScene* pGameScene = (CGameScene*)CSceneManager::GetInstance()->GetScene("game")->pScene;
				pGameScene->SavePlayData();

				// �S�[���ɐ؂�ւ���
				m_portalProgress = PORTAL_PROGRESS::GOAL;
				break;
			}

			// �ړ���̃J������L���ɂ���
			m_portal->GetNextCamera()->GetComponent<CCamera>()->SetVisible(true);

			// ���C���J�����𖳌��ɂ���
			gameObject->GetComponent<CCamera>()->SetVisible(false);

			m_portalProgress = PORTAL_PROGRESS::NEXT_STAGE;		// ����
		}
		break;

	case PORTAL_PROGRESS::NEXT_STAGE:		// ���̃X�e�[�W�ւƊ��S�ڍs����
		// ���̏ꏊ�ւƃJ�������ړ�����
		transform->SetPos(m_portal->GetNextCamera()->transform->GetWPos());
		transform->SetRot(m_portal->GetNextCamera()->transform->GetWRot());

		// �I����
		m_portalProgress = PORTAL_PROGRESS::END;
		break;

	case PORTAL_PROGRESS::END:
		// ���C���J������L���ɂ���
		gameObject->GetComponent<CCamera>()->SetVisible(true);

		// �ړ���̃J�����𖳌��ɂ���
		m_portal->GetNextCamera()->GetComponent<CCamera>()->SetVisible(false);
		m_portal->GetNextCamera()->GetComponent<CCameraMove>()->enabled = false;
		m_portal->ResetNextCamera();
		m_portal = nullptr;

		// �t�H�[�J�X���v���C���[�Ɉڂ�
		m_focus = FOCUS::PLAYER;
		m_portalProgress = PORTAL_PROGRESS::INIT_POS;
		break;

	case PORTAL_PROGRESS::GOAL:
		// ���U���g�^�O�ȊO�̃I�u�W�F�N�g��j������
		std::vector<GameObject*> pObjects = GameObject::GetAllGameObjects();
		for (int i = 0; i < pObjects.size(); i++)
		{
			if (pObjects[i]->GetTag() != "result" && pObjects[i]->GetTag() != TAG_NODESTROY)
			{ // ���U���g�^�O�ȊO�̏ꍇ
				pObjects[i]->Destroy();	// �j��
			}
			else
			{ // ���U���g�^�O���t���Ă���ꍇ
				pObjects[i]->SetVisible(true);	// �\��
			}
		}

		// �S�[���̍ŏI����
		CGameScene* pGameScene = (CGameScene*)CSceneManager::GetInstance()->GetScene("game")->pScene;
		pGameScene->FinishGoal();
		break;
	}
}

//=============================================================
// [CCameraMove] �|�[�^���̐ݒ�
//=============================================================
void CCameraMove::SetNextPortal(CNextPortal* pNextPortal)
{
	m_portal = pNextPortal;
}

//=============================================================
// [CCameraMove] �U��
//=============================================================
void CCameraMove::SetShake(const int& nPower, const int& nTime)
{
	m_nShakePower = nPower;
	m_nShakeTime = nTime;
}

//=============================================================
// [CChangePlayerFocus] �R���W�����ɂ���Ƃ�
//=============================================================
void CChangePlayerFocus::OnTriggerStay(GameObject* other)
{
	if (other->GetTag() == "Player")
	{ // �v���C���[�̂Ƃ�
		GameObject::Find("Camera")->GetComponent<CCameraMove>()->SetCameraPlayerOffset(m_changedOffset);
	}
}
