//=============================================================
//
// デバックシーン [debug.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "debug.h"
#include "component/3d/camera.h"
#include "component/3d/collision.h"
#include "component/3d/light.h"
#include "scripts/camera_move.h"


//=============================================================
// [CDebugScene] 初期化
//=============================================================
void CDebugScene::Init()
{
	// ゲームオブジェクトの生成
	m_pCamera = new GameObject();
	m_pCamera->transform->Translate(0.0f, 100.0f, -100.0f);
	m_pCamera->transform->LookAt({ 0.0f, 0.0f, 0.0f });
	m_pCamera->SetName("Camera");

	// コンポーネントの追加
	m_pCamera->AddComponent<CCamera>();
	m_pCamera->AddComponent<CCameraMove>();

	// 色の設定
	CCamera* pCameraComp = m_pCamera->GetComponent<CCamera>();
	pCameraComp->SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));

	// ライトの作成
	GameObject* pD3DLight = new GameObject();
	pD3DLight->SetTag(TAG_NODESTROY);
	CD3DLight* pLightComp = pD3DLight->AddComponent<CD3DLight>();
	pLightComp->SetType(D3DLIGHT_DIRECTIONAL);
	pLightComp->SetDiffuse(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pLightComp->SetDirection(D3DXVECTOR3(0.22f, -0.87f, 0.44f));

	pLightComp = pD3DLight->AddComponent<CD3DLight>();
	pLightComp->SetType(D3DLIGHT_DIRECTIONAL);
	pLightComp->SetDiffuse(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pLightComp->SetDirection(D3DXVECTOR3(-0.18f, 0.88f, -0.44f));

	pLightComp = pD3DLight->AddComponent<CD3DLight>();
	pLightComp->SetType(D3DLIGHT_DIRECTIONAL);
	pLightComp->SetDiffuse(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pLightComp->SetDirection(D3DXVECTOR3(0.89f, 0.11f, 0.44f));

	// ワールド読み込み
	m_worldLoader = new WorldLoader();
	m_worldLoader->Load("data\\WORLD\\stage_01.wdit");



	GameObject* pTest = new GameObject();
	pTest->transform->SetScale(0.2f);
	CBoxCollider* pBoxCol = pTest->AddComponent<CBoxCollider>();
	pBoxCol->SetSize({ 10.0f, 10.0f, 10.0f });
	CCollision::GetCollision(pTest)->UpdateCollision();
}

//=============================================================
// [CDebugScene] 終了
//=============================================================
void CDebugScene::Uninit()
{
	if (m_worldLoader != nullptr)
	{
		delete m_worldLoader;
		m_worldLoader = nullptr;
	}
}

//=============================================================
// [CDebugScene] 更新
//=============================================================
void CDebugScene::Update()
{

}

//=============================================================
// [CDebugScene] 描画
//=============================================================
void CDebugScene::Draw()
{

}