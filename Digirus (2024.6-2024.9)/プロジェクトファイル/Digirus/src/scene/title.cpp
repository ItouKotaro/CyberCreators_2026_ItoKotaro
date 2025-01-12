//=============================================================
//
// �^�C�g�� [title.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "title.h"
#include "component/2d/polygon.h"
#include "manager.h"

// �ÓI�����o�̏�����
const std::string CTitleScene::IMG_BG = "data\\TEXTURE\\TITLE\\bg.png";
const std::string CTitleScene::IMG_START = "data\\TEXTURE\\TITLE\\start.png";
const std::string CTitleScene::IMG_SETTING = "data\\TEXTURE\\TITLE\\setting.png";
const std::string CTitleScene::IMG_END = "data\\TEXTURE\\TITLE\\end.png";

const D3DXVECTOR2 CTitleScene::CHOICE_POS = D3DXVECTOR2(140.0f, 650.0f);
const float CTitleScene::CHOICE_WIDTH = 850.0f;
const float CTitleScene::CHOICE_HEIGHT = 100.0f;
const float CTitleScene::CHOICE_SPACE = 130.0f;
const float CTitleScene::CHOICED_ALPHA = 0.4f;

const D3DXVECTOR2 CTitleScene::BAR_SIZE = D3DXVECTOR2(10.0f, 110.0f);
const float CTitleScene::BAR_WIDTHPOS = 100.0f;

//=============================================================
// [CTitleScene] ������
//=============================================================
void CTitleScene::Init()
{
	m_pBG = new GameObject();
	m_pBG->AddComponent<CPolygon>(IMG_BG);
	m_pBG->transform->SetSize(1920.0f, 1080.0f);
	m_pBG->transform->SetPos(0.0f, 0.0f);

	// �I�����̏�����
	InitChoices();
}

//=============================================================
// [CTitleScene] �I��
//=============================================================
void CTitleScene::Uninit()
{

}

//=============================================================
// [CTitleScene] �X�V
//=============================================================
void CTitleScene::Update()
{
	// �I���̍X�V
	UpdateChoices();

	// �I�����ɂ�鏈��
	if (INPUT_INSTANCE->onInput("select_enter"))
	{
		switch (m_choice)
		{
		case CTitleScene::START:
			CSceneManager::GetInstance()->SetScene("os");
			break;
		case CTitleScene::SETTING:
			break;
		case CTitleScene::END:
			DestroyWindow(CManager::GetRenderer()->GetHWND());
			break;
		}
	}

}

//=============================================================
// [CTitleScene] �`��
//=============================================================
void CTitleScene::Draw()
{

}

//=============================================================
// [CTitleScene] �I�����̏�����
//=============================================================
void CTitleScene::InitChoices()
{
	// �ϐ�
	m_choice = CHOICE::START;

	// �J�n����
	m_pStartChoice = new GameObject();
	m_pStartChoice->AddComponent<CPolygon>(IMG_START);
	m_pStartChoice->transform->SetSize(CHOICE_WIDTH, CHOICE_HEIGHT);
	m_pStartChoice->transform->SetPos(CHOICE_POS);

	// �ݒ�
	m_pSettingChoice = new GameObject();
	m_pSettingChoice->AddComponent<CPolygon>(IMG_SETTING);
	m_pSettingChoice->transform->SetSize(CHOICE_WIDTH, CHOICE_HEIGHT);
	m_pSettingChoice->transform->SetPos(CHOICE_POS + D3DXVECTOR2(0.0f, CHOICE_SPACE));

	// �I������
	m_pEndChoice = new GameObject();
	m_pEndChoice->AddComponent<CPolygon>(IMG_END);
	m_pEndChoice->transform->SetSize(CHOICE_WIDTH, CHOICE_HEIGHT);
	m_pEndChoice->transform->SetPos(CHOICE_POS + D3DXVECTOR2(0.0f, CHOICE_SPACE * 2));

	// �I���o�[
	m_pSelectBar = new GameObject();
	m_pSelectBar->AddComponent<CPolygon>();
	m_pSelectBar->transform->SetSize(BAR_SIZE);
	m_pSelectBar->transform->SetPos(BAR_WIDTHPOS, CHOICE_POS.y);
}

//=============================================================
// [CTitleScene] �I����Ԃ̍X�V
//=============================================================
void CTitleScene::UpdateChoices()
{
	// �I��
	if (INPUT_INSTANCE->onInput("select_up"))
	{ // ��
		if (m_choice > CHOICE::START)
		{ // �X�^�[�g��艺�ɂ���Ƃ�
			m_choice = (CHOICE)(m_choice - 1);
		}
	}
	if (INPUT_INSTANCE->onInput("select_down"))
	{ // ��
		if (m_choice < CHOICE::END)
		{ // �I������ɂ���Ƃ�
			m_choice = (CHOICE)(m_choice + 1);
		}
	}

	// �I����Ԃɂ���ē����x�̕ύX
	if (m_choice == CHOICE::START)
		m_pStartChoice->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	else
		m_pStartChoice->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, CHOICED_ALPHA));

	if (m_choice == CHOICE::SETTING)
		m_pSettingChoice->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	else
		m_pSettingChoice->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, CHOICED_ALPHA));
	
	if (m_choice == CHOICE::END)
		m_pEndChoice->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	else
		m_pEndChoice->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, CHOICED_ALPHA));

	// �o�[�̈ʒu
	m_pSelectBar->transform->SetPos(BAR_WIDTHPOS, CHOICE_POS.y + CHOICE_SPACE*m_choice);
}