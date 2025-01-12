//=============================================================
//
// ���Ȃ�|�[�^�� [next_portal.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "next_portal.h"
#include "component/3d/field.h"
#include "component/3d/camera.h"
#include "component/3d/render_texture.h"
#include "scripts/camera_move.h"
#include "component/3d/collision.h"
#include "component/3d/light.h"
#include "component/3d/mesh.h"
#include "scene/game.h"
#include "manager.h"
#include "scene.h"

//=============================================================
// [CNextPortal] �R���X�g���N�^
//=============================================================
CNextPortal::CNextPortal()
{
	m_bIsGoal = false;
}

//=============================================================
// [CNextPortal] ������
//=============================================================
void CNextPortal::Init()
{
	// �f�B�X�v���C
	m_pDisplayObj = new GameObject();
	m_pDisplayObj->SetName("ComputerDisplay");
	CField* pDisplay = m_pDisplayObj->AddComponent<CField>();
	m_pDisplayObj->SetParent(gameObject);
	m_pDisplayObj->transform->Rotate(-D3DX_PI * 0.5f, 0.0f, 0.0f);

	// �s����̃J����
	m_pNextCamera = new GameObject();
	CCamera* pNextCamera = m_pNextCamera->AddComponent<CCamera>();
	pNextCamera->SetVisible(false);
	pNextCamera->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	m_pNextCamera->AddComponent<CCameraMove>()->enabled = false;
	m_pNextCamera->GetComponent<CCamera>()->GetSkybox()->SetTexture(0, "data\\TEXTURE\\SKY\\Daylight.png");


	// �����_�[�e�N�X�`���̐ݒ�
	CRenderTexture* pNextTexture = m_pNextCamera->AddComponent<CRenderTexture>();
	pNextTexture->SetTargetCamera(pNextCamera);
	pNextTexture->Create(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT);

	// �f�B�X�v���C�̃e�N�X�`���ݒ�
	pDisplay->BindTexture(pNextTexture->GetTexture());
}

//=============================================================
// [CNextPortal] �I��
//=============================================================
void CNextPortal::Uninit()
{
	m_pDisplayObj->Destroy();
	m_pDisplayObj = nullptr;
}

//=============================================================
// [CNextPortal] �X�V
//=============================================================
void CNextPortal::Update()
{
	if (CManager::GetPause())
		return;

	// �N���R���W�������擾
	GameObject* pEnter = gameObject->FindNameChildren("Enter");

	if (pEnter != nullptr)
	{
		// �R���W�����ɐN�����Ă���I�u�W�F�N�g���擾
		std::vector<GameObject*>& overlapping = CCollision::GetCollision(pEnter)->GetOverlappingGameObjects();

		for (int i = 0; i < (int)overlapping.size(); i++)
		{
			if (overlapping[i]->GetTag() == "Player")
			{ // �v���C���[�̂Ƃ�
				// �J�����̎擾
				GameObject* pCamera = GameObject::Find("Camera");
				if (pCamera == nullptr ||
					pCamera->GetComponent<CCameraMove>()->GetFocus() == CCameraMove::FOCUS::PORTAL)
				{
					return;
				}

				// �t�H�[�J�X���R���s���[�^�ɐ؂�ւ���
				pCamera->GetComponent<CCameraMove>()->SetNextPortal(this);
				pCamera->GetComponent<CCameraMove>()->SetFocus(CCameraMove::FOCUS::PORTAL);
			}
		}
	}
}

//=============================================================
// [CNextPortal] �R���W�����ɓ������Ƃ�
//=============================================================
void CNextPortal::OnTriggerEnter(GameObject* other)
{
	//if (other->GetTag() == "Player")
	//{ // �v���C���[�̂Ƃ�
	//	// �J�����̎擾
	//	GameObject* pCamera = GameObject::Find("Camera");
	//	if (pCamera == nullptr)
	//	{
	//		return;
	//	}

	//	// �t�H�[�J�X���R���s���[�^�ɐ؂�ւ���
	//	pCamera->GetComponent<CCameraMove>()->SetNextPortal(this);
	//	pCamera->GetComponent<CCameraMove>()->SetFocus(CCameraMove::FOCUS::PORTAL);
	//}
}

//=============================================================
// [CNextPortal] �f�B�X�v���C�T�C�Y�̐ݒ�
//=============================================================
void CNextPortal::SetDisplaySize(const float& x, const float& y)
{
	m_pDisplayObj->GetComponent<CField>()->Set(x, y);
}

//=============================================================
// [CNextPortal] �f�B�X�v���C�I�t�Z�b�g�̐ݒ�
//=============================================================
void CNextPortal::SetDisplayOffset(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	m_pDisplayObj->transform->SetPos(pos);
	m_pDisplayObj->transform->SetRot(rot);
}

//=============================================================
// [CNextPortal] �s����̐ݒ�
//=============================================================
void CNextPortal::SetNextPos(const D3DXVECTOR3& pos)
{
	m_nextPos = pos;

	// �J�����̐ݒ�
	ResetNextCamera();
}

//=============================================================
// [CNextPortal] �S�[���ɂ��邩
//=============================================================
void CNextPortal::SetIsGoal(const bool& bGoal)
{
	// ��Ԃ�ύX����
	m_bIsGoal = bGoal;

	// �|�[�^���̉�ʂ�ύX����
	CField* pDisplay = m_pDisplayObj->GetComponent<CField>();
	if (m_bIsGoal)
	{
		CGameScene* pGameScene = (CGameScene*)CSceneManager::GetInstance()->GetScene("game")->pScene;
		pDisplay->BindTexture(pGameScene->GetResultTexture());
	}
	else
	{
		pDisplay->BindTexture(m_pNextCamera->GetComponent<CRenderTexture>()->GetTexture());
	}
}

//=============================================================
// [CNextPortal] �J�����̃��Z�b�g
//=============================================================
void CNextPortal::ResetNextCamera()
{
	// �J�����̐ݒ�
	m_pNextCamera->transform->SetPos(m_nextPos + D3DXVECTOR3(0.0f, 70.0f, -70.0f));
	m_pNextCamera->transform->LookAt(m_nextPos + D3DXVECTOR3(0.0f, 10.0f, 10.0f));
}