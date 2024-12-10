//=============================================================
//
// プレイヤー [player.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "player.h"
#include "component/3d/collision.h"
#include "component/3d/motion.h"
#include "component/3d/particle.h"
#include "component/other/sound.h"
#include "component/2d/text.h"
#include "manager.h"
#include "scripts/enemy.h"
#include "scripts/bullet.h"
#include "scripts/skillpt.h"
#include "scripts/skill.h"
#include "scripts/bar.h"
#include "scripts/bug_attack.h"
#include "scripts/camera_move.h"
#include "scripts/trajectory.h"
#include "input_system.h"
#include "benlib.h"
#include "scene.h"
#include "scene/game.h"

// 静的メンバの初期化
const float CPlayer::MOVE_SPEED = 50.0f;
const float CPlayer::DASH_RATE = 1.5f;
const int CPlayer::REVIVAL = 3;
const int CPlayer::DEF_LIFE = 120;
const int CPlayer::DEF_STAMINA = 240;
const float CPlayer::ROTATION_SPEED = 0.4f;

const int CPlayer::DASH_STAMINA = 2;
const int CPlayer::ROLLING_STAMINA = 100;

//=============================================================
// [CPlayer] 初期化
//=============================================================
void CPlayer::Init()
{
	// 変数の初期化
	m_nLife = DEF_LIFE;
	m_nStamina = DEF_STAMINA;
	m_fObjectiveAngle = 0.0f;
	m_fCurrentAngle = transform->GetWRotY();
	m_state = STATE::STAY;
	m_nStateCounter = 0;
	m_nRevival = REVIVAL;
	m_savePoint = transform->GetWPos();

	// 攻撃範囲としてコリジョンを追加
	m_pAttackCollision = gameObject->FindNameChildren("AttackCollision");
	m_pReachCollision = gameObject->FindNameChildren("ReachCollision");

	// 武器の取得
	m_pWeaponObj = gameObject->FindNameChildren("weapon");
	m_pWeaponObj->AddComponent<CTrajectory>();

	// HPバー
	m_pHPBarObj = new GameObject();
	m_pHPBarObj->SetName("HPBar");
	m_pHPBarObj->AddComponent<CProgressBar>(20);
	m_pHPBarObj->GetComponent<CProgressBar>()->SetBold(60.0f);
	m_pHPBarObj->GetComponent<CProgressBar>()->SetLength(700.0f);
	m_pHPBarObj->GetComponent<CProgressBar>()->SetFillColor(D3DCOLOR_RGBA(9, 176, 0, 255));
	m_pHPBarObj->GetComponent<CProgressBar>()->SetNonFillColor(D3DCOLOR_RGBA(200, 200, 200, 255));
	m_pHPBarObj->GetComponent<CProgressBar>()->SetBGColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	m_pHPBarObj->transform->SetPos(350.0f, 50.0f);

	// スタミナバー
	m_pStaminaBarObj = new GameObject();
	m_pHPBarObj->SetName("StaminaBar");
	m_pStaminaBarObj->AddComponent<CProgressBar>(12);
	m_pStaminaBarObj->GetComponent<CProgressBar>()->SetBold(30.0f);
	m_pStaminaBarObj->GetComponent<CProgressBar>()->SetLength(500.0f);
	m_pStaminaBarObj->GetComponent<CProgressBar>()->SetFillColor(D3DCOLOR_RGBA(255, 187, 0, 255));
	m_pStaminaBarObj->GetComponent<CProgressBar>()->SetNonFillColor(D3DCOLOR_RGBA(200, 200, 200, 255));
	m_pStaminaBarObj->GetComponent<CProgressBar>()->SetBGColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	m_pStaminaBarObj->transform->SetPos(350.0f, 120.0f);

	// 復活回数UI
	m_pRevivalText = new GameObject();
	m_pRevivalText->SetName("RevivalUI");
	m_pRevivalText->SetPriority(7);
	m_pRevivalText->AddComponent<CText>();
	m_pRevivalText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
	m_pRevivalText->GetComponent<CText>()->SetFontSize(150);
	m_pRevivalText->GetComponent<CText>()->SetFontColor(D3DCOLOR_RGBA(255, 0, 0, 255));
	m_pRevivalText->GetComponent<CText>()->SetEdgeColor(D3DCOLOR_RGBA(0, 0, 0, 255));
	m_pRevivalText->GetComponent<CText>()->SetOutlineSize(5);
	m_pRevivalText->GetComponent<CText>()->SetAlign(CText::CENTER);
	m_pRevivalText->GetComponent<CText>()->SetText(std::to_string(m_nRevival));
	m_pRevivalText->transform->SetPos(157.5f, 100.0f);

	// サウンドリスナー
	gameObject->AddComponent<CSoundListener>();
	gameObject->GetComponent<CSoundListener>()->IsUpdateAngle(false);


	// スキルコンポーネントの追加
	gameObject->AddComponent<CSkillPoint>();
	gameObject->AddComponent<CSkillManager>();

	CBugAttackSkill* pBugAttackSkill = new CBugAttackSkill();
	gameObject->GetComponent<CSkillManager>()->SetSkill(0, pBugAttackSkill);
}

//=============================================================
// [CPlayer] 終了
//=============================================================
void CPlayer::Uninit()
{
	std::vector<GameObject*> pChild = gameObject->GetChildren();
	for (unsigned int i = 0; i < gameObject->GetChildren().size(); i++)
	{
		gameObject->GetChildren()[i]->Destroy();
	}

	// UIを破棄する
	m_pHPBarObj->Destroy();
	m_pStaminaBarObj->Destroy();
	m_pRevivalText->Destroy();

	// 攻撃範囲のオブジェクトを破棄
	m_pAttackCollision->Destroy();
	m_pReachCollision->Destroy();
}

//=============================================================
// [CPlayer] 更新
//=============================================================
void CPlayer::Update()
{
	if (CManager::GetPause())
		return;

	// 移動操作
	MoveControl();

	// 向きの更新
	UpdateAngle();

	// モーションの更新
	UpdateState();

	// スタミナの更新
	UpdateStamina();

	// 回転アクション
	RollingAction();

	// 攻撃操作
	AttackControl();

	// 地面判定
	FallJudge();

	// 下に落ちたとき
	if (transform->GetWPos().y < -100.0f)
	{
		OnDeath();
	}

	// バーの更新
	m_pHPBarObj->GetComponent<CProgressBar>()->SetProgress(m_pHPBarObj->GetComponent<CProgressBar>()->GetProgress() + (m_nLife / (float)DEF_LIFE - m_pHPBarObj->GetComponent<CProgressBar>()->GetProgress()) * 0.2f + 0.01f);
	m_pStaminaBarObj->GetComponent<CProgressBar>()->SetProgress(m_pStaminaBarObj->GetComponent<CProgressBar>()->GetProgress() + (m_nStamina / (float)DEF_STAMINA - m_pStaminaBarObj->GetComponent<CProgressBar>()->GetProgress()) * 0.2f + 0.01f);
}


//=============================================================
// [CPlayer] 移動操作
//=============================================================
void CPlayer::MoveControl()
{
	if (m_state == STATE::ATTACK &&
		m_nStateCounter > 10)
		return;

	if (m_state == STATE::ROLL)
		return;

	int nChangedCounter = GameObject::Find("Camera")->GetComponent<CCameraMove>()->GetChangedCounter();
	if (0 < nChangedCounter && nChangedCounter < CCameraMove::CHANGE_TIMING)
		return;

	// コントローラー用
	short controlStickLx = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>()->GetState().Gamepad.sThumbLX;
	short controlStickLy = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>()->GetState().Gamepad.sThumbLY;

	// コントローラーのデッドゾーン
	if ((controlStickLx <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		controlStickLx > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(controlStickLy <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			controlStickLy > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		controlStickLx = 0;
		controlStickLy = 0;
	}

	// 移動処理
	if (INPUT_INSTANCE->onPress("w") ||
		INPUT_INSTANCE->onPress("a") ||
		INPUT_INSTANCE->onPress("s") ||
		INPUT_INSTANCE->onPress("d") ||
		(controlStickLx != 0 || controlStickLy != 0))
	{
		float fCameraAngle = GameObject::Find("Camera")->transform->GetWRotY();

		if (INPUT_INSTANCE->onInput("dash") &&
			m_nStamina >= DASH_STAMINA && m_bIsFloor)
		{ // ダッシュ
			CCollision::GetCollision(gameObject)->GetRigidBody()->setLinearVelocity(
				btVector3(sinf(m_fCurrentAngle + fCameraAngle) * MOVE_SPEED * DASH_RATE, CCollision::GetCollision(gameObject)->GetRigidBody()->getLinearVelocity().getY(), cosf(m_fCurrentAngle + fCameraAngle) * MOVE_SPEED * DASH_RATE)
			);
			m_state = STATE::WALK;
			m_nStateCounter = 10;
			m_nStamina -= DASH_STAMINA;
			m_bDash = true;
		}
		else
		{ // 歩き
			CCollision::GetCollision(gameObject)->GetRigidBody()->setLinearVelocity(
				btVector3(sinf(m_fCurrentAngle + fCameraAngle) * MOVE_SPEED, CCollision::GetCollision(gameObject)->GetRigidBody()->getLinearVelocity().getY(), cosf(m_fCurrentAngle + fCameraAngle) * MOVE_SPEED)
			);
			// 歩きモーションに変更
			m_state = STATE::WALK;
			m_nStateCounter = 10;
			m_bDash = false;
		}
	}
}

//=============================================================
// [CPlayer] 動く方向の角度を取得する
//=============================================================
float CPlayer::GetMoveAngle()
{
	// コントローラー用
	short controlStickLx = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>()->GetState().Gamepad.sThumbLX;
	short controlStickLy = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>()->GetState().Gamepad.sThumbLY;

	// コントローラーのデッドゾーン
	if ((controlStickLx <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		controlStickLx > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(controlStickLy <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			controlStickLy > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		controlStickLx = 0;
		controlStickLy = 0;
	}

	if (controlStickLx != 0 || controlStickLy != 0)
	{ // コントローラー操作されているとき
		return atan2f(controlStickLx, controlStickLy);
	}
	else if (INPUT_INSTANCE->onPress("w"))
	{
		if (INPUT_INSTANCE->onPress("a"))
		{
			return D3DX_PI * 1.75f;
		}
		else if (INPUT_INSTANCE->onPress("d"))
		{
			return D3DX_PI * 0.25f;
		}
		else
		{
			return 0.0f;
		}
	}
	else if (INPUT_INSTANCE->onPress("s"))
	{
		if (INPUT_INSTANCE->onPress("a"))
		{
			return D3DX_PI * 1.25f;
		}
		else if (INPUT_INSTANCE->onPress("d"))
		{
			return D3DX_PI * 0.75f;
		}
		else
		{
			return D3DX_PI;
		}
	}
	else if (INPUT_INSTANCE->onPress("a"))
	{
		return D3DX_PI * 1.5;
	}
	else if (INPUT_INSTANCE->onPress("d"))
	{
		return D3DX_PI * 0.5f;
	}

	return 0.0f;
}

//=============================================================
// [CPlayer] 攻撃操作
//=============================================================
void CPlayer::FallJudge()
{
	// レイを飛ばして地面があるか判定する
	btVector3 btRayFrom = btVector3(transform->GetWPos().x, transform->GetWPos().y, transform->GetWPos().z);
	btVector3 btRayTo = btVector3(transform->GetWPos().x, transform->GetWPos().y - 10.0f, transform->GetWPos().z);
	btCollisionWorld::ClosestRayResultCallback rayCallback(btRayFrom, btRayTo);
	CManager::GetPhysics()->GetDynamicsWorld().rayTest(btRayFrom, btRayTo, rayCallback);
	if (rayCallback.hasHit())
	{ // 当たったとき
		CCollision::GetCollision(gameObject)->GetRigidBody()->setFriction(1.0f);
		m_bIsFloor = true;

		// ジャンプ
		if (INPUT_INSTANCE->onInput("jump"))
		{
			CCollision::GetCollision(gameObject)->GetRigidBody()->setLinearVelocity(
				CCollision::GetCollision(gameObject)->GetRigidBody()->getLinearVelocity() + btVector3(0.0f, 50.0f, 0.0f)
			);
		}
	}
	else
	{
		CCollision::GetCollision(gameObject)->GetRigidBody()->applyGravity();
		CCollision::GetCollision(gameObject)->GetRigidBody()->setFriction(0.0f);
		m_bIsFloor = false;
	}
}

//=============================================================
// [CPlayer] 回転
//=============================================================
void CPlayer::RollingAction()
{
	// 回転中の処理
	if (m_state == STATE::ROLL)
	{
		CCollision::GetCollision(gameObject)->GetRigidBody()->setLinearVelocity(
			btVector3(sinf(m_fCurrentAngle + GameObject::Find("Camera")->transform->GetWRotY()) * 80.0f, CCollision::GetCollision(gameObject)->GetRigidBody()->getLinearVelocity().getY(), cosf(m_fCurrentAngle + GameObject::Find("Camera")->transform->GetWRotY()) * 80.0f)
		);

		if (GameObject::Find("Camera")->GetComponent<CCameraMove>()->GetFocus() == CCameraMove::FOCUS::PORTAL)
		{ // ポータルにフォーカスしているとき
			m_state = STATE::STAY;
		}

		return;
	}

	// 回転
	if (INPUT_INSTANCE->onInput("rolling") &&
		m_nStamina >= ROLLING_STAMINA)
	{
		m_state = STATE::ROLL;
		m_nStateCounter = gameObject->GetComponent<CMotionManager>()->FindContents("roll")->GetMaxFrame();
		m_nStamina -= ROLLING_STAMINA;
	}
}

//=============================================================
// [CPlayer] 攻撃操作
//=============================================================
void CPlayer::AttackControl()
{
	if (m_state == STATE::ROLL)
		return;

	// 攻撃モーション時のみ、武器を表示する
	std::string sMotionName;
	if (gameObject->GetComponent<CMotionManager>()->FindLayer("Base")->GetPlayContents() != nullptr)
	{
		sMotionName = gameObject->GetComponent<CMotionManager>()->FindLayer("Base")->GetPlayContents()->GetName();
	}
	if (sMotionName == "attack0" || sMotionName == "attack1")
	{ // 攻撃モーション時
		// 武器の表示
		m_pWeaponObj->SetActive(true);

		if (!m_pWeaponObj->GetComponent<CTrajectory>()->GetShow())
		{ // 最初の処理
			m_pWeaponObj->GetComponent<CTrajectory>()->ResetPos(m_pWeaponObj->transform->GetWPos());
			m_pWeaponObj->GetComponent<CTrajectory>()->SetShow(true);
		}

		// 軌道を設定する
		D3DXVECTOR3 root = { 0.0f, 2.0f, 0.0f };
		D3DXVECTOR3 tip = {0.0f, 15.0f, 0.0f};
		D3DXVec3TransformCoord(&root, &root, &m_pWeaponObj->transform->GetMatrix());
		D3DXVec3TransformCoord(&tip, &tip, &m_pWeaponObj->transform->GetMatrix());
		m_pWeaponObj->GetComponent<CTrajectory>()->AddTrajectory(root, tip);
	}
	else
	{
		// 武器の非表示
		m_pWeaponObj->SetActive(false);
		m_pWeaponObj->GetComponent<CTrajectory>()->SetShow(false);
	}

	// 攻撃アクション
	if (INPUT_INSTANCE->onInput("attack") && m_state == STATE::ATTACK &&
		m_nStateCounter <= 15)
	{ // 攻撃キーが押されたとき（1発目以降）
		
		m_nChainAttack++;

		// 攻撃動作
		AttackAction();

		// モーション
		m_state = STATE::ATTACK;
		m_nStateCounter = 40;
	}
	else if (INPUT_INSTANCE->onInput("attack") && m_state != STATE::ATTACK)
	{
		m_nChainAttack = 0;

		// 攻撃動作
		AttackAction();

		// モーション
		m_state = STATE::ATTACK;
		m_nStateCounter = 40;
	}
}

//=============================================================
// [CPlayer] 攻撃動作
//=============================================================
void CPlayer::AttackAction()
{
	// 攻撃範囲に重なっているオブジェクトを取得する
	std::vector<GameObject*>& pOverlapping = CCollision::GetCollision(m_pAttackCollision)->GetOverlappingGameObjects();

	// ダメージを与える
	bool bIsHit = false;
	for (unsigned int i = 0; i < pOverlapping.size(); i++)
	{
		CEnemy* pEnemy = pOverlapping[i]->GetComponent<CEnemy>(true);
		if (pEnemy != nullptr)
		{
			pEnemy->DamageEnemy(5, transform->GetWPos());
			bIsHit = true;
		}
	}

	// リーチ内に敵が入っていたらその方向に飛ばす
	std::vector<GameObject*>& pReachOverlapping = CCollision::GetCollision(m_pReachCollision)->GetOverlappingGameObjects();
	for (unsigned int i = 0; i < pOverlapping.size(); i++)
	{
		CEnemy* pEnemy = pReachOverlapping[i]->GetComponent<CEnemy>(true);
		if (pEnemy != nullptr)
		{
			float fAngle = PosAngle(transform->GetWPos(), pEnemy->transform->GetWPos());
			CCollision::GetCollision(gameObject)->GetRigidBody()->setLinearVelocity(btVector3(sinf(fAngle) * 80.0f, 0.0f, cosf(fAngle) * 80.0f));
			break;
		}
	}

	// 音を鳴らす
	GameObject* pSlashSound = new GameObject;
	pSlashSound->AddComponent<CSound>();
	pSlashSound->GetComponent<CSound>()->LoadWav(bIsHit ? "data\\SOUND\\SE\\slash_hit.wav" : "data\\SOUND\\SE\\slash.wav");
	pSlashSound->GetComponent<CSound>()->Play();
	pSlashSound->GetComponent<CSound>()->SetPitch(rand() % 3 * 0.1f + 1.0f);
	pSlashSound->GetComponent<CSound>()->IsStoppedDestroy();
}

//=============================================================
// [CPlayer] ダメージを受けたときの処理
//=============================================================
void CPlayer::OnDamage()
{
	// カメラを揺らす
	GameObject::Find("Camera")->GetComponent<CCameraMove>()->SetShake(10, 3);

	// 音を鳴らす
	GameObject* pDamageSound = new GameObject;
	pDamageSound->AddComponent<CSound>();
	pDamageSound->GetComponent<CSound>()->LoadWav("data\\SOUND\\SE\\player_damage.wav");
	pDamageSound->GetComponent<CSound>()->SetVolume(0.6f);
	pDamageSound->GetComponent<CSound>()->Play();
	pDamageSound->GetComponent<CSound>()->SetPitch(rand() % 3 * 0.1f + 1.0f);
	pDamageSound->GetComponent<CSound>()->IsStoppedDestroy();
}

//=============================================================
// [CPlayer] 死亡時の処理
//=============================================================
void CPlayer::OnDeath()
{
	// 復活回数をデクリメント
	m_nRevival--;
	m_pRevivalText->GetComponent<CText>()->SetText(std::to_string(m_nRevival));

	if (m_nRevival <= 0)
	{ // 復活できなくなったとき
		gameObject->Destroy(true);

		// シーンを変更する
		CGameScene* pGameScene = (CGameScene*)CSceneManager::GetInstance()->GetScene("game")->pScene;
		pGameScene->OnGameOver();
	}
	else
	{ // 復活処理
		// セーブポイントに移動する
		CCollision::GetCollision(gameObject)->GetRigidBody()->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
		CCollision::GetCollision(gameObject)->GetRigidBody()->getWorldTransform().getOrigin() = btVector3(m_savePoint.x, m_savePoint.y, m_savePoint.z);
		gameObject->transform->SetRot(0.0f, 0.0f, 0.0f);

		// カメラ位置をリセットする
		GameObject* pCamera = GameObject::Find("Camera");
		if (pCamera != nullptr)
		{
			pCamera->transform->SetPos(m_savePoint);
		}
	}
}

//=============================================================
// [CPlayer] ダメージを与える
//=============================================================
void CPlayer::DamagePlayer(const int& nDamage)
{
	// 体力からダメージを減算
	m_nLife -= nDamage;

	OnDamage();

	// 死亡判定
	if (m_nLife <= 0)
	{ // 体力が無くなったとき
		// 死亡時の処理
		OnDeath();
	}
}

//=============================================================
// [CPlayer] プレイヤーをヒール
//=============================================================
void CPlayer::HealPlayer(const int& nHeal)
{
	m_nLife += nHeal;
	if (m_nLife > DEF_LIFE)
		m_nLife = DEF_LIFE;
}

//=============================================================
// [CPlayer] 向きの方向処理
//=============================================================
void CPlayer::UpdateAngle()
{
	if (m_state == STATE::ATTACK)
		return;

	if (m_state == STATE::ROLL)
		return;

	// コントローラー用
	short controlStickLx = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>()->GetState().Gamepad.sThumbLX;
	short controlStickLy = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>()->GetState().Gamepad.sThumbLY;

	// コントローラーのデッドゾーン
	if ((controlStickLx <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		controlStickLx > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(controlStickLy <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		controlStickLy > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		controlStickLx = 0;
		controlStickLy = 0;
	}

	if (INPUT_INSTANCE->onPress("w") ||
		INPUT_INSTANCE->onPress("a") ||
		INPUT_INSTANCE->onPress("s") ||
		INPUT_INSTANCE->onPress("d") ||
		(controlStickLx != 0 || controlStickLy != 0))
	{
		// カメラの角度を取得する
		GameObject* pCamera = GameObject::Find("Camera");

		// 目標方向を取得する
		m_fObjectiveAngle = GetMoveAngle();

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
		transform->SetRot(0.0f, m_fCurrentAngle + GameObject::Find("Camera")->transform->GetWRotY(), 0.0f);
	}

}

//=============================================================
// [CPlayer] 状態の更新処理
//=============================================================
void CPlayer::UpdateState()
{
	if (m_nStateCounter <= 0)
	{ // カウンターが終了したときの処理
		m_state = STATE::STAY;
	}
	else
	{
		// カウンターを減らす
		m_nStateCounter--;
	}

	switch (m_state)
	{
	case CPlayer::STAY:
		gameObject->GetComponent<CMotionManager>()->Play("stay");
		break;
	case CPlayer::WALK:
		gameObject->GetComponent<CMotionManager>()->Play("walk");
		break;
	case CPlayer::ATTACK:
		gameObject->GetComponent<CMotionManager>()->Play("attack" + std::to_string(m_nChainAttack % 2));
		break;
	case CPlayer::ROLL:
		gameObject->GetComponent<CMotionManager>()->Play("roll");
		break;
	}
}

//=============================================================
// [CPlayer] スタミナの更新処理
//=============================================================
void CPlayer::UpdateStamina()
{
	// スタミナの回復
	if (m_nStamina < DEF_STAMINA &&
		(m_state == STATE::STAY || m_state == STATE::WALK) && 
		(!m_bDash && !INPUT_INSTANCE->onInput("dash")))
	{
		m_nStamina++;
	}
}