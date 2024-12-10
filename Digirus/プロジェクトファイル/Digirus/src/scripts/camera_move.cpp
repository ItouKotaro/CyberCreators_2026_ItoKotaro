//=============================================================
//
// カメラの動き [camera_move.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "camera_move.h"
#include "component/3d/camera.h"
#include "component/3d/mesh.h"
#include "component/other/sound.h"
#include "scene/game.h"
#include "scripts/player.h"
#include "manager.h"
#include "benlib.h"

// 静的メンバ変数の初期化
const int CCameraMove::CHANGING_TIME = 90;
const int CCameraMove::CHANGE_TIMING = 40;

//=============================================================
// [CCameraMove] 初期化
//=============================================================
void CCameraMove::Init()
{
	m_focus = FOCUS::PLAYER;
	m_nChangedCounter = 0;
	m_playerCameraOffset = { 0.0f, 40.0f, -80.0f };
	m_nShakePower = 0;
	m_nShakeTime = 0;
}

//=============================================================
// [CCameraMove] 更新
//=============================================================
void CCameraMove::Update()
{
	if (CManager::GetPause())
		return;

	switch (m_focus)
	{
	case FOCUS::PLAYER:
		PlayerFocus();
		break;
	case FOCUS::PORTAL:
		PortalFocus();
		break;
	}

	// 振動
	if (m_nShakeTime > 0)
	{
		D3DXVECTOR3 shake = { 
			(float)(rand() % (m_nShakePower * 2) - m_nShakePower) * 0.1f,
			(float)(rand() % (m_nShakePower * 2) - m_nShakePower) * 0.1f,
			(float)(rand() % (m_nShakePower * 2) - m_nShakePower) * 0.1f
		};
		transform->SetPos(transform->GetPos() + shake);

		// カウンターを進める
		m_nShakeTime--;
	}
}

//=============================================================
// [CCameraMove] フォーカス対象の設定
//=============================================================
void CCameraMove::SetFocus(FOCUS focus)
{
	m_focus = focus;
	m_portalProgress = PORTAL_PROGRESS::INIT_POS;
}

//=============================================================
// [CCameraMove] プレイヤーへのフォーカス
//=============================================================
void CCameraMove::PlayerFocus()
{
	// プレイヤーを取得する
	GameObject* pPlayer = GameObject::Find("Player");

	if (pPlayer == nullptr)
	{
		return;	// 取得できなかったときは処理をスルー
	}

	// カメラの目的地を計算する
	D3DXVECTOR3 objectivePos;
	objectivePos = pPlayer->transform->GetWPos();
	if (m_nChangedCounter > CHANGE_TIMING)
		objectivePos += m_playerCameraOffset;
	else
		objectivePos += {0.0f, 40.0f, -80.0f};
	
	
	// 減衰をつける
	D3DXVECTOR3 cameraPos = transform->GetWPos();
	cameraPos += (objectivePos - transform->GetWPos()) * 0.08f;

	// 位置を適用する
	transform->SetPos(cameraPos);

	// 指定の方向に向かせる
	transform->LookAt(pPlayer->transform->GetWPos() + D3DXVECTOR3(0.0f, 10.0f, 10.0f));

	if (m_nChangedCounter > 0)
	{
		m_nChangedCounter--;
	}
}

//=============================================================
// [CCameraMove] ポータルへのフォーカス
//=============================================================
void CCameraMove::PortalFocus()
{
	// ポータルが設定済みか
	if (m_portal == nullptr)
	{ // 設定されていない
		return;	// スキップ
	}

	if (GameObject::Find("Player") == nullptr)
	{ // プレイヤーが存在しないとき
		return;	// スキップ
	}

	// 進行度ごとの処理
	D3DXVECTOR3 objectivePos, frontPos, cameraPos, targetPos;;
	D3DXVECTOR3 cameraRot, objectiveRot;
	std::vector<GameObject*> pAllChildren = GameObject::Find("Player")->GetChildren(true);

	// ゴール時の更新処理
	if (m_portal->GetIsGoal() && m_portalProgress != PORTAL_PROGRESS::GOAL)
	{
		CGameScene* pGameScene = (CGameScene*)CSceneManager::GetInstance()->GetScene("game")->pScene;
		pGameScene->UpdateGoal();
	}

	switch (m_portalProgress)
	{
	case PORTAL_PROGRESS::INIT_POS:
		// プレイヤーを非表示にする
		for (int i = 0; i < (int)pAllChildren.size(); i++)
		{
			if (pAllChildren[i]->GetComponent<CMesh>() != nullptr)
			{
				pAllChildren[i]->SetActive(false);
			}
		}
		GameObject::Find("Player")->GetComponent<CPlayer>()->enabled = false;

		// ポータルの方向に向かせる
		transform->LookAt(m_portal->GetDisplayObj()->transform->GetWPos());

		// コンピューターの位置から初期地を割り出す
		objectivePos = { 0.0f, 100.0f, 0.0f };
		D3DXVec3TransformCoord(&objectivePos, &objectivePos, &m_portal->GetDisplayObj()->transform->GetMatrix());

		if (m_portal->GetIsGoal())
		{
			CGameScene* pGameScene = (CGameScene*)CSceneManager::GetInstance()->GetScene("game")->pScene;
			pGameScene->onInitGoal();
		}

		// 減衰をつける
		cameraPos = transform->GetWPos();
		cameraPos += (objectivePos - transform->GetWPos()) * 0.08f;
		transform->SetPos(cameraPos);		// 位置を適用する

		// 次の段階へ
		if (PosDistance(transform->GetWPos(), objectivePos) < 0.4f)
		{
			// 初期位置に移動
			transform->SetPos(objectivePos);

			// プレイヤーを転移先に移動する
			if (!m_portal->GetIsGoal())
			{ // ゴールじゃないとき
				CCollision::GetCollision(GameObject::Find("Player"))->GetRigidBody()->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
				CCollision::GetCollision(GameObject::Find("Player"))->GetRigidBody()->getWorldTransform().getOrigin() = btVector3(
					m_portal->GetNextPos().x, m_portal->GetNextPos().y, m_portal->GetNextPos().z);

				// プレイヤーを有効にする
				GameObject::Find("Player")->GetComponent<CPlayer>()->enabled = true;

				// プレイヤーのセーブポイントを設定する
				GameObject::Find("Player")->GetComponent<CPlayer>()->SetSavePoint(m_portal->GetNextPos());
			}

			// 音を鳴らす
			GameObject* pGotoSound = new GameObject;
			pGotoSound->AddComponent<CSound>()->LoadWav("data\\SOUND\\SE\\goto_display.wav");
			pGotoSound->GetComponent<CSound>()->IsStoppedDestroy();
			pGotoSound->GetComponent<CSound>()->Play();

			m_portalProgress = PORTAL_PROGRESS::GO_MOVE;		// 次へ
		}
		break;

	case PORTAL_PROGRESS::GO_MOVE:

		// 移動先カメラのフォーカスをプレイヤーにする
		m_portal->GetNextCamera()->GetComponent<CCameraMove>()->enabled = true;

		// プレイヤーを表示する
		if (!m_portal->GetIsGoal())
		{
			for (int i = 0; i < (int)pAllChildren.size(); i++)
			{
				if (pAllChildren[i]->GetComponent<CMesh>() != nullptr)
				{
					pAllChildren[i]->SetActive(true);
				}
			}
		}

		// ポータルの方向に向かせる
		targetPos = m_portal->GetDisplayObj()->transform->GetWPos();
		float fTargetAngle, fTargetHeightAngle;
		fTargetAngle = -atan2f(targetPos.x - transform->GetWPos().x, targetPos.z - transform->GetWPos().z);
		fTargetHeightAngle = atan2f(sqrtf(fabsf(targetPos.x - transform->GetWPos().x) * fabsf(targetPos.x - transform->GetWPos().x) +
			fabsf(targetPos.z - transform->GetWPos().z) * fabsf(targetPos.z - transform->GetWPos().z)), (targetPos.y - transform->GetWPos().y));
		objectiveRot = D3DXVECTOR3(-fTargetHeightAngle + D3DX_PI * 0.5f, fTargetAngle, 0.0f);
		
		objectiveRot.z = m_portal->GetDisplayObj()->transform->GetWRotZ() * -1;

		// 向かせる減衰
		cameraRot = transform->GetWRot();
		cameraRot += (objectiveRot - transform->GetWRot()) * 0.08f;
		transform->SetRot(cameraRot);

		// コンピューターの位置から画面を割り出す
		frontPos = { m_portal->GetDisplayAdjust().x, m_portal->GetFrontStop(), m_portal->GetDisplayAdjust().y };
		D3DXVec3TransformCoord(&frontPos, &frontPos, &m_portal->GetDisplayObj()->transform->GetMatrix());

		// 減衰をつける
		cameraPos = transform->GetWPos();
		cameraPos += (frontPos - transform->GetWPos()) * 0.08f;
		transform->SetPos(cameraPos);		// 位置を適用する

		// 次の段階へ
		if (PosDistance(transform->GetWPos(), frontPos) < 0.001f)
		{
			if (m_portal->GetIsGoal())
			{ // ゴールのとき
				// データを記録する
				CGameScene* pGameScene = (CGameScene*)CSceneManager::GetInstance()->GetScene("game")->pScene;
				pGameScene->SavePlayData();

				// ゴールに切り替える
				m_portalProgress = PORTAL_PROGRESS::GOAL;
				break;
			}

			// 移動先のカメラを有効にする
			m_portal->GetNextCamera()->GetComponent<CCamera>()->SetVisible(true);

			// メインカメラを無効にする
			gameObject->GetComponent<CCamera>()->SetVisible(false);

			m_portalProgress = PORTAL_PROGRESS::NEXT_STAGE;		// 次へ
		}
		break;

	case PORTAL_PROGRESS::NEXT_STAGE:		// 次のステージへと完全移行する
		// 次の場所へとカメラを移動する
		transform->SetPos(m_portal->GetNextCamera()->transform->GetWPos());
		transform->SetRot(m_portal->GetNextCamera()->transform->GetWRot());

		// 終了へ
		m_portalProgress = PORTAL_PROGRESS::END;
		break;

	case PORTAL_PROGRESS::END:
		// メインカメラを有効にする
		gameObject->GetComponent<CCamera>()->SetVisible(true);

		// 移動先のカメラを無効にする
		m_portal->GetNextCamera()->GetComponent<CCamera>()->SetVisible(false);
		m_portal->GetNextCamera()->GetComponent<CCameraMove>()->enabled = false;
		m_portal->ResetNextCamera();
		m_portal = nullptr;

		// フォーカスをプレイヤーに移す
		m_focus = FOCUS::PLAYER;
		m_portalProgress = PORTAL_PROGRESS::INIT_POS;
		break;

	case PORTAL_PROGRESS::GOAL:
		// リザルトタグ以外のオブジェクトを破棄する
		std::vector<GameObject*> pObjects = GameObject::GetAllGameObjects();
		for (int i = 0; i < pObjects.size(); i++)
		{
			if (pObjects[i]->GetTag() != "result" && pObjects[i]->GetTag() != TAG_NODESTROY)
			{ // リザルトタグ以外の場合
				pObjects[i]->Destroy();	// 破棄
			}
			else
			{ // リザルトタグが付いている場合
				pObjects[i]->SetVisible(true);	// 表示
			}
		}

		// ゴールの最終処理
		CGameScene* pGameScene = (CGameScene*)CSceneManager::GetInstance()->GetScene("game")->pScene;
		pGameScene->FinishGoal();
		break;
	}
}

//=============================================================
// [CCameraMove] ポータルの設定
//=============================================================
void CCameraMove::SetNextPortal(CNextPortal* pNextPortal)
{
	m_portal = pNextPortal;
}

//=============================================================
// [CCameraMove] 振動
//=============================================================
void CCameraMove::SetShake(const int& nPower, const int& nTime)
{
	m_nShakePower = nPower;
	m_nShakeTime = nTime;
}

//=============================================================
// [CChangePlayerFocus] コリジョンにいるとき
//=============================================================
void CChangePlayerFocus::OnTriggerStay(GameObject* other)
{
	if (other->GetTag() == "Player")
	{ // プレイヤーのとき
		GameObject::Find("Camera")->GetComponent<CCameraMove>()->SetCameraPlayerOffset(m_changedOffset);
	}
}
