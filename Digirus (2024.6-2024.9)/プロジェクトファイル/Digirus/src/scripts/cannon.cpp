//=============================================================
//
// キャノン砲 [cannon.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "cannon.h"
#include "benlib.h"
#include "component/other/sound.h"
#include "scripts/player.h"
#include "manager.h"

// 静的メンバ変数の初期化
const int CCannonBullet::DAMAGE = 30;

//=============================================================
// [CCannon] 初期化
//=============================================================
void CCannon::Init()
{
	// 設定する
	m_fSpeed = CDataField::GetDataField(gameObject)->GetFloat("Speed");
	m_nInterval = CDataField::GetDataField(gameObject)->GetInt("Interval");
	m_nIntervalCounter = m_nInterval;
	m_nLife = CDataField::GetDataField(gameObject)->GetInt("LifeTime");
}

//=============================================================
// [CCannon] 更新
//=============================================================
void CCannon::Update()
{
	if (CManager::GetPause())
	{
		return;
	}

	// 射出頻度
	m_nIntervalCounter--;
	if (m_nIntervalCounter <= 0)
	{ // 射出タイミング
		// 砲弾を生成する
		Transform trans(transform->GetWPos());
		GameObject* pBullet = GameObject::LoadPrefab("data\\PREFAB\\bullet.pref", trans);

		// 向かう方向を計算する
		D3DXVECTOR3 direction = { 0.0f, 0.0f, -1.0f };
		D3DXMATRIX mtxRot = transform->GetRotationMatrix();
		D3DXVec3TransformCoord(&direction, &direction, &mtxRot);

		// 設定する
		pBullet->AddComponent<CCannonBullet>();
		pBullet->GetComponent<CCannonBullet>()->SetLife(m_nLife);
		pBullet->GetComponent<CCannonBullet>()->SetMove(direction * m_fSpeed);

		// 音を鳴らす
		GameObject* pPopSound = new GameObject;
		pPopSound->transform->SetPos(transform->GetWPos());
		pPopSound->AddComponent<CSound>();
		pPopSound->GetComponent<CSound>()->LoadWav("data\\SOUND\\SE\\pop.wav");
		pPopSound->GetComponent<CSound>()->SetVolume(20);
		pPopSound->GetComponent<CSound>()->Play();
		pPopSound->GetComponent<CSound>()->IsStoppedDestroy();

		// 時間をリセット
		m_nIntervalCounter = m_nInterval;
	}
}

//=============================================================
// [CCannonBullet] 初期化
//=============================================================
void CCannonBullet::Init()
{
	m_nLife = 120;
	m_move = {0.0f, 0.0f, 0.0f};
}

//=============================================================
// [CCannonBullet] 更新
//=============================================================
void CCannonBullet::Update()
{
	if (CManager::GetPause())
	{
		return;
	}

	// 移動
	transform->Translate(m_move);

	// 寿命
	m_nLife--;
	if (m_nLife <= 0)
	{ // 寿命がなくなったとき
		gameObject->Destroy();		// 自身を破棄する
	}
}

//=============================================================
// [CCannonBullet] プレイヤーに触れたときの処理
//=============================================================
void CCannonBullet::OnTriggerEnter(GameObject* other)
{
	if (other->GetName() == "Player" &&
		other->GetComponent<CPlayer>() != nullptr)
	{
		// プレイヤーにダメージを与える
		other->GetComponent<CPlayer>()->DamagePlayer(DAMAGE);
	}

	// オブジェクトを破棄する
	gameObject->Destroy(true);
}