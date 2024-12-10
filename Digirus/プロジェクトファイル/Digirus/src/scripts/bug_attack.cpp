//=============================================================
//
// バグアタック [bug_attack.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "bug_attack.h"
#include "component/3d/collision.h"
#include "scripts/enemy.h"
#include "manager.h"
#include "benlib.h"

// 静的メンバ変数の初期化
const float CBugAttack::MOVE_SPEED = 50.0f;
const int CBugAttack::ATTACK_DAMAGE = 4;

//=============================================================
// [CBugAttack] 初期化
//=============================================================
void CBugAttack::Init()
{
	CCollision* pCollision = CCollision::GetCollision(gameObject);
	if (pCollision == nullptr ||
		pCollision->GetRigidBody() == nullptr)
	{ // リジッドボディが見つからないとき
		return;
	}

	gameObject->GetComponent<CRigidBody>()->SetAlwayActive(true);
}

//=============================================================
// [CBugAttack] 更新
//=============================================================
void CBugAttack::Update()
{
	if (CManager::GetPause())
		return;

	CCollision* pCollision = CCollision::GetCollision(gameObject);
	if (pCollision == nullptr ||
		pCollision->GetRigidBody() == nullptr)
	{ // リジッドボディが見つからないとき
		return;
	}

	// ジャンプ
	UpdateJump();

	// 倒れないようにする
	transform->SetRot(0.0f, transform->GetRotY(), 0.0f);

	// 移動量の方向に進む
	pCollision->GetRigidBody()->setLinearVelocity(btVector3(
		sinf(transform->GetWRotY() + D3DX_PI * 0.5f) * MOVE_SPEED,
		pCollision->GetRigidBody()->getLinearVelocity().getY(),
		cosf(transform->GetWRotY() + D3DX_PI * 0.5f) * MOVE_SPEED));

	// 敵を取得する
	std::vector<CEnemy*> pEnemyList = Component::GetComponents<CEnemy>(true);
	for (int i = 0; i < pEnemyList.size(); i++)
	{		
		if (PosDistance(pEnemyList[i]->transform->GetWPos(), transform->GetWPos()) < 30.0f)
		{ // 敵が近くにいるとき
			float fEnemyAngle = PosAngle(transform->GetWPos(), pEnemyList[i]->transform->GetWPos());
			transform->SetRot(0.0f, fEnemyAngle - D3DX_PI * 0.5f, 0.0f);
			//transform->Rotate(0.0f, (fEnemyAngle - D3DX_PI * 0.5f - transform->GetWRot().y) * 0.3f, 0.0f);
			break;
		}
	}

	// 衝突判定
	GameObject* pDetectionObj = gameObject->FindNameChildren("Detection");
	if (pDetectionObj != nullptr &&
		CCollision::GetCollision(pDetectionObj) != nullptr)
	{
		std::vector<GameObject*>& pDetectionOverlapping = CCollision::GetCollision(pDetectionObj)->GetOverlappingGameObjects();
		for (int i = 0; i < pDetectionOverlapping.size(); i++)
		{
			if (pDetectionOverlapping[i]->GetTag() == "Enemy")
			{ // 敵と衝突したとき
				pDetectionOverlapping[i]->GetComponent<CEnemy>(true)->DamageEnemy(ATTACK_DAMAGE, transform->GetWPos());	// 敵にダメージを与える
				gameObject->Destroy(true);		// 自らを破棄する
			}
		}
	}

	// 寿命
	m_nLife--;
	if (m_nLife <= 0)
	{
		gameObject->Destroy(true);
	}
}

//=============================================================
// [CBugAttack] ジャンプ
//=============================================================
void CBugAttack::UpdateJump()
{
	// レイを飛ばして地面があるか判定する
	btVector3 btRayFrom = btVector3(transform->GetWPos().x, transform->GetWPos().y, transform->GetWPos().z);
	btVector3 btRayTo = btVector3(transform->GetWPos().x, transform->GetWPos().y - 5.0f, transform->GetWPos().z);
	btCollisionWorld::ClosestRayResultCallback rayCallback(btRayFrom, btRayTo);
	CManager::GetPhysics()->GetDynamicsWorld().rayTest(btRayFrom, btRayTo, rayCallback);
	if (rayCallback.hasHit())
	{ // 地面についているとき
		btRayTo = btVector3(transform->GetWPos().x, transform->GetWPos().y, transform->GetWPos().z + 15.0f);
		btCollisionWorld::ClosestRayResultCallback rayJumpCallback(btRayFrom, btRayTo);
		CManager::GetPhysics()->GetDynamicsWorld().rayTest(btRayFrom, btRayTo, rayJumpCallback);
		// ジャンプ
		if (rayJumpCallback.hasHit())
		{
			CCollision::GetCollision(gameObject)->GetRigidBody()->setLinearVelocity(
				CCollision::GetCollision(gameObject)->GetRigidBody()->getLinearVelocity() + btVector3(0.0f, 5.0f, 0.0f)
			);
		}
	}
}