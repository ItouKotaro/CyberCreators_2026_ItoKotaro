//=============================================================
//
// ウイルス [virus.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "virus.h"
#include "benlib.h"
#include "manager.h"
#include "component/3d/mesh.h"
#include "component/3d/collision.h"
#include "scripts/player.h"
#include "scripts/skillpt.h"

// 静的メンバ変数の初期化
const float CVirus::ATTRACT_DISTANCE = 15.0f;
const float CVirus::ATTRACT_POWER = 0.1f;
const float CVirus::MIN_SIZE = 0.5f;
const float CVirus::MAX_SIZE = 3.5f;


//=============================================================
// [CVirus] コンストラクタ
//=============================================================
CVirus::CVirus(const int& nPoint)
{
	m_nPoint = nPoint;
}

//=============================================================
// [CVirus] 初期化
//=============================================================
void CVirus::Init()
{
	// 変数の初期化
	m_move = { 0.0f, 0.0f, 0.0f };

	SetPoint(m_nPoint);
}

//=============================================================
// [CVirus] 更新
//=============================================================
void CVirus::Update()
{
	if (CManager::GetPause())
		return;

	// プレイヤーを取得する
	GameObject* pPlayer = GameObject::Find("Player");

	if (pPlayer != nullptr && pPlayer->GetComponent<CPlayer>()->enabled)
	{ // プレイヤーが見つかったとき
		// 一定の距離以内の場合はプレイヤーの方向へ向かう
		float fDistance = PosPlaneDistance(pPlayer->transform->GetWPos(), transform->GetWPos());
		if (fDistance < ATTRACT_DISTANCE)
		{
			D3DXVECTOR3 vecAngle = PosRotation(transform->GetWPos(), pPlayer->transform->GetWPos() + D3DXVECTOR3(0.0f, 20.0f, 0.0f));
			m_move += vecAngle * ATTRACT_POWER;
		}

		// 移動量の減衰
		m_move -= m_move * 0.08f;

		// 移動量を反映させる
		gameObject->GetParent()->transform->Translate(m_move);
	}
}

//=============================================================
// [CVirus] 当たり判定
//=============================================================
void CVirus::OnTriggerEnter(GameObject* other)
{
	if (other->GetTag() == "Player")
	{ // プレイヤーのとき
		// プレイヤーのスキルポイントを増加させる
		CSkillPoint* pSkillPt = other->GetComponent<CSkillPoint>();

		if (pSkillPt != nullptr)
		{
			pSkillPt->AddPoint(m_nPoint);

			// 破棄する
			gameObject->GetParent()->Destroy(true);
		}
	}
}

//=============================================================
// [CVirus] ポイントの設定
//=============================================================
void CVirus::SetPoint(const int& nPoint)
{
	m_nPoint = nPoint;	// ポイントの変更

	// モデルの大きさを変更する
	float fEveryScale = (MAX_SIZE - MIN_SIZE) * 0.01f;
	transform->SetScale(nPoint * fEveryScale + MIN_SIZE);
}