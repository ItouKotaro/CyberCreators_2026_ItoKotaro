//=============================================================
//
// ��������M [exploding_barrels.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "exploding_barrels.h"
#include "component/3d/mesh.h"
#include "component/3d/collision.h"
#include "benlib.h"

// �ÓI�����o�ϐ��̏�����
const float CExplodingBarrels::EXPLOSION_RADIUS = 80.0f;
const float CExplodingBarrels::EXPLOSION_POWER = 20.0f;

//=============================================================
// [CExplodingBarrels] ������
//=============================================================
void CExplodingBarrels::Init()
{
	gameObject->SetTag("Exploding");
	gameObject->AddComponent<CMesh>("data\\MODEL\\exploding_barrels.x");
	gameObject->AddComponent<CCylinderCollider>(10.0f, 14.0f);
	CRigidBody* pRigid = gameObject->AddComponent<CRigidBody>();
	pRigid->GetCollision()->SetMass(30.0f);

	// �����̓����蔻��
	m_pExplosionCol = new GameObject();
	m_pExplosionCol->SetParent(gameObject);
	m_pExplosionCol->AddComponent<CSphereCollider>(EXPLOSION_RADIUS);
	CCollision::GetCollision(m_pExplosionCol)->IsTrigger(true);
}

//=============================================================
// [CExplodingBarrels] �I��
//=============================================================
void CExplodingBarrels::Uninit()
{
	m_pExplosionCol->Destroy();
}

//=============================================================
// [CExplodingBarrels] �X�V
//=============================================================
void CExplodingBarrels::Update()
{

}

//=============================================================
// [CExplodingBarrels] ����
//=============================================================
void CExplodingBarrels::Explosion()
{
	std::vector<GameObject*>& pOverlapping = CCollision::GetCollision(m_pExplosionCol)->GetOverlappingGameObjects();
	for (int i = 0; i < pOverlapping.size(); i++)
	{
		if (pOverlapping[i] == gameObject)
		{ // �������g�̂Ƃ�
			continue;	// �X�L�b�v
		}

		CCollision* pCollision = CCollision::GetCollision(pOverlapping[i]);
		if (pCollision != nullptr)
		{
			btRigidBody* pRigid = pCollision->GetRigidBody();
			if (pRigid != nullptr)
			{ // ���W�b�h�{�f�B�[�����݂���Ƃ�
				// ��������M����̋������擾����i�З́j
				float fDistance = PosDistance(gameObject->transform->GetWPos(), pOverlapping[i]->transform->GetWPos());
				float fPower = EXPLOSION_RADIUS * 2.0f - fDistance;
				fPower *= EXPLOSION_POWER;

				// ���ł����������v�Z����
				D3DXVECTOR3 explosionDir = PosRotation(gameObject->transform->GetWPos(), pOverlapping[i]->transform->GetWPos());

				// �͂�������
				pRigid->applyCentralImpulse(btVector3(explosionDir.x * fPower, explosionDir.y * fPower, explosionDir.z * fPower));
			}
		}
	}

	// ���g��j������
	gameObject->Destroy();
}