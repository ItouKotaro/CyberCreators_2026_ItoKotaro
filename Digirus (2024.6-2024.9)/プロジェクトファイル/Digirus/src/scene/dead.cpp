//=============================================================
//
// ���S��� [dead.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "dead.h"
#include "component/2d/polygon.h"
#include "component/2d/text.h"
#include "component/other/sound.h"
#include "manager.h"

//=============================================================
// [CDeadScene] ������
//=============================================================
void CDeadScene::Init()
{
	// �ϐ��̏�����
	m_fAlpha = 1.0f;
	m_fRollingRetryIcon = 0.0f;

	// ����炷
	GameObject* pDeadSound = new GameObject;
	pDeadSound->AddComponent<CSound>()->LoadWav("data\\SOUND\\SE\\dead.wav");
	pDeadSound->GetComponent<CSound>()->IsStoppedDestroy();
	pDeadSound->GetComponent<CSound>()->Play();
	pDeadSound->GetComponent<CSound>()->SetVolume(50.0f);

	// ����炷
	GameObject* pDeadBGM = new GameObject;
	pDeadBGM->AddComponent<CSound>()->LoadWav("data\\SOUND\\BGM\\dead.wav");
	pDeadBGM->GetComponent<CSound>()->SetLoop(true);
	pDeadBGM->GetComponent<CSound>()->Play();
	pDeadBGM->GetComponent<CSound>()->SetVolume(0.0f);
	pDeadBGM->GetComponent<CSound>()->FadeIn(0.01f, 0.5f);

	// ��̔w�i���쐬����
	m_pUpBG = new GameObject;
	m_pUpBG->SetPriority(18);
	m_pUpBG->AddComponent<CPolygon>();
	m_pUpBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(128, 25, 25, 255));
	m_pUpBG->transform->SetSize(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT / 2);
	m_pUpBG->transform->SetPos(0.0f, -CRenderer::SCREEN_HEIGHT / 2);

	// ���̔w�i���쐬����
	m_pDownBG = new GameObject;
	m_pDownBG->SetPriority(18);
	m_pDownBG->AddComponent<CPolygon>();
	m_pDownBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(128, 25, 25, 255));
	m_pDownBG->transform->SetSize(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT / 2);
	m_pDownBG->transform->SetPos(0.0f, CRenderer::SCREEN_HEIGHT);

	// ���̔w�i���쐬����
	m_pLeftBG = new GameObject;
	m_pLeftBG->SetPriority(18);
	m_pLeftBG->AddComponent<CPolygon>();
	m_pLeftBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(128, 25, 25, 255));
	m_pLeftBG->transform->SetSize(CRenderer::SCREEN_WIDTH / 2, CRenderer::SCREEN_HEIGHT);
	m_pLeftBG->transform->SetPos(-CRenderer::SCREEN_WIDTH / 2, 0.0f);

	// �E�̔w�i���쐬����
	m_pRightBG = new GameObject;
	m_pRightBG->SetPriority(18);
	m_pRightBG->AddComponent<CPolygon>();
	m_pRightBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(128, 25, 25, 255));
	m_pRightBG->transform->SetSize(CRenderer::SCREEN_WIDTH / 2, CRenderer::SCREEN_HEIGHT);
	m_pRightBG->transform->SetPos(CRenderer::SCREEN_WIDTH, 0.0f);

	// ���S�e�L�X�g
	GameObject* pDeadText = new GameObject;
	pDeadText->SetPriority(19);
	pDeadText->transform->SetPos(CRenderer::SCREEN_WIDTH / 2, 230.0f, 0.0f);
	pDeadText->AddComponent<CTypingText>();
	pDeadText->GetComponent<CTypingText>()->SetTypingSpeed(10);
	pDeadText->GetComponent<CTypingText>()->SetFontSize(180);
	pDeadText->GetComponent<CTypingText>()->SetAlign(CText::ALIGN::CENTER);
	pDeadText->GetComponent<CTypingText>()->SetFont("07�S�r�S�V�b�N");
	pDeadText->GetComponent<CTypingText>()->SetText("���ł��Ă��܂���");

	// ���g���C
	m_pRetry = new GameObject;
	m_pRetry->SetPriority(19);
	m_pRetry->AddComponent<CPolygon>();
	m_pRetry->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\DEAD\\retry.png");
	m_pRetry->transform->SetSize(540, 160);
	m_pRetry->transform->SetPos(300.0f, 700.0f);

	// ���g���C�A�C�R��
	m_pRetryIcon = new GameObject;
	m_pRetryIcon->SetParent(m_pRetry);
	m_pRetryIcon->SetPriority(20);
	m_pRetryIcon->transform->SetPos(95.0f, 80.0f);
	m_pRetryIcon->AddComponent<CPolygon>();
	m_pRetryIcon->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\DEAD\\retry_icon.png");
	m_pRetryIcon->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	m_pRetryIcon->GetComponent<CPolygon>()->SetAnchorPoint({ 130 / 2, 130 / 2 });
	m_pRetryIcon->transform->SetSize(130, 130);

	// �z�[��
	m_pHome = new GameObject;
	m_pHome->SetPriority(19);
	m_pHome->AddComponent<CPolygon>();
	m_pHome->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\DEAD\\home.png");
	m_pHome->transform->SetSize(540, 160);
	m_pHome->transform->SetPos(CRenderer::SCREEN_WIDTH - 300.0f - 540.0f, 700.0f);

	// �z�[���A�C�R��
	m_pHomeIcon = new GameObject;
	m_pHomeIcon->SetParent(m_pHome);
	m_pHomeIcon->SetPriority(20);
	m_pHomeIcon->transform->SetPos(95.0f, 80.0f);
	m_pHomeIcon->AddComponent<CPolygon>();
	m_pHomeIcon->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\DEAD\\home_icon.png");
	m_pHomeIcon->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	m_pHomeIcon->GetComponent<CPolygon>()->SetAnchorPoint({ 110 / 2, 110 / 2 });
	m_pHomeIcon->transform->SetSize(110, 110);

	// �J�[�\���𐶐�����
	m_pCursor = new CCursor();
	m_pCursor->Init();
	m_pCursor->SetPriority(21);
}

//=============================================================
// [CDeadScene] �I��
//=============================================================
void CDeadScene::Uninit()
{
	// �J�[�\���̔j��
	if (m_pCursor != nullptr)
	{
		m_pCursor->Uninit();
		delete m_pCursor;
		m_pCursor = nullptr;
	}
}

//=============================================================
// [CDeadScene] �X�V
//=============================================================
void CDeadScene::Update()
{
	// �J�[�\���̍X�V
	m_pCursor->Update();

	// �㉺���E��߂�
	m_pUpBG->transform->Translate((D3DXVECTOR3(0.0f, 0.0f, 0.0f) - m_pUpBG->transform->GetWPos()) * 0.06f);
	m_pDownBG->transform->Translate((D3DXVECTOR3(0.0f, CRenderer::SCREEN_HEIGHT/2, 0.0f) - m_pDownBG->transform->GetWPos()) * 0.06f);
	m_pLeftBG->transform->Translate((D3DXVECTOR3(0.0f, 0.0f, 0.0f) - m_pLeftBG->transform->GetWPos()) * 0.06f);
	m_pRightBG->transform->Translate((D3DXVECTOR3(CRenderer::SCREEN_WIDTH / 2, 0.0f, 0.0f) - m_pRightBG->transform->GetWPos()) * 0.06f);


	// ���g���C
	if (m_pRetry->transform->GetWPos().x <= m_pCursor->GetPos().x &&
		m_pCursor->GetPos().x <= m_pRetry->transform->GetWPos().x + 540.0f &&
		m_pRetry->transform->GetWPos().y <= m_pCursor->GetPos().y &&
		m_pCursor->GetPos().y <= m_pRetry->transform->GetWPos().y + 160.0f)
	{ // �J�[�\������ɂ���Ƃ�
		if (INPUT_INSTANCE->onInput("click"))
		{ // �N���b�N�����Ƃ�
			CDeadScene::Uninit();
			CSceneManager::GetInstance()->GetScene("game")->pScene->Uninit();
			GameObject::DestroyAll();
			GameObject::DestroyDeathFlag();
			CSceneManager::GetInstance()->SetScene("game");
			return;
		}

		// �A�C�R������]������
		m_pRetryIcon->transform->SetRot(m_pRetryIcon->transform->GetRot().z + (D3DX_PI * 2 - m_pRetryIcon->transform->GetRot().z) * 0.08f);

		// �F��ς���
		m_pRetry->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(160, 160, 160, 255));
	}
	else
	{ // �J�[�\������ɂȂ��Ƃ�
		// �A�C�R�������̏�Ԃɖ߂�
		m_pRetryIcon->transform->SetRot(m_pRetryIcon->transform->GetRot().z + (0.0f - m_pRetryIcon->transform->GetRot().z) * 0.2f);

		// �F��ς���
		m_pRetry->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	}


	// �z�[��
	if (m_pHome->transform->GetWPos().x <= m_pCursor->GetPos().x &&
		m_pCursor->GetPos().x <= m_pHome->transform->GetWPos().x + 540.0f &&
		m_pHome->transform->GetWPos().y <= m_pCursor->GetPos().y &&
		m_pCursor->GetPos().y <= m_pHome->transform->GetWPos().y + 160.0f)
	{ // �J�[�\������ɂ���Ƃ�
		if (INPUT_INSTANCE->onInput("click"))
		{ // �N���b�N�����Ƃ�
			CSceneManager::GetInstance()->SetScene("os");
			return;
		}

		// �A�C�R������]������
		D3DXVECTOR2 shakeHomeIcon = D3DXVECTOR2(rand() % 6 - 3, rand() % 6 - 3);
		m_pHomeIcon->transform->SetPos(95.0f + shakeHomeIcon.x, 80.0f + shakeHomeIcon.y);

		// �F��ς���
		m_pHome->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(160, 160, 160, 255));
	}
	else
	{ // �J�[�\������ɂȂ��Ƃ�
		// �A�C�R�������̏�Ԃɖ߂�
		m_pHomeIcon->transform->SetPos(95.0f, 80.0f);

		// �F��ς���
		m_pHome->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	}
}