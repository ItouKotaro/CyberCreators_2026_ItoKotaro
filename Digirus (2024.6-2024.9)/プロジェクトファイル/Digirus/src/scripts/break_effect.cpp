//=============================================================
//
// �j��G�t�F�N�g [break_effect.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "break_effect.h"
#include "component/3d/mesh.h"
#include "component/3d/collision.h"
#include "scripts/virus.h"
#include "scripts/skillpt.h"
#include "scripts/player.h"
#include "manager.h"

// �ÓI�����o�ϐ��̏�����
const std::string CBreakEffect::MESH_PATH = "data\\MODEL\\box.x";

//=============================================================
// [CBreakEffect] �R���X�g���N�^
//=============================================================
CBreakEffect::CBreakEffect(const int& nLife, const float& fSize)
{
	m_nLife = nLife;
	m_fScale = fSize;
}

//=============================================================
// [CBreakEffect] ������
//=============================================================
void CBreakEffect::Init()
{
	m_pPickupObj = new GameObject();

	m_pPickupObj->AddComponent<CBoxCollider>(D3DXVECTOR3(2.0f, 2.0f, 2.0f) * (m_fScale + 0.2f));
	CCollision::GetCollision(m_pPickupObj)->IsTrigger(true);
	m_pPickupObj->AddComponent<CVirus>(3);
	m_pPickupObj->SetParent(gameObject);

	// �Q�[���I�u�W�F�N�g�̐ݒ�
	gameObject->SetTag("Virus");
	gameObject->transform->SetScale(m_fScale);

	// ���b�V��
	CMesh* pMesh = gameObject->AddComponent<CMesh>(MESH_PATH);

	// �R���W����
	gameObject->AddComponent<CBoxCollider>(D3DXVECTOR3(2.0f, 2.0f, 2.0f) * m_fScale);
	CRigidBody* pRigid = gameObject->AddComponent<CRigidBody>();
	pRigid->SetAlwayActive(true);
	pRigid->GetCollision()->SetMass(15.0f);

	// ��΂�
	D3DXVECTOR3 impulseDir = D3DXVECTOR3(static_cast<float>(rand() % 20 - 10), static_cast<float>(rand() % 20 - 10), static_cast<float>(rand() % 20 - 10));
	D3DXVec3Normalize(&impulseDir, &impulseDir);
	impulseDir *= static_cast<float>(rand() % 40);
	pRigid->GetRigidBody()->applyCentralImpulse(btVector3(impulseDir.x, impulseDir.y, impulseDir.z));
}

//=============================================================
// [CBreakEffect] �X�V
//=============================================================
void CBreakEffect::Update()
{
	if (CManager::GetPause())
		return;

	// ����
	m_nLife--;
	if (m_nLife <= 0)
	{ // �������s�����Ƃ�
		gameObject->Destroy(true);
	}
}