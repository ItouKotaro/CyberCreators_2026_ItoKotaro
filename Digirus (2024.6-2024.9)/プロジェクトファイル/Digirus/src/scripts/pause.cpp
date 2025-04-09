//=============================================================
//
// ポーズ [pause.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "pause.h"
#include "component/2d/polygon.h"
#include "component/2d/text.h"
#include "component/other/sound.h"
#include "manager.h"
#include "scene/game.h"

const int CPause::BUTTON_WIDTH = 620;
const int CPause::BUTTON_HEIGHT = 130;
const int CPause::BUTTON_SPACE = 30;

//=============================================================
// [CPause] 初期化
//=============================================================
void CPause::Init()
{
	// 変数の初期化
	m_bIsPause = false;

	// カーソル
	m_pCursor = new CCursor();
	m_pCursor->Init();
	m_pCursor->SetPriority(22);

	// 背景
	m_pBG = new GameObject;
	m_pBG->SetPriority(20);
	m_pBG->AddComponent<CPolygon>();
	m_pBG->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(0, 0, 0, 0.9f));
	m_pBG->transform->SetSize(static_cast<float>(CRenderer::SCREEN_WIDTH), static_cast<float>(CRenderer::SCREEN_HEIGHT));

	// ポーズテキスト
	m_pPauseText = new GameObject;
	m_pPauseText->SetPriority(21);
	m_pPauseText->AddComponent<CText>();
	m_pPauseText->GetComponent<CText>()->SetAlign(CText::ALIGN::CENTER);
	m_pPauseText->GetComponent<CText>()->SetFont("ベストテン-CRT");
	m_pPauseText->GetComponent<CText>()->SetText("<size=160>ポーズ");
	m_pPauseText->transform->SetPos(static_cast<float>(CRenderer::SCREEN_WIDTH/2), 180.0f);

	// 続ける
	m_pContinue = new GameObject;
	m_pContinue->SetPriority(21);
	m_pContinue->AddComponent<CPolygon>();
	m_pContinue->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\PAUSE\\continue.png");
	m_pContinue->transform->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	m_pContinue->transform->SetPos(static_cast<float>((CRenderer::SCREEN_WIDTH - BUTTON_WIDTH) / 2), 400.0f);

	// リトライ
	m_pRetry = new GameObject;
	m_pRetry->SetPriority(21);
	m_pRetry->AddComponent<CPolygon>();
	m_pRetry->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\PAUSE\\retry.png");
	m_pRetry->transform->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	m_pRetry->transform->SetPos(static_cast<float>((CRenderer::SCREEN_WIDTH - BUTTON_WIDTH) / 2), 400.0f + static_cast<float>((BUTTON_HEIGHT + BUTTON_SPACE) * 1));

	// ホームへ戻るボタン
	m_pBackHome = new GameObject;
	m_pBackHome->SetPriority(21);
	m_pBackHome->AddComponent<CPolygon>();
	m_pBackHome->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\PAUSE\\home.png");
	m_pBackHome->transform->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	m_pBackHome->transform->SetPos(static_cast<float>((CRenderer::SCREEN_WIDTH - BUTTON_WIDTH) / 2), static_cast<float>(400.0f + (BUTTON_HEIGHT + BUTTON_SPACE) * 2));

	// 非表示
	SetShow(m_bIsPause);
	SetPause(false);
}

//=============================================================
// [CPause] 終了
//=============================================================
void CPause::Uninit()
{
	// カーソルの破棄
	if (m_pCursor != nullptr)
	{
		m_pCursor->Uninit();
		delete m_pCursor;
		m_pCursor = nullptr;
	}

	// オブジェクトの破棄
	m_pBG->Destroy();
	m_pPauseTextBG->Destroy();
	m_pPauseText->Destroy();
	m_pContinue->Destroy();
	m_pRetry->Destroy();
	m_pBackHome->Destroy();
}

//=============================================================
// [CPause] 更新
//=============================================================
void CPause::Update()
{
	// ゲームシーンを取得する
	CGameScene* pGameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);

	// ゴールしているときは処理しない
	if (pGameScene->IsGoal())
		return;

	// ポーズ中の処理
	if (m_bIsPause)
	{
		// 制限時間の効果音が流れているときは停止する
		if (pGameScene->GetClockSound()->GetComponent<CSound>()->GetState() == CSound::PLAYING)
		{
			pGameScene->GetClockSound()->GetComponent<CSound>()->Stop();
		}

		// カーソルの更新
		m_pCursor->Update();

		// 続ける
		if (m_pContinue->transform->GetPos().x <= m_pCursor->GetPos().x &&
			m_pCursor->GetPos().x <= m_pContinue->transform->GetPos().x + BUTTON_WIDTH &&
			m_pContinue->transform->GetPos().y <= m_pCursor->GetPos().y &&
			m_pCursor->GetPos().y <= m_pContinue->transform->GetPos().y + BUTTON_HEIGHT)
		{
			if (INPUT_INSTANCE->onInput("click"))
			{ // クリックされたとき
				SetPause(false);
			}

			m_pContinue->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(120, 120, 120, 255));
		}
		else
		{
			m_pContinue->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
		}

		// リトライ
		if (m_pRetry->transform->GetPos().x <= m_pCursor->GetPos().x &&
			m_pCursor->GetPos().x <= m_pRetry->transform->GetPos().x + BUTTON_WIDTH &&
			m_pRetry->transform->GetPos().y <= m_pCursor->GetPos().y &&
			m_pCursor->GetPos().y <= m_pRetry->transform->GetPos().y + BUTTON_HEIGHT)
		{
			if (INPUT_INSTANCE->onInput("click"))
			{ // クリックされたとき
				CSceneManager::GetInstance()->GetScene("game")->pScene->Uninit();
				GameObject::DestroyAll();
				GameObject::DestroyDeathFlag();
				CSceneManager::GetInstance()->GetScene("game")->pScene->Init();
				return;
			}

			m_pRetry->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(120, 120, 120, 255));
		}
		else
		{
			m_pRetry->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
		}

		// ホームへ戻る
		if (m_pBackHome->transform->GetPos().x <= m_pCursor->GetPos().x &&
			m_pCursor->GetPos().x <= m_pBackHome->transform->GetPos().x + BUTTON_WIDTH &&
			m_pBackHome->transform->GetPos().y <= m_pCursor->GetPos().y &&
			m_pCursor->GetPos().y <= m_pBackHome->transform->GetPos().y + BUTTON_HEIGHT)
		{
			if (INPUT_INSTANCE->onInput("click"))
			{ // クリックされたとき
				CSceneManager::GetInstance()->SetScene("os");
			}

			m_pBackHome->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(120, 120, 120, 255));
		}
		else
		{
			m_pBackHome->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
		}
	}

	// ポーズキーを押されたとき
	if (INPUT_INSTANCE->onInput("pause"))
	{
		SetPause(!m_bIsPause);
	}
}

//=============================================================
// [CPause] ポーズの設定
//=============================================================
void CPause::SetPause(const bool& bPause)
{
	m_bIsPause = bPause;
	CManager::SetPause(bPause);
	SetShow(m_bIsPause);
}

//=============================================================
// [CPause] ポーズの表示
//=============================================================
void CPause::SetShow(const bool& bShow)
{
	m_pBG->SetVisible(bShow);
	//m_pPauseTextBG->SetVisible(bShow);
	m_pPauseText->SetVisible(bShow);
	m_pContinue->SetVisible(bShow);
	m_pRetry->SetVisible(bShow);
	m_pBackHome->SetVisible(bShow);
	m_pCursor->SetVisible(bShow);
}