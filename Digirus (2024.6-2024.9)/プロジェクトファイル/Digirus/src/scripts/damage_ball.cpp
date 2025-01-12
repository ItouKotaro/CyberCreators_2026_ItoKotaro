//=============================================================
//
// ダメージボール [damage_ball.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "damage_ball.h"
#include "scripts/player.h"
#include "manager.h"

//=============================================================
// [CDamageBallEmitter] 初期化
//=============================================================
void CDamageBallEmitter::Init()
{
	CDataField* pDataField = CDataField::GetDataField(gameObject);
	m_nLength = pDataField->GetInt("Length");
	m_nInterval = pDataField->GetInt("Interval");
	m_nIntervalCounter = 0;
	m_nBetweenRand = pDataField->GetInt("BetweenRand");
	m_nLife = pDataField->GetInt("Life");
}

//=============================================================
// [CDamageBallEmitter] 更新
//=============================================================
void CDamageBallEmitter::Update()
{
	if (CManager::GetPause())
		return;

	// 生成タイミング
	m_nIntervalCounter--;
	if (m_nIntervalCounter <= 0)
	{ // タイミングが来たとき
		// オブジェクトを生成する
		GameObject* pBall = GameObject::LoadPrefab("data\\PREFAB\\iron_ball.pref");
		pBall->FindNameChildren("Damage")->AddComponent<CDamageBall>();
		pBall->FindNameChildren("Damage")->GetComponent<CDamageBall>()->SetLife(m_nLife);

		// 位置を決める
		D3DXVECTOR3 vecPos = { (float)(rand() % m_nLength - m_nLength / 2), 0.0f, 0.0f };
		D3DXMATRIX mtxRot = transform->GetRotationMatrix();
		D3DXVec3TransformCoord(&vecPos, &vecPos, &mtxRot);
		vecPos += transform->GetWPos();

		// 移動する
		pBall->transform->SetPos(vecPos);

		// 次の時間を決める
		m_nIntervalCounter = m_nInterval + rand() % (m_nBetweenRand * 2) - m_nBetweenRand;
	}
}

//=============================================================
// [CDamageBall] 初期化
//=============================================================
void CDamageBall::Init()
{
	m_nLife = 120;
}

//=============================================================
// [CDamageBall] 更新
//=============================================================
void CDamageBall::Update()
{
	if (CManager::GetPause())
		return;

	// 寿命
	m_nLife--;
	if (m_nLife <= 0)
	{ // 尽きたとき
		gameObject->GetParent()->Destroy(true);		// 破棄する
	}
}

//=============================================================
// [CDamageBall] プレイヤーにあたったとき
//=============================================================
void CDamageBall::OnTriggerEnter(GameObject* other)
{
	if (other->GetName() == "Player" &&
		other->GetComponent<CPlayer>() != nullptr)
	{ // プレイヤーに触れたとき
		other->GetComponent<CPlayer>()->DamagePlayer(20);	// ダメージを与える
		gameObject->GetParent()->Destroy(true);		// 破棄する
	}
}