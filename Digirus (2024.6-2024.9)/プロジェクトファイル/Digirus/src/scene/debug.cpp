//=============================================================
//
// �f�o�b�N�V�[�� [debug.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "debug.h"
#include "component/3d/camera.h"
#include "component/3d/collision.h"
#include "component/3d/light.h"
#include "scripts/camera_move.h"


//=============================================================
// [CDebugScene] ������
//=============================================================
void CDebugScene::Init()
{
	// �Q�[���I�u�W�F�N�g�̐���
	m_pCamera = new GameObject();
	m_pCamera->transform->Translate(0.0f, 100.0f, -100.0f);
	m_pCamera->transform->LookAt({ 0.0f, 0.0f, 0.0f });
	m_pCamera->SetName("Camera");

	// �R���|�[�l���g�̒ǉ�
	m_pCamera->AddComponent<CCamera>();
	m_pCamera->AddComponent<CCameraMove>();

	// �F�̐ݒ�
	CCamera* pCameraComp = m_pCamera->GetComponent<CCamera>();
	pCameraComp->SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));

	// ���C�g�̍쐬
	GameObject* pD3DLight = new GameObject();
	pD3DLight->SetTag(TAG_NODESTROY);
	CD3DLight* pLightComp = pD3DLight->AddComponent<CD3DLight>();
	pLightComp->SetType(D3DLIGHT_DIRECTIONAL);
	pLightComp->SetDiffuse(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pLightComp->SetDirection(D3DXVECTOR3(0.22f, -0.87f, 0.44f));

	pLightComp = pD3DLight->AddComponent<CD3DLight>();
	pLightComp->SetType(D3DLIGHT_DIRECTIONAL);
	pLightComp->SetDiffuse(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pLightComp->SetDirection(D3DXVECTOR3(-0.18f, 0.88f, -0.44f));

	pLightComp = pD3DLight->AddComponent<CD3DLight>();
	pLightComp->SetType(D3DLIGHT_DIRECTIONAL);
	pLightComp->SetDiffuse(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pLightComp->SetDirection(D3DXVECTOR3(0.89f, 0.11f, 0.44f));

	// ���[���h�ǂݍ���
	m_worldLoader = new WorldLoader();
	m_worldLoader->Load("data\\WORLD\\stage_01.wdit");



	GameObject* pTest = new GameObject();
	pTest->transform->SetScale(0.2f);
	CBoxCollider* pBoxCol = pTest->AddComponent<CBoxCollider>();
	pBoxCol->SetSize({ 10.0f, 10.0f, 10.0f });
	CCollision::GetCollision(pTest)->UpdateCollision();
}

//=============================================================
// [CDebugScene] �I��
//=============================================================
void CDebugScene::Uninit()
{
	if (m_worldLoader != nullptr)
	{
		delete m_worldLoader;
		m_worldLoader = nullptr;
	}
}

//=============================================================
// [CDebugScene] �X�V
//=============================================================
void CDebugScene::Update()
{

}

//=============================================================
// [CDebugScene] �`��
//=============================================================
void CDebugScene::Draw()
{

}