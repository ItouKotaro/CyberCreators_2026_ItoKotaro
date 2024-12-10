//=============================================================
//
// 弾 [bullet.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "bullet.h"
#include "component/3d/mesh.h"
#include "component/3d/collision.h"
#include "scripts/enemy.h"
#include "scripts/exploding_barrels.h"

// 静的メンバ変数の初期化
const std::string CBullet::BULLET_MESH = "data\\MODEL\\test_bullet.x";
const float CBullet::SPEED = 1.5f;

//=============================================================
// [CBullet] 初期化
//=============================================================
void CBullet::Init()
{
	// 弾のモデル
	gameObject->AddComponent<CMesh>(BULLET_MESH);
	gameObject->AddComponent<CSphereCollider>(3.0f);
	gameObject->transform->SetScale(0.6f);
	CCollision::GetCollision(gameObject)->IsTrigger(true);
}

//=============================================================
// [CBullet] 更新
//=============================================================
void CBullet::Update()
{
	// 動かす
	transform->Translate(m_move);

	// 寿命
	m_nLife--;
	if (m_nLife <= 0)
	{ // 寿命がなくなったとき
		gameObject->Destroy();
	}
}

//=============================================================
// [CBullet] 当たり判定に入ったとき
//=============================================================
void CBullet::OnTriggerEnter(GameObject* other)
{
	if (other->GetTag() == "Enemy")
	{ // 当たったオブジェクトが敵のとき
		// 敵コンポーネントを取得する
		CEnemy* pEnemy = other->GetComponent<CEnemy>(true);

		if (pEnemy != nullptr)
		{ // 敵コンポーネントが存在するとき
			pEnemy->DamageEnemy(1, transform->GetWPos());		// 敵にダメージを与える
		}
	}
	else if (other->GetTag() == "Exploding")
	{
		// 爆発する樽のコンポーネントを取得する
		CExplodingBarrels* pExploding = other->GetComponent<CExplodingBarrels>();

		if (pExploding != nullptr)
		{ // 爆発する樽のコンポーネントが存在するとき
			pExploding->Explosion();			// 爆発を引き起こす
		}
	}

	// 破棄する
	if (other->GetTag() != "Player")
	{
		gameObject->Destroy();
	}
}

//=============================================================
// [CBullet] 寿命の設定
//=============================================================
void CBullet::SetLife(const int& nLife)
{
	m_nLife = nLife;
}

//=============================================================
// [CBullet] 攻撃力の設定
//=============================================================
void CBullet::SetAttack(const int& nAttack)
{
	m_nAttack = nAttack;
}

//=============================================================
// [CBullet] 移動量の設定
//=============================================================
void CBullet::SetMove(const D3DXVECTOR3& move)
{
	m_move = move;
}