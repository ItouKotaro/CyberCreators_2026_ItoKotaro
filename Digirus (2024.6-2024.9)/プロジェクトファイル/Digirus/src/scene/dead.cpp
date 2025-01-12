//=============================================================
//
// 死亡画面 [dead.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "dead.h"
#include "component/2d/polygon.h"
#include "component/2d/text.h"
#include "component/other/sound.h"
#include "manager.h"

//=============================================================
// [CDeadScene] 初期化
//=============================================================
void CDeadScene::Init()
{
	// 変数の初期化
	m_fAlpha = 1.0f;
	m_fRollingRetryIcon = 0.0f;

	// 音を鳴らす
	GameObject* pDeadSound = new GameObject;
	pDeadSound->AddComponent<CSound>()->LoadWav("data\\SOUND\\SE\\dead.wav");
	pDeadSound->GetComponent<CSound>()->IsStoppedDestroy();
	pDeadSound->GetComponent<CSound>()->Play();
	pDeadSound->GetComponent<CSound>()->SetVolume(50.0f);

	// 音を鳴らす
	GameObject* pDeadBGM = new GameObject;
	pDeadBGM->AddComponent<CSound>()->LoadWav("data\\SOUND\\BGM\\dead.wav");
	pDeadBGM->GetComponent<CSound>()->SetLoop(true);
	pDeadBGM->GetComponent<CSound>()->Play();
	pDeadBGM->GetComponent<CSound>()->SetVolume(0.0f);
	pDeadBGM->GetComponent<CSound>()->FadeIn(0.01f, 0.5f);

	// 上の背景を作成する
	m_pUpBG = new GameObject;
	m_pUpBG->SetPriority(18);
	m_pUpBG->AddComponent<CPolygon>();
	m_pUpBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(128, 25, 25, 255));
	m_pUpBG->transform->SetSize(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT / 2);
	m_pUpBG->transform->SetPos(0.0f, -CRenderer::SCREEN_HEIGHT / 2);

	// 下の背景を作成する
	m_pDownBG = new GameObject;
	m_pDownBG->SetPriority(18);
	m_pDownBG->AddComponent<CPolygon>();
	m_pDownBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(128, 25, 25, 255));
	m_pDownBG->transform->SetSize(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT / 2);
	m_pDownBG->transform->SetPos(0.0f, CRenderer::SCREEN_HEIGHT);

	// 左の背景を作成する
	m_pLeftBG = new GameObject;
	m_pLeftBG->SetPriority(18);
	m_pLeftBG->AddComponent<CPolygon>();
	m_pLeftBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(128, 25, 25, 255));
	m_pLeftBG->transform->SetSize(CRenderer::SCREEN_WIDTH / 2, CRenderer::SCREEN_HEIGHT);
	m_pLeftBG->transform->SetPos(-CRenderer::SCREEN_WIDTH / 2, 0.0f);

	// 右の背景を作成する
	m_pRightBG = new GameObject;
	m_pRightBG->SetPriority(18);
	m_pRightBG->AddComponent<CPolygon>();
	m_pRightBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(128, 25, 25, 255));
	m_pRightBG->transform->SetSize(CRenderer::SCREEN_WIDTH / 2, CRenderer::SCREEN_HEIGHT);
	m_pRightBG->transform->SetPos(CRenderer::SCREEN_WIDTH, 0.0f);

	// 死亡テキスト
	GameObject* pDeadText = new GameObject;
	pDeadText->SetPriority(19);
	pDeadText->transform->SetPos(CRenderer::SCREEN_WIDTH / 2, 230.0f, 0.0f);
	pDeadText->AddComponent<CTypingText>();
	pDeadText->GetComponent<CTypingText>()->SetTypingSpeed(10);
	pDeadText->GetComponent<CTypingText>()->SetFontSize(180);
	pDeadText->GetComponent<CTypingText>()->SetAlign(CText::ALIGN::CENTER);
	pDeadText->GetComponent<CTypingText>()->SetFont("07鉄瓶ゴシック");
	pDeadText->GetComponent<CTypingText>()->SetText("消滅してしまった");

	// リトライ
	m_pRetry = new GameObject;
	m_pRetry->SetPriority(19);
	m_pRetry->AddComponent<CPolygon>();
	m_pRetry->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\DEAD\\retry.png");
	m_pRetry->transform->SetSize(540, 160);
	m_pRetry->transform->SetPos(300.0f, 700.0f);

	// リトライアイコン
	m_pRetryIcon = new GameObject;
	m_pRetryIcon->SetParent(m_pRetry);
	m_pRetryIcon->SetPriority(20);
	m_pRetryIcon->transform->SetPos(95.0f, 80.0f);
	m_pRetryIcon->AddComponent<CPolygon>();
	m_pRetryIcon->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\DEAD\\retry_icon.png");
	m_pRetryIcon->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	m_pRetryIcon->GetComponent<CPolygon>()->SetAnchorPoint({ 130 / 2, 130 / 2 });
	m_pRetryIcon->transform->SetSize(130, 130);

	// ホーム
	m_pHome = new GameObject;
	m_pHome->SetPriority(19);
	m_pHome->AddComponent<CPolygon>();
	m_pHome->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\DEAD\\home.png");
	m_pHome->transform->SetSize(540, 160);
	m_pHome->transform->SetPos(CRenderer::SCREEN_WIDTH - 300.0f - 540.0f, 700.0f);

	// ホームアイコン
	m_pHomeIcon = new GameObject;
	m_pHomeIcon->SetParent(m_pHome);
	m_pHomeIcon->SetPriority(20);
	m_pHomeIcon->transform->SetPos(95.0f, 80.0f);
	m_pHomeIcon->AddComponent<CPolygon>();
	m_pHomeIcon->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\DEAD\\home_icon.png");
	m_pHomeIcon->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	m_pHomeIcon->GetComponent<CPolygon>()->SetAnchorPoint({ 110 / 2, 110 / 2 });
	m_pHomeIcon->transform->SetSize(110, 110);

	// カーソルを生成する
	m_pCursor = new CCursor();
	m_pCursor->Init();
	m_pCursor->SetPriority(21);
}

//=============================================================
// [CDeadScene] 終了
//=============================================================
void CDeadScene::Uninit()
{
	// カーソルの破棄
	if (m_pCursor != nullptr)
	{
		m_pCursor->Uninit();
		delete m_pCursor;
		m_pCursor = nullptr;
	}
}

//=============================================================
// [CDeadScene] 更新
//=============================================================
void CDeadScene::Update()
{
	// カーソルの更新
	m_pCursor->Update();

	// 上下左右を閉める
	m_pUpBG->transform->Translate((D3DXVECTOR3(0.0f, 0.0f, 0.0f) - m_pUpBG->transform->GetWPos()) * 0.06f);
	m_pDownBG->transform->Translate((D3DXVECTOR3(0.0f, CRenderer::SCREEN_HEIGHT/2, 0.0f) - m_pDownBG->transform->GetWPos()) * 0.06f);
	m_pLeftBG->transform->Translate((D3DXVECTOR3(0.0f, 0.0f, 0.0f) - m_pLeftBG->transform->GetWPos()) * 0.06f);
	m_pRightBG->transform->Translate((D3DXVECTOR3(CRenderer::SCREEN_WIDTH / 2, 0.0f, 0.0f) - m_pRightBG->transform->GetWPos()) * 0.06f);


	// リトライ
	if (m_pRetry->transform->GetWPos().x <= m_pCursor->GetPos().x &&
		m_pCursor->GetPos().x <= m_pRetry->transform->GetWPos().x + 540.0f &&
		m_pRetry->transform->GetWPos().y <= m_pCursor->GetPos().y &&
		m_pCursor->GetPos().y <= m_pRetry->transform->GetWPos().y + 160.0f)
	{ // カーソルが上にあるとき
		if (INPUT_INSTANCE->onInput("click"))
		{ // クリックしたとき
			CDeadScene::Uninit();
			CSceneManager::GetInstance()->GetScene("game")->pScene->Uninit();
			GameObject::DestroyAll();
			GameObject::DestroyDeathFlag();
			CSceneManager::GetInstance()->SetScene("game");
			return;
		}

		// アイコンを回転させる
		m_pRetryIcon->transform->SetRot(m_pRetryIcon->transform->GetRot().z + (D3DX_PI * 2 - m_pRetryIcon->transform->GetRot().z) * 0.08f);

		// 色を変える
		m_pRetry->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(160, 160, 160, 255));
	}
	else
	{ // カーソルが上にないとき
		// アイコンを元の状態に戻す
		m_pRetryIcon->transform->SetRot(m_pRetryIcon->transform->GetRot().z + (0.0f - m_pRetryIcon->transform->GetRot().z) * 0.2f);

		// 色を変える
		m_pRetry->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	}


	// ホーム
	if (m_pHome->transform->GetWPos().x <= m_pCursor->GetPos().x &&
		m_pCursor->GetPos().x <= m_pHome->transform->GetWPos().x + 540.0f &&
		m_pHome->transform->GetWPos().y <= m_pCursor->GetPos().y &&
		m_pCursor->GetPos().y <= m_pHome->transform->GetWPos().y + 160.0f)
	{ // カーソルが上にあるとき
		if (INPUT_INSTANCE->onInput("click"))
		{ // クリックしたとき
			CSceneManager::GetInstance()->SetScene("os");
			return;
		}

		// アイコンを回転させる
		D3DXVECTOR2 shakeHomeIcon = D3DXVECTOR2(rand() % 6 - 3, rand() % 6 - 3);
		m_pHomeIcon->transform->SetPos(95.0f + shakeHomeIcon.x, 80.0f + shakeHomeIcon.y);

		// 色を変える
		m_pHome->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(160, 160, 160, 255));
	}
	else
	{ // カーソルが上にないとき
		// アイコンを元の状態に戻す
		m_pHomeIcon->transform->SetPos(95.0f, 80.0f);

		// 色を変える
		m_pHome->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	}
}