//=============================================================
//
// レーザーブロック [laser_block.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "laser_block.h"
#include "component/3d/collision.h"
#include "component/3d/mesh.h"
#include "scripts/player.h"

//=============================================================
// [CLaserBlock] 初期化
//=============================================================
void CLaserBlock::Init()
{
	// 変数の初期化
	m_nDamage = 1;
	m_move = { 0.0f, 0.0f, 0.0f };
	m_nMoveCount = 0;
	m_nMoveCounter = 0;

	// 当たり判定を作成する
	m_pLaserCollision = new GameObject;
	m_pLaserCollision->SetParent(gameObject);
	m_pLaserCollision->AddComponent<CBoxCollider>();
	CCollision::GetCollision(m_pLaserCollision)->IsTrigger(true);

	// バーを半透明にする
	gameObject->FindNameChildren("bar")->GetComponent<CMesh>()->SetAlpha(0.1f);
}

//=============================================================
// [CLaserBlock] 更新
//=============================================================
void CLaserBlock::Update()
{
	// バーを回転させる
	gameObject->FindNameChildren("bar")->transform->Rotate(0.0f, 0.0f, 0.1f);

	// プレイヤーにあたったときのダメージ
	std::vector<GameObject*> pOverlapping = CCollision::GetCollision(m_pLaserCollision)->GetOverlappingGameObjects();
	for (int i = 0; i < pOverlapping.size(); i++)
	{
		if (pOverlapping[i]->GetName() == "Player")
		{ // プレイヤーのとき
			CPlayer* pPlayer = pOverlapping[i]->GetComponent<CPlayer>();
			if (pPlayer != nullptr)
			{
				pPlayer->DamagePlayer(m_nDamage);	// ダメージを与える
			}
		}
	}

	// 移動
	transform->Translate(m_move);

	// カウンターでの反転
	if (m_nMoveCount != 0)
	{ // 0以外のとき
		m_nMoveCounter++;
		if (m_nMoveCounter >= m_nMoveCount)
		{ // カウントが超えたとき
			m_move *= -1;				// 移動量反転
			m_nMoveCounter = 0;	// カウンターリセット
		}
	}
}

//=============================================================
// [CLaserBlock] 初期化
//=============================================================
void CLaserBlock::SetLength(const float& fLength)
{
	m_fLength = fLength;

	// 当たり判定を更新する
	m_pLaserCollision->GetComponent<CBoxCollider>()->SetSize({ 1.0f, 1.0f, fLength * 0.5f });
	m_pLaserCollision->GetComponent<CBoxCollider>()->SetOffset({0.0f, 0.0f, -fLength * 0.5f});
	CCollision::GetCollision(m_pLaserCollision)->UpdateCollision();

	// バーを伸ばす
	gameObject->FindNameChildren("bar")->transform->SetScale(0.8f, 0.8f, m_fLength);
}