//=============================================================
//
// �|�[�Y [pause.cpp]
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
// [CPause] ������
//=============================================================
void CPause::Init()
{
	// �ϐ��̏�����
	m_bIsPause = false;

	// �J�[�\��
	m_pCursor = new CCursor();
	m_pCursor->Init();
	m_pCursor->SetPriority(22);

	// �w�i
	m_pBG = new GameObject;
	m_pBG->SetPriority(20);
	m_pBG->AddComponent<CPolygon>();
	m_pBG->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(0, 0, 0, 0.9f));
	m_pBG->transform->SetSize(static_cast<float>(CRenderer::SCREEN_WIDTH), static_cast<float>(CRenderer::SCREEN_HEIGHT));

	// �|�[�Y�e�L�X�g
	m_pPauseText = new GameObject;
	m_pPauseText->SetPriority(21);
	m_pPauseText->AddComponent<CText>();
	m_pPauseText->GetComponent<CText>()->SetAlign(CText::ALIGN::CENTER);
	m_pPauseText->GetComponent<CText>()->SetFont("�x�X�g�e��-CRT");
	m_pPauseText->GetComponent<CText>()->SetText("<size=160>�|�[�Y");
	m_pPauseText->transform->SetPos(static_cast<float>(CRenderer::SCREEN_WIDTH/2), 180.0f);

	// ������
	m_pContinue = new GameObject;
	m_pContinue->SetPriority(21);
	m_pContinue->AddComponent<CPolygon>();
	m_pContinue->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\PAUSE\\continue.png");
	m_pContinue->transform->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	m_pContinue->transform->SetPos(static_cast<float>((CRenderer::SCREEN_WIDTH - BUTTON_WIDTH) / 2), 400.0f);

	// ���g���C
	m_pRetry = new GameObject;
	m_pRetry->SetPriority(21);
	m_pRetry->AddComponent<CPolygon>();
	m_pRetry->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\PAUSE\\retry.png");
	m_pRetry->transform->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	m_pRetry->transform->SetPos(static_cast<float>((CRenderer::SCREEN_WIDTH - BUTTON_WIDTH) / 2), 400.0f + static_cast<float>((BUTTON_HEIGHT + BUTTON_SPACE) * 1));

	// �z�[���֖߂�{�^��
	m_pBackHome = new GameObject;
	m_pBackHome->SetPriority(21);
	m_pBackHome->AddComponent<CPolygon>();
	m_pBackHome->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\PAUSE\\home.png");
	m_pBackHome->transform->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	m_pBackHome->transform->SetPos(static_cast<float>((CRenderer::SCREEN_WIDTH - BUTTON_WIDTH) / 2), static_cast<float>(400.0f + (BUTTON_HEIGHT + BUTTON_SPACE) * 2));

	// ��\��
	SetShow(m_bIsPause);
	SetPause(false);
}

//=============================================================
// [CPause] �I��
//=============================================================
void CPause::Uninit()
{
	// �J�[�\���̔j��
	if (m_pCursor != nullptr)
	{
		m_pCursor->Uninit();
		delete m_pCursor;
		m_pCursor = nullptr;
	}

	// �I�u�W�F�N�g�̔j��
	m_pBG->Destroy();
	m_pPauseTextBG->Destroy();
	m_pPauseText->Destroy();
	m_pContinue->Destroy();
	m_pRetry->Destroy();
	m_pBackHome->Destroy();
}

//=============================================================
// [CPause] �X�V
//=============================================================
void CPause::Update()
{
	// �Q�[���V�[�����擾����
	CGameScene* pGameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);

	// �S�[�����Ă���Ƃ��͏������Ȃ�
	if (pGameScene->IsGoal())
		return;

	// �|�[�Y���̏���
	if (m_bIsPause)
	{
		// �������Ԃ̌��ʉ�������Ă���Ƃ��͒�~����
		if (pGameScene->GetClockSound()->GetComponent<CSound>()->GetState() == CSound::PLAYING)
		{
			pGameScene->GetClockSound()->GetComponent<CSound>()->Stop();
		}

		// �J�[�\���̍X�V
		m_pCursor->Update();

		// ������
		if (m_pContinue->transform->GetPos().x <= m_pCursor->GetPos().x &&
			m_pCursor->GetPos().x <= m_pContinue->transform->GetPos().x + BUTTON_WIDTH &&
			m_pContinue->transform->GetPos().y <= m_pCursor->GetPos().y &&
			m_pCursor->GetPos().y <= m_pContinue->transform->GetPos().y + BUTTON_HEIGHT)
		{
			if (INPUT_INSTANCE->onInput("click"))
			{ // �N���b�N���ꂽ�Ƃ�
				SetPause(false);
			}

			m_pContinue->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(120, 120, 120, 255));
		}
		else
		{
			m_pContinue->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
		}

		// ���g���C
		if (m_pRetry->transform->GetPos().x <= m_pCursor->GetPos().x &&
			m_pCursor->GetPos().x <= m_pRetry->transform->GetPos().x + BUTTON_WIDTH &&
			m_pRetry->transform->GetPos().y <= m_pCursor->GetPos().y &&
			m_pCursor->GetPos().y <= m_pRetry->transform->GetPos().y + BUTTON_HEIGHT)
		{
			if (INPUT_INSTANCE->onInput("click"))
			{ // �N���b�N���ꂽ�Ƃ�
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

		// �z�[���֖߂�
		if (m_pBackHome->transform->GetPos().x <= m_pCursor->GetPos().x &&
			m_pCursor->GetPos().x <= m_pBackHome->transform->GetPos().x + BUTTON_WIDTH &&
			m_pBackHome->transform->GetPos().y <= m_pCursor->GetPos().y &&
			m_pCursor->GetPos().y <= m_pBackHome->transform->GetPos().y + BUTTON_HEIGHT)
		{
			if (INPUT_INSTANCE->onInput("click"))
			{ // �N���b�N���ꂽ�Ƃ�
				CSceneManager::GetInstance()->SetScene("os");
			}

			m_pBackHome->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(120, 120, 120, 255));
		}
		else
		{
			m_pBackHome->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
		}
	}

	// �|�[�Y�L�[�������ꂽ�Ƃ�
	if (INPUT_INSTANCE->onInput("pause"))
	{
		SetPause(!m_bIsPause);
	}
}

//=============================================================
// [CPause] �|�[�Y�̐ݒ�
//=============================================================
void CPause::SetPause(const bool& bPause)
{
	m_bIsPause = bPause;
	CManager::SetPause(bPause);
	SetShow(m_bIsPause);
}

//=============================================================
// [CPause] �|�[�Y�̕\��
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