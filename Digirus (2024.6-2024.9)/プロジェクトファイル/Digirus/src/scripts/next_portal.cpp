//=============================================================
//
// 次なるポータル [next_portal.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "next_portal.h"
#include "component/3d/field.h"
#include "component/3d/camera.h"
#include "component/3d/render_texture.h"
#include "scripts/camera_move.h"
#include "component/3d/collision.h"
#include "component/3d/light.h"
#include "component/3d/mesh.h"
#include "scene/game.h"
#include "manager.h"
#include "scene.h"

//=============================================================
// [CNextPortal] コンストラクタ
//=============================================================
CNextPortal::CNextPortal()
{
	m_bIsGoal = false;
}

//=============================================================
// [CNextPortal] 初期化
//=============================================================
void CNextPortal::Init()
{
	// ディスプレイ
	m_pDisplayObj = new GameObject();
	m_pDisplayObj->SetName("ComputerDisplay");
	CField* pDisplay = m_pDisplayObj->AddComponent<CField>();
	m_pDisplayObj->SetParent(gameObject);
	m_pDisplayObj->transform->Rotate(-D3DX_PI * 0.5f, 0.0f, 0.0f);

	// 行き先のカメラ
	m_pNextCamera = new GameObject();
	CCamera* pNextCamera = m_pNextCamera->AddComponent<CCamera>();
	pNextCamera->SetVisible(false);
	pNextCamera->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	m_pNextCamera->AddComponent<CCameraMove>()->enabled = false;
	m_pNextCamera->GetComponent<CCamera>()->GetSkybox()->SetTexture(0, "data\\TEXTURE\\SKY\\Daylight.png");


	// レンダーテクスチャの設定
	CRenderTexture* pNextTexture = m_pNextCamera->AddComponent<CRenderTexture>();
	pNextTexture->SetTargetCamera(pNextCamera);
	pNextTexture->Create(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT);

	// ディスプレイのテクスチャ設定
	pDisplay->BindTexture(pNextTexture->GetTexture());
}

//=============================================================
// [CNextPortal] 終了
//=============================================================
void CNextPortal::Uninit()
{
	m_pDisplayObj->Destroy();
	m_pDisplayObj = nullptr;
}

//=============================================================
// [CNextPortal] 更新
//=============================================================
void CNextPortal::Update()
{
	if (CManager::GetPause())
		return;

	// 侵入コリジョンを取得
	GameObject* pEnter = gameObject->FindNameChildren("Enter");

	if (pEnter != nullptr)
	{
		// コリジョンに侵入しているオブジェクトを取得
		std::vector<GameObject*>& overlapping = CCollision::GetCollision(pEnter)->GetOverlappingGameObjects();

		for (int i = 0; i < (int)overlapping.size(); i++)
		{
			if (overlapping[i]->GetTag() == "Player")
			{ // プレイヤーのとき
				// カメラの取得
				GameObject* pCamera = GameObject::Find("Camera");
				if (pCamera == nullptr ||
					pCamera->GetComponent<CCameraMove>()->GetFocus() == CCameraMove::FOCUS::PORTAL)
				{
					return;
				}

				// フォーカスをコンピュータに切り替える
				pCamera->GetComponent<CCameraMove>()->SetNextPortal(this);
				pCamera->GetComponent<CCameraMove>()->SetFocus(CCameraMove::FOCUS::PORTAL);
			}
		}
	}
}

//=============================================================
// [CNextPortal] コリジョンに入ったとき
//=============================================================
void CNextPortal::OnTriggerEnter(GameObject* other)
{
	//if (other->GetTag() == "Player")
	//{ // プレイヤーのとき
	//	// カメラの取得
	//	GameObject* pCamera = GameObject::Find("Camera");
	//	if (pCamera == nullptr)
	//	{
	//		return;
	//	}

	//	// フォーカスをコンピュータに切り替える
	//	pCamera->GetComponent<CCameraMove>()->SetNextPortal(this);
	//	pCamera->GetComponent<CCameraMove>()->SetFocus(CCameraMove::FOCUS::PORTAL);
	//}
}

//=============================================================
// [CNextPortal] ディスプレイサイズの設定
//=============================================================
void CNextPortal::SetDisplaySize(const float& x, const float& y)
{
	m_pDisplayObj->GetComponent<CField>()->Set(x, y);
}

//=============================================================
// [CNextPortal] ディスプレイオフセットの設定
//=============================================================
void CNextPortal::SetDisplayOffset(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	m_pDisplayObj->transform->SetPos(pos);
	m_pDisplayObj->transform->SetRot(rot);
}

//=============================================================
// [CNextPortal] 行き先の設定
//=============================================================
void CNextPortal::SetNextPos(const D3DXVECTOR3& pos)
{
	m_nextPos = pos;

	// カメラの設定
	ResetNextCamera();
}

//=============================================================
// [CNextPortal] ゴールにするか
//=============================================================
void CNextPortal::SetIsGoal(const bool& bGoal)
{
	// 状態を変更する
	m_bIsGoal = bGoal;

	// ポータルの画面を変更する
	CField* pDisplay = m_pDisplayObj->GetComponent<CField>();
	if (m_bIsGoal)
	{
		CGameScene* pGameScene = (CGameScene*)CSceneManager::GetInstance()->GetScene("game")->pScene;
		pDisplay->BindTexture(pGameScene->GetResultTexture());
	}
	else
	{
		pDisplay->BindTexture(m_pNextCamera->GetComponent<CRenderTexture>()->GetTexture());
	}
}

//=============================================================
// [CNextPortal] カメラのリセット
//=============================================================
void CNextPortal::ResetNextCamera()
{
	// カメラの設定
	m_pNextCamera->transform->SetPos(m_nextPos + D3DXVECTOR3(0.0f, 70.0f, -70.0f));
	m_pNextCamera->transform->LookAt(m_nextPos + D3DXVECTOR3(0.0f, 10.0f, 10.0f));
}