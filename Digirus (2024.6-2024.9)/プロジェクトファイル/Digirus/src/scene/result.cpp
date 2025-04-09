//=============================================================
//
// ���U���g�V�[�� [result.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "result.h"
#include "component/2d/polygon.h"
#include "manager.h"

//=============================================================
// [CResultScene] ������
//=============================================================
void CResultScene::Init()
{
	//m_pBG = new GameObject();
	//m_pBG->AddComponent<CPolygon>("./data/TEXTURE/result.png");
	//m_pBG->transform->SetSize(1920.0f, 1080.0f);
	//m_pBG->transform->SetPos(0.0f, 0.0f);
}

//=============================================================
// [CResultScene] �I��
//=============================================================
void CResultScene::Uninit()
{

}

//=============================================================
// [CResultScene] �X�V
//=============================================================
void CResultScene::Update()
{
	if (INPUT_INSTANCE->onInput("select_enter"))
	{
		CSceneManager::GetInstance()->SetScene("os");
	}
}

//=============================================================
// [CResultScene] �`��
//=============================================================
void CResultScene::Draw()
{

}




//=============================================================
// [CResult] ������
//=============================================================
void CResult::Init()
{
	m_pBGObj = new GameObject;
	m_pBGObj->transform->SetSize(static_cast<float>(CRenderer::SCREEN_WIDTH), static_cast<float>(CRenderer::SCREEN_HEIGHT));
	m_pBGObj->AddComponent<CPolygon>();
	m_pBGObj->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
}