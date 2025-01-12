//=============================================================
//
// 敵 [enemy.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "enemy.h"
#include "benlib.h"
#include "manager.h"
#include "component/3d/collision.h"
#include "component/3d/billboard.h"
#include "scripts/player.h"
#include "scripts/break_effect.h"
#include "component/3d/particle.h"
#include "component/3d/motion.h"
#include "component/other/sound.h"

// 静的メンバ変数の初期化
const float CEnemy::KNOCKBACK_POWER = 70.0f;
const float CEnemy::ROTATION_SPEED = 0.08f;

//=============================================================
// [CEnemy] 初期化
//=============================================================
void CEnemy::Init()
{
	m_nLife = 10;
	m_fSpeed = 1.0f;
	m_nAttack = 5;
	m_fAttackRange = 2.0f;
	m_nFoundDuration = 120;
	m_nDamagedMoveDuration = 60;
	m_nAttackDuration = 120;
	m_fObjectiveAngle = transform->GetWRotY();
	m_fCurrentAngle = transform->GetWRotY();

	m_nStateCounter = 0;
	m_state = STATE::SEARCH;

	// 状態
	m_pStateUI = new GameObject();
	m_pStateUI->SetParent(gameObject);
	m_pStateUI->transform->Translate(0.0f, 20.0f, 0.0f);
	m_pStateUI->AddComponent<CBillboard>();
	m_pStateUI->GetComponent<CBillboard>()->Set(20.0f, 20.0f);

	m_pSearchRange = gameObject->FindNameChildren("Search");
}

//=============================================================
// [CEnemy] 終了
//=============================================================
void CEnemy::Uninit()
{
	// 状態UIの破棄
	m_pStateUI->Destroy();

	// 察知範囲のオブジェクトを破棄
	m_pSearchRange->Destroy();

	// このオブジェクトも破棄する
	gameObject->Destroy(true);
}

//=============================================================
// [CEnemy] 更新
//=============================================================
void CEnemy::Update()
{
	if (CManager::GetPause())
		return;

	// 状態ごとの動作
	switch (m_state)
	{
	case STATE::SEARCH:
		SearchAction();
		break;
	case STATE::FOUND:
		FoundAction();
		break;
	case STATE::ATTACK:
		AttackAction();
		break;
	case STATE::DAMAGE_MOVE:
		DamagedAction();
		break;
	}

	m_pStateUI->SetVisible(m_state != STATE::SEARCH || m_nStateCounter > 0);

	// 状態管理
	StateManager();

	// 回転処理
	UpdateAngle();
}

//=============================================================
// [CEnemy] ダメージを受けたときの処理
//=============================================================
void CEnemy::OnDamage()
{
	// ノックバック
	btRigidBody* pRigidBody = CCollision::GetCollision(gameObject)->GetRigidBody();
	pRigidBody->activate(true);
	if (pRigidBody != nullptr)
	{ // リジッドボディが存在するとき
		btVector3 power = btVector3(sinf(m_damagedAngle + D3DX_PI) * KNOCKBACK_POWER, 0.0f, cosf(m_damagedAngle + D3DX_PI) * KNOCKBACK_POWER);

		// 力を加える
		pRigidBody->setLinearVelocity(power);
	}

	if (m_state == FOUND || m_state == ATTACK)
	{ // 捜索以外のとき
		return; //スキップ
	}

	// ダメージを受けた方向に向く
	m_fObjectiveAngle = m_damagedAngle + D3DX_PI;

	// 攻撃の受けた方向に向かう
	m_state = DAMAGE_MOVE;
	m_nStateCounter = m_nDamagedMoveDuration;

	// 見失ってる状態にする
	OnLostPlayer();
}

//=============================================================
// [CEnemy] 死亡時の処理
//=============================================================
void CEnemy::OnDeath()
{
	// 敵の大きさ
	int nHeight = 20.0f;
	int nRange = 10.0f;

	// 死亡エフェクトの表示
	for (int i = 0; i < 12; i++)
	{
		GameObject* pBreakEffect = new GameObject();

		// 位置決定
		pBreakEffect->transform->SetPos(transform->GetWPos());
		pBreakEffect->transform->Translate(rand() % (nRange * 2) - nRange, rand() % nHeight, rand() % (nRange * 2) - nRange);
		pBreakEffect->transform->Rotate(rand() % 314 * 0.01f, rand() % 314 * 0.01f, rand() % 314 * 0.01f);
		pBreakEffect->AddComponent<CBreakEffect>(rand() % 120 + 260, (rand() % 20 + 10)  * 0.03f);
	}

	// オブジェクトを破棄する
	gameObject->Destroy();
}

//=============================================================
// [CEnemy] プレイヤーを発見したときの処理
//=============================================================
void CEnemy::OnFoundPlayer()
{
	// ビルボードを表示する
	CBillboard* pBillboard = m_pStateUI->GetComponent<CBillboard>();
	pBillboard->SetTexture("data\\TEXTURE\\ENEMY_STATE\\found.png");
}

//=============================================================
// [CEnemy] プレイヤーを見失ったときの処理
//=============================================================
void CEnemy::OnLostPlayer()
{
	// パーティクルを表示する
	CBillboard* pBillboard = m_pStateUI->GetComponent<CBillboard>();
	pBillboard->SetTexture("data\\TEXTURE\\ENEMY_STATE\\not_found.png");
}

//=============================================================
// [CEnemy] プレイヤーを引き付ける
//=============================================================
void CEnemy::AttractPlayer()
{
	// リジッドボディを取得する
	btRigidBody* pRigidBody = CCollision::GetCollision(gameObject)->GetRigidBody();

	// プレイヤーを取得する
	GameObject* pPlayer = GameObject::Find("Player");

	if (pRigidBody != nullptr && pPlayer != nullptr)
	{ // リジッドボディが存在するとき
		// 向かうべき角度を取得する
		float fAngle = PosAngle(transform->GetWPos(), pPlayer->transform->GetWPos());
		btVector3 power = btVector3(sinf(fAngle) * m_fSpeed, 0.0f, cosf(fAngle) * m_fSpeed);

		// 向きを変更する
		m_fObjectiveAngle = fAngle + D3DX_PI;

		// 力を加える
		pRigidBody->translate(power);
	}
}

//=============================================================
// [CEnemy] ダメージを与える
//=============================================================
void CEnemy::DamageEnemy(const int& nDamage, const	 D3DXVECTOR3& damagePos)
{
	// 体力からダメージを減算
	m_nLife -= nDamage;
	m_damagedAngle = PosAngle(transform->GetWPos(), damagePos);

	OnDamage();

	// 死亡判定
	if (m_nLife <= 0)
	{ // 体力が無くなったとき
		// 死亡時の処理
		OnDeath();
	}
}

//=============================================================
// [CEnemy] 体力の設定
//=============================================================
void CEnemy::SetLife(const int& nLife)
{
	m_nLife = nLife;
}

//=============================================================
// [CEnemy] 体力の取得
//=============================================================
int CEnemy::GetLife()
{
	return m_nLife;
}

//=============================================================
// [CEnemy] 速さの設定
//=============================================================
void CEnemy::SetSpeed(const float& fSpeed)
{
	m_fSpeed = fSpeed;
}

//=============================================================
// [CEnemy] 速さの取得
//=============================================================
float CEnemy::GetSpeed()
{
	return m_fSpeed;
}

//=============================================================
// [CEnemy] 攻撃力の設定
//=============================================================
void CEnemy::SetAttack(const int& nAttack)
{
	m_nAttack = nAttack;
}

//=============================================================
// [CEnemy] 攻撃力の取得
//=============================================================
int CEnemy::GetAttack()
{
	return m_nAttack;
}

//=============================================================
// [CEnemy] 攻撃範囲の設定
//=============================================================
void CEnemy::SetAttackRange(const float& fRange)
{
	m_fAttackRange = fRange;
}

//=============================================================
// [CEnemy] 発見の継続時間
//=============================================================
void CEnemy::SetFoundDuration(const int& nDuration)
{
	m_nFoundDuration = nDuration;
}

//=============================================================
// [CEnemy] 攻撃の継続時間
//=============================================================
void CEnemy::SetAttackDuration(const int& nDuration)
{

}

//=============================================================
// [CEnemy] ダメージを受けたときの移動時間
//=============================================================
void CEnemy::SetDamagedMoveDuration(const int& nDuration)
{
	m_nDamagedMoveDuration = nDuration;
}

//=============================================================
// [CEnemy] 察知範囲オブジェクトの取得
//=============================================================
GameObject* CEnemy::GetSearchRange()
{
	return m_pSearchRange;
}

//=============================================================
// [CEnemy] 捜索動作
//=============================================================
void CEnemy::SearchAction()
{
	if (!CCollision::isCollisionExist(m_pSearchRange))
	{ // 当たり判定が存在しないとき
		return;
	}

	// 捜索範囲内にプレイヤーがいないかを確認する
	std::vector<GameObject*>& pOverlappingObjs = CCollision::GetCollision(m_pSearchRange)->GetOverlappingGameObjects();
	for (int i = 0; i < pOverlappingObjs.size(); i++)
	{
		if (pOverlappingObjs[i]->GetTag() == "Player")
		{ // プレイヤーが範囲内に入っているとき
			m_state = STATE::FOUND;						// 発見状態に切り替える
			m_nStateCounter = m_nFoundDuration;	// 発見状態の継続時間を設定する
			OnFoundPlayer();
			return;
		}
	}
}

//=============================================================
// [CEnemy] 発見動作
//=============================================================
void CEnemy::FoundAction()
{
	// 攻撃範囲に入っているか
	GameObject* pPlayer = GameObject::Find("Player");	// プレイヤーの取得
	if (pPlayer != nullptr)
	{
		if (PosDistance(transform->GetWPos(), pPlayer->transform->GetWPos()) <= m_fAttackRange)
		{ // 攻撃範囲内のとき
			m_nStateCounter = m_nAttackDuration;	// 攻撃時間の設定
			AttackDecision();									// 攻撃決定
			m_state = STATE::ATTACK;					// 攻撃に移行
			return;
		}
	}

	// 再度、捜索範囲内にプレイヤーが入ったとき
	std::vector<GameObject*>& pOverlappingObjs = CCollision::GetCollision(m_pSearchRange)->GetOverlappingGameObjects();
	for (int i = 0; i < pOverlappingObjs.size(); i++)
	{
		if (pOverlappingObjs[i]->GetTag() == "Player")
		{ // プレイヤーが範囲内に入っているとき
			m_nStateCounter = m_nFoundDuration;	// 継続時間の再設定を行う
			return;
		}
	}

	// プレイヤーを見失うとき
	if (m_nStateCounter == 1)
	{
		OnLostPlayer();	// 見失いイベント

		// 捜索状態に以降
		m_nStateCounter = 120;
		m_state = STATE::SEARCH;
	}
}

//=============================================================
// [CEnemy] 攻撃動作
//=============================================================
void CEnemy::AttackAction()
{
	// 攻撃タイミング
	GameObject* pPlayer = GameObject::Find("Player");	// プレイヤーの取得
	if (PosDistance(transform->GetWPos(), pPlayer->transform->GetWPos()) <= m_fAttackRange)
	{ // 攻撃範囲内のとき
		pPlayer->GetComponent<CPlayer>()->DamagePlayer(m_nAttack);		// ダメージを与える
	}
}

//=============================================================
// [CEnemy] ダメージを受けたときの動作
//=============================================================
void CEnemy::DamagedAction()
{
	// リジッドボディを取得する
	btRigidBody* pRigidBody = CCollision::GetCollision(gameObject)->GetRigidBody();
	if (pRigidBody != nullptr)
	{
		// 向かうべき角度を取得する
		btVector3 power = btVector3(sinf(m_damagedAngle) * m_fSpeed, 0.0f, cosf(m_damagedAngle) * m_fSpeed);

		// 力を加える
		pRigidBody->translate(power);
	}

	// 捜索も行う
	SearchAction();
}

//=============================================================
// [CEnemy] 攻撃決定（攻撃手段の決定）
//=============================================================
void CEnemy::AttackDecision()
{

}

//=============================================================
// [CEnemy] 状態管理
//=============================================================
void CEnemy::StateManager()
{
	// カウンターのデクリメント
	m_nStateCounter--;

	// カウンターが0になったとき
	if (m_nStateCounter <= 0)
	{
		// 攻撃状態から捜索に移行するとき
		if (m_state == STATE::ATTACK)
		{
			OnLostPlayer();
			m_nStateCounter = 120;
		}

		m_state = STATE::SEARCH;	// 捜索に戻す
	}
}

//=============================================================
// [CEnemy] 向きの更新
//=============================================================
void CEnemy::UpdateAngle()
{
	// 回転の制御
	if (m_fCurrentAngle > D3DX_PI)
	{
		m_fCurrentAngle -= D3DX_PI * 2;
	}
	else if (m_fCurrentAngle < -D3DX_PI)
	{
		m_fCurrentAngle += D3DX_PI * 2;
	}

	if (m_fObjectiveAngle > D3DX_PI)
	{
		m_fObjectiveAngle -= D3DX_PI * 2;
	}
	else if (m_fObjectiveAngle < -D3DX_PI)
	{
		m_fObjectiveAngle += D3DX_PI * 2;
	}

	// 自然な回転処理
	if (m_fCurrentAngle < m_fObjectiveAngle)
	{
		if (m_fCurrentAngle + D3DX_PI > m_fObjectiveAngle)
		{
			m_fCurrentAngle += fabsf(m_fObjectiveAngle - m_fCurrentAngle) * ROTATION_SPEED;
		}
		else
		{
			m_fCurrentAngle -= fabsf((m_fObjectiveAngle - D3DX_PI * 2) - m_fCurrentAngle) * ROTATION_SPEED;
		}
	}
	else if (m_fCurrentAngle > m_fObjectiveAngle)
	{
		if (m_fCurrentAngle - D3DX_PI < m_fObjectiveAngle)
		{
			m_fCurrentAngle -= fabsf(m_fObjectiveAngle - m_fCurrentAngle) * ROTATION_SPEED;
		}
		else
		{
			m_fCurrentAngle += fabsf((m_fObjectiveAngle + D3DX_PI * 2) - m_fCurrentAngle) * ROTATION_SPEED;
		}
	}

	// 角度を変更する
	transform->SetRot(0.0f, m_fCurrentAngle, 0.0f);
}



//=============================================================
// [CDebugEnemy] 初期化
//=============================================================
void CDebugEnemy::Init()
{
	CEnemy::Init();
	SetAttack(30);
	SetLife(10);
	SetFoundDuration(300);
	SetAttackRange(20.0f);
	SetSpeed(0.4f);
	SetDamagedMoveDuration(120);
}

//=============================================================
// [CDebugEnemy] 発見動作
//=============================================================
void CDebugEnemy::FoundAction()
{
	CEnemy::FoundAction();

	// プレイヤーの方向に向かう
	CEnemy::AttractPlayer();
}

//=============================================================
// [CDebugEnemy] 攻撃動作
//=============================================================
void CDebugEnemy::AttackAction()
{
	if (GetStateCounter() == 60)
	{ // ダメージタイミング
		CEnemy::AttackAction();
	}
}

//=============================================================
// [CSlimeEnemy] 初期化
//=============================================================
void CSlimeEnemy::Init()
{
	CEnemy::Init();
	SetLife(3);
	SetFoundDuration(300);
	SetAttackRange(15.0f);
	SetSpeed(0.6f);
	SetDamagedMoveDuration(120);
	SetAttack(10);
	SetAttackDuration(30);
}

//=============================================================
// [CSlimeEnemy] 発見動作
//=============================================================
void CSlimeEnemy::FoundAction()
{
	CEnemy::FoundAction();

	// リジッドボディを取得
	auto pRigidbody = CCollision::GetCollision(gameObject)->GetRigidBody();

	if (fabsf(pRigidbody->getLinearVelocity().getY()) > 0.3f)
	{
		// 移動
		CEnemy::AttractPlayer();
	}

	// ジャンプカウンター
	m_nJumpCounter--;
	if (m_nJumpCounter <= 0)
	{
		// ジャンプする
		btVector3 vecJump = pRigidbody->getLinearVelocity() + btVector3(0.0f, 60.0f, 0.0f);
		pRigidbody->setLinearVelocity(vecJump);

		// モーション
		gameObject->GetComponent<CMotionManager>()->Play("jump");

		// 音
		GameObject* pSound = new GameObject;
		pSound->SetParent(gameObject);
		pSound->AddComponent<CSound>();
		pSound->GetComponent<CSound>()->LoadWav("data\\SOUND\\SE\\slime.wav");
		pSound->GetComponent<CSound>()->SetVolume(18);
		pSound->GetComponent<CSound>()->Play();
		pSound->GetComponent<CSound>()->IsStoppedDestroy();

		// 次のジャンプ
		if (m_nJumpCounter <= 0)
		{
			m_nJumpCounter = 120 + rand() % 80;
		}
	}
}

//=============================================================
// [CSlimeEnemy] 攻撃動作
//=============================================================
void CSlimeEnemy::AttackAction()
{
	if (GetStateCounter() == 10)
	{ // ダメージタイミング
		CEnemy::AttackAction();
	}
}