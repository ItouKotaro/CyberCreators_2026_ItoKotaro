//=============================================================
//
// 爆発する樽 [exploding_barrels.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "exploding_barrels.h"
#include "component/3d/mesh.h"
#include "component/3d/collision.h"
#include "benlib.h"

// 静的メンバ変数の初期化
const float CExplodingBarrels::EXPLOSION_RADIUS = 80.0f;
const float CExplodingBarrels::EXPLOSION_POWER = 20.0f;

//=============================================================
// [CExplodingBarrels] 初期化
//=============================================================
void CExplodingBarrels::Init()
{
	gameObject->SetTag("Exploding");
	gameObject->AddComponent<CMesh>("data\\MODEL\\exploding_barrels.x");
	gameObject->AddComponent<CCylinderCollider>(10.0f, 14.0f);
	CRigidBody* pRigid = gameObject->AddComponent<CRigidBody>();
	pRigid->GetCollision()->SetMass(30.0f);

	// 爆発の当たり判定
	m_pExplosionCol = new GameObject();
	m_pExplosionCol->SetParent(gameObject);
	m_pExplosionCol->AddComponent<CSphereCollider>(EXPLOSION_RADIUS);
	CCollision::GetCollision(m_pExplosionCol)->IsTrigger(true);
}

//=============================================================
// [CExplodingBarrels] 終了
//=============================================================
void CExplodingBarrels::Uninit()
{
	m_pExplosionCol->Destroy();
}

//=============================================================
// [CExplodingBarrels] 更新
//=============================================================
void CExplodingBarrels::Update()
{

}

//=============================================================
// [CExplodingBarrels] 爆発
//=============================================================
void CExplodingBarrels::Explosion()
{
	std::vector<GameObject*>& pOverlapping = CCollision::GetCollision(m_pExplosionCol)->GetOverlappingGameObjects();
	for (int i = 0; i < pOverlapping.size(); i++)
	{
		if (pOverlapping[i] == gameObject)
		{ // 自分自身のとき
			continue;	// スキップ
		}

		CCollision* pCollision = CCollision::GetCollision(pOverlapping[i]);
		if (pCollision != nullptr)
		{
			btRigidBody* pRigid = pCollision->GetRigidBody();
			if (pRigid != nullptr)
			{ // リジッドボディーが存在するとき
				// 爆発する樽からの距離を取得する（威力）
				float fDistance = PosDistance(gameObject->transform->GetWPos(), pOverlapping[i]->transform->GetWPos());
				float fPower = EXPLOSION_RADIUS * 2.0f - fDistance;
				fPower *= EXPLOSION_POWER;

				// 飛んでいく方向を計算する
				D3DXVECTOR3 explosionDir = PosRotation(gameObject->transform->GetWPos(), pOverlapping[i]->transform->GetWPos());

				// 力を加える
				pRigid->applyCentralImpulse(btVector3(explosionDir.x * fPower, explosionDir.y * fPower, explosionDir.z * fPower));
			}
		}
	}

	// 自身を破棄する
	gameObject->Destroy();
}