//=============================================================
//
// OS [operation_system.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "operation_system.h"
#include "component/2d/polygon.h"
#include "component/2d/text.h"
#include "component/other/sound.h"
#include "game.h"
#include "manager.h"
#include "benlib.h"
#include <fstream>

CWindowObject* CWindowObject::m_pDragging = nullptr;
CWindowObject* CWindowObject::m_pTopWindow = nullptr;
CFileListContent* COperationSystem::m_pSelectedFileContent = nullptr;
std::vector<CWindowObject*> CWindowObject::m_pWindowObjects;

const int CWindowObject::TITLEBAR_HEIGHT = 50;
const int CWindowObject::CLOSEBUTTON_SIZE = 50;

const int CCloseWindow::BUTTON_WIDTH = 156;
const int CCloseWindow::BUTTON_HEIGHT = 70;
const int CWorldWindow::BUTTON_WIDTH = 156;
const int CWorldWindow::BUTTON_HEIGHT = 70;
const int COperationSystem::TABBAR_WIDTH = 350;
const int COperationSystem::TABBAR_HEIGHT = 100;
const int COperationSystem::LOGO_SIZE = 200;
const int COperationSystem::PLAYBAR_WIDTH = 1350;
const int COperationSystem::PLAYBAR_HEIGHT = 600;
const int COperationSystem::POWER_SIZE = 130;
const int COperationSystem::GUIDE_WIDTH = 800;
const int COperationSystem::GUIDE_HEIGHT = 90;
const int CFileListContent::FILELIST_WIDTH = 800;
const int CFileListContent::FILELIST_HEIGHT = 100;
const int CFileListContent::FILELIST_EDGE = 4;
const int CFileListContent::FILELIST_ICONSIZE = 70;
const int CFileListContent::DOUBLECLICK_TIME = 20;
const char* COperationSystem::LOAD_PATH = "data\\WORLD\\";
const char* COperationSystem::INFO_FILENAME = "info.json";
const char* COperationSystem::WORLD_FILENAME = "world.wdit";
const std::string COperationSystem::BACK_NAME = "�߂�";

//=============================================================
// [COperationSystem] ������
//=============================================================
void COperationSystem::Init()
{
	m_selectTab = SELECT_TAB::HOME;
	strcpy(&m_sCurrentDirectory[0], LOAD_PATH);
	
	// BGM
	GameObject* pBGM = new GameObject;
	pBGM->AddComponent<CSound>();
	pBGM->GetComponent<CSound>()->LoadWav("data\\SOUND\\BGM\\title.wav");
	pBGM->GetComponent<CSound>()->SetVolume(0.9f);
	pBGM->GetComponent<CSound>()->Play();
	pBGM->GetComponent<CSound>()->SetLoop(true);

	// �J�[�\��
	m_pCursor = new CCursor;
	m_pCursor->Init();
	m_pCursor->SetPriority(16);

	// �w�i
	GameObject* pBG = new GameObject();
	pBG->SetPriority(2);
	pBG->transform->SetSize(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT);
	pBG->AddComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));

	// ���S
	m_pLogoObj = new GameObject;
	m_pLogoObj->transform->SetPos((TABBAR_WIDTH - LOGO_SIZE) / 2, 30.0f);
	m_pLogoObj->transform->SetSize(LOGO_SIZE, LOGO_SIZE);
	CPolygon* pLogo = m_pLogoObj->AddComponent<CPolygon>();
	pLogo->SetTexture("data\\TEXTURE\\HOME\\logo0.png");
	m_nLogoCounter = 0;
	m_nLogoType = 0;

	// �d���{�^��
	m_pPowerObj = new GameObject;
	m_pPowerObj->transform->SetPos(10.0f, CRenderer::SCREEN_HEIGHT - POWER_SIZE - 20.0f);
	m_pPowerObj->transform->SetSize(POWER_SIZE, POWER_SIZE);
	CPolygon* pPowerIcon = m_pPowerObj->AddComponent<CPolygon>();
	pPowerIcon->SetTexture("data\\TEXTURE\\HOME\\power_icon.png");

	// �K�C�h�w�i
	GameObject* pGuideBG = new GameObject;
	pGuideBG->SetPriority(10);
	pGuideBG->transform->SetSize(CRenderer::SCREEN_WIDTH - TABBAR_WIDTH, GUIDE_HEIGHT);
	pGuideBG->transform->SetPos(TABBAR_WIDTH + 2.0f, CRenderer::SCREEN_HEIGHT - GUIDE_HEIGHT);
	pGuideBG->AddComponent<CPolygon>()->SetColor(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));

	// �K�C�h�\��
	m_pGuide = new GameObject;
	m_pGuide->SetPriority(11);
	m_pGuide->transform->SetSize(GUIDE_WIDTH, GUIDE_HEIGHT);
	m_pGuide->transform->SetPos(CRenderer::SCREEN_WIDTH - GUIDE_WIDTH, CRenderer::SCREEN_HEIGHT - GUIDE_HEIGHT - 5.0f);
	m_pGuide->AddComponent<CPolygon>();

	// �I��
	m_nScroll = 0;
	m_selectTabObj = new GameObject;
	m_selectTabObj->SetPriority(5);
	m_selectTabObj->transform->SetPos(0.0f, 300.0f);
	m_selectTabObj->transform->SetSize(TABBAR_WIDTH, TABBAR_HEIGHT);
	CPolygon* selectTabPoly = m_selectTabObj->AddComponent<CPolygon>();
	selectTabPoly->SetColor(D3DCOLOR_RGBA(0, 100, 0, 255));

	GameObject* selectTabBar = new GameObject;
	selectTabBar->SetParent(m_selectTabObj);
	selectTabBar->transform->SetSize(20.0f, 100.0f);
	selectTabBar->AddComponent<CPolygon>();

	// �^�u�w�i
	GameObject* pTagBGObj = new GameObject;
	pTagBGObj->SetPriority(4);
	pTagBGObj->transform->SetSize(TABBAR_WIDTH, CRenderer::SCREEN_HEIGHT);
	pTagBGObj->AddComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(16, 23, 22, 255));

	// �^�u�Ƃ̎d�؂�
	GameObject* pPartition = new GameObject;
	pPartition->transform->SetPos(TABBAR_WIDTH, 0.0f);
	pPartition->transform->SetSize(2.0f, CRenderer::SCREEN_HEIGHT);
	CPolygon* pPartitionPoly = pPartition->AddComponent<CPolygon>();
	pPartitionPoly->SetColor(D3DCOLOR_RGBA(200, 200, 200, 255));

	// �^�u�̍��ڂ𐶐�����
	for (int i = 0; i < SELECT_TAB::MAX; i++)
	{
		GameObject* pTabObj = new GameObject;
		pTabObj->transform->SetPos(90.0f, 318.0f + i * 120.0f);
		CText* pTabText = pTabObj->AddComponent<CText>();
		pTabText->SetFont("�x�X�g�e��-CRT");
		pTabText->SetFontSize(70);

		switch (i)
		{
		case 0:
			pTabText->SetText("�z�[��");
			break;
		case 1:
			pTabText->SetText("�v���C");
			break;
		}
	}
	

	// �^�u�̏�����
	InitHomeTab();
	InitPlayTab();

	// ���ׂẴ^�u���\���ɂ���
	std::vector<GameObject*> pAllObjects = GameObject::GetAllGameObjects();
	for (int i = 0; i < pAllObjects.size(); i++)
	{
		if (pAllObjects[i]->GetTag() == "play" ||
			pAllObjects[i]->GetTag() == "file")
		{
			pAllObjects[i]->SetActive(false);
		}
	}
}

//=============================================================
// [COperationSystem] �I��
//=============================================================
void COperationSystem::Uninit()
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
// [COperationSystem] �X�V
//=============================================================
void COperationSystem::Update()
{
	// �^�u�̍X�V
	UpdateTab();

	// �J�[�\���̍X�V
	m_pCursor->Update();

	// �K�C�h
	if (m_pCursor->GetCursorDevice() == CursorDevice::MOUSE)
	{ // �}�E�X
		m_pGuide->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\HOME\\guide_mouse.png");
	}
	else
	{ // �R���g���[���[
		m_pGuide->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\HOME\\guide_control.png");
	}

	// ���S�`�F���W���[
	m_nLogoCounter--;
	if (m_nLogoCounter <= 0)
	{
		srand((unsigned int)clock());
		m_pLogoObj->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\HOME\\logo" + std::to_string(m_nLogoType) + ".png");

		if (m_nLogoType == 0)
		{
			m_nLogoType = 1;
			m_nLogoCounter = 60 + rand() % 400;
		}
		else
		{
			m_nLogoType = 0;
			m_nLogoCounter = 5 + rand() % 40;
		}
	}
}

//=============================================================
// [COperationSystem] �`��
//=============================================================
void COperationSystem::Draw()
{

}

//=============================================================
// [COperationSystem] �^�u�̍X�V����
//=============================================================
void COperationSystem::UpdateTab()
{
	SELECT_TAB oldTab = m_selectTab;

	// �I���̕ύX�i�R���g���[���[�̏\���L�[�̂݁j
	if (INPUT_INSTANCE->onInput("select_up") &&
		0 < m_selectTab)
	{
		m_selectTab = static_cast<SELECT_TAB>(m_selectTab - 1);
	}
	if (INPUT_INSTANCE->onInput("select_down") &&
		m_selectTab + 1 < SELECT_TAB::MAX)
	{
		m_selectTab = static_cast<SELECT_TAB>(m_selectTab + 1);
	}

	// �N���b�N���ꂽ�Ƃ���ɉ����ă^�u��؂�ւ���
	for (int i = 0; i < SELECT_TAB::MAX; i++)
	{
		// �E�B���h�E����O�ɂ���Ƃ�
		if (CWindowObject::GetSelectedAllArea(m_pCursor->GetPos()))
		{
			break;	// �������X�L�b�v
		}

		if (0.0f <= m_pCursor->GetPos().x && m_pCursor->GetPos().x <= TABBAR_WIDTH &&
			295.0f + 120.0f * i <= m_pCursor->GetPos().y && m_pCursor->GetPos().y <= 295.0f + 120.0f * i + TABBAR_HEIGHT &&
			INPUT_INSTANCE->onInput("click"))
		{
			m_selectTab = static_cast<SELECT_TAB>(i);
		}
	}

	// �d���{�^���̔���
	if (m_pPowerObj->transform->GetWPos().x <= m_pCursor->GetPos().x && m_pCursor->GetPos().x <= m_pPowerObj->transform->GetWPos().x + POWER_SIZE &&
		m_pPowerObj->transform->GetWPos().y <= m_pCursor->GetPos().y && m_pCursor->GetPos().y <= m_pPowerObj->transform->GetWPos().y + POWER_SIZE &&
		INPUT_INSTANCE->onInput("click"))
	{ // �d���{�^�����N���b�N�����Ƃ�
		GameObject* pPowerWindow = new GameObject;
		pPowerWindow->AddComponent<CCloseWindow>();
	}


	// �I���|���S���̈ړ�
	m_selectTabObj->transform->SetPos(0.0f, 295.0f + m_selectTab * 120.0f);
	m_selectTabObj->transform->SetSize(m_selectTabObj->transform->GetSize().x + (TABBAR_WIDTH - m_selectTabObj->transform->GetSize().x) * 0.1f, TABBAR_HEIGHT);


	// �^�u�̍X�V����
	switch (m_selectTab)
	{
	case COperationSystem::HOME:
		break;
	case COperationSystem::PLAY:
		UpdatePlayTab();
		break;
	case COperationSystem::MAX:
		break;
	}

	// �^�u���ς�����Ƃ��̏���
	if (oldTab != m_selectTab)
	{
		// ���ׂẴI�u�W�F�N�g���擾����
		std::vector<GameObject*> pAllObjects = GameObject::GetAllGameObjects();
		std::string sTabTag;

		// �؂�ւ��O�̃^�u���\���ɂ���
		switch (oldTab)
		{
		case COperationSystem::HOME:
			sTabTag = "home";
			break;
		case COperationSystem::PLAY:
			sTabTag = "play";
			break;
		}

		// ��\������
		for (int i = 0; i < pAllObjects.size(); i++)
		{
			if (pAllObjects[i]->GetTag() == sTabTag)
			{
				pAllObjects[i]->SetActive(false);
			}
		}

		// �V�����^�u��\������
		switch (m_selectTab)
		{
		case COperationSystem::HOME:
			sTabTag = "home";
			break;
		case COperationSystem::PLAY:
			sTabTag = "play";
			break;
		}

		// �\������
		for (int i = 0; i < pAllObjects.size(); i++)
		{
			if (pAllObjects[i]->GetTag() == sTabTag)
			{
				pAllObjects[i]->SetActive(true);
			}
		}

		// �t�@�C�����X�g�̓��ꏈ��
		for (int i = 0; i < pAllObjects.size(); i++)
		{
			if (pAllObjects[i]->GetTag() == "file")
			{
				pAllObjects[i]->SetActive(m_selectTab == COperationSystem::PLAY);
			}
		}

		// ����炷
		GameObject* pSound = new GameObject;
		pSound->AddComponent<CSound>();
		pSound->GetComponent<CSound>()->LoadWav("data\\SOUND\\SE\\click.wav");
		pSound->GetComponent<CSound>()->Play();
		pSound->GetComponent<CSound>()->IsStoppedDestroy();

		// �I���̃T�C�Y��ύX����
		m_selectTabObj->transform->SetSize(0.0f, TABBAR_HEIGHT);
	}
}

//=============================================================
// [COperationSystem] �z�[���^�u�̏���������
//=============================================================
void COperationSystem::InitHomeTab()
{
	// �e�L�X�g
	GameObject* pTitleObj = new GameObject();
	pTitleObj->SetTag("home");
	pTitleObj->transform->SetPos(570.0f, 110.0f);
	CTypingText* pTitleText = pTitleObj->AddComponent<CTypingText>();
	pTitleText->SetFont("�x�X�g�e��-CRT");
	pTitleText->SetText("Digirus �ւ悤����");
	pTitleText->SetFontSize(150);
	pTitleText->SetTypingSpeed(10);

	// �ڕW
	GameObject* pObjectiveBG = new GameObject();
	pObjectiveBG->SetTag("home");
	pObjectiveBG->transform->SetSize(200.0f, 100.0f);
	pObjectiveBG->transform->SetPos(480.0f, 414.0f);
	pObjectiveBG->AddComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(50, 50, 50, 255));

	GameObject* pObjectiveObj = new GameObject();
	pObjectiveObj->SetTag("home");
	pObjectiveObj->transform->SetPos(500.0f, 425.0f);
	CText* pObjectiveText = pObjectiveObj->AddComponent<CText>();
	pObjectiveText->SetFont("�x�X�g�e��-CRT");
	pObjectiveText->SetText("�ڕW");
	pObjectiveText->SetFontSize(100);


	pObjectiveObj = new GameObject();
	pObjectiveObj->SetTag("home");
	pObjectiveObj->transform->SetPos(730.0f, 440.0f);
	pObjectiveText = pObjectiveObj->AddComponent<CText>();
	pObjectiveText->SetFont("�x�X�g�e��-CRT");
	pObjectiveText->SetText("�U���Ώۂ̃f�o�C�X�ɐN�����A���[���ɐ��荞�����I");
	pObjectiveText->SetFontSize(55);

	pObjectiveObj = new GameObject();
	pObjectiveObj->SetTag("home");
	pObjectiveObj->transform->SetPos(470.0f, 700.0f);
	pObjectiveText = pObjectiveObj->AddComponent<CText>();
	pObjectiveText->SetFontColor(D3DCOLOR_RGBA(40, 118, 252, 255));
	pObjectiveText->SetEdgeColor(D3DCOLOR_RGBA(40, 118, 252, 255));
	pObjectiveText->SetFont("�x�X�g�e��-CRT");
	pObjectiveText->SetText("���̃v���C��I�����āA�X�e�[�W���_�u���N���b�N�I");
	pObjectiveText->SetFontSize(70);
}

//=============================================================
// [COperationSystem] �v���C�^�u�̏���������
//=============================================================
void COperationSystem::InitPlayTab()
{
	// �^�C�g��
	GameObject* pPlayTitleObj = new GameObject;
	pPlayTitleObj->SetPriority(10);
	pPlayTitleObj->SetTag("play");
	pPlayTitleObj->transform->SetPos(450.0f, 100.0f);
	CText* pPlayTitleText = pPlayTitleObj->AddComponent<CText>();
	pPlayTitleText->SetFont("�x�X�g�e��-CRT");
	pPlayTitleText->SetText("�u���E�U");
	pPlayTitleText->SetFontSize(140);

	// �^�C�g���w�i
	GameObject* pPlayTitleBGObj = new GameObject;
	pPlayTitleBGObj->SetPriority(9);
	pPlayTitleBGObj->SetTag("play");
	pPlayTitleBGObj->transform->SetPos(450.0f, 0.0f);
	pPlayTitleBGObj->transform->SetSize(900.0f, 300.0f);
	pPlayTitleBGObj->AddComponent<CPolygon>()->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	// �d�؂�
	GameObject* pPartition = new GameObject;
	pPartition->SetTag("play");
	pPartition->transform->SetPos(PLAYBAR_WIDTH,  320);
	pPartition->transform->SetSize(2.0f, PLAYBAR_HEIGHT);
	CPolygon* pPartitionPoly = pPartition->AddComponent<CPolygon>();
	pPartitionPoly->SetColor(D3DCOLOR_RGBA(200, 200, 200, 155));

	// �T���l�C��
	m_thumbnailObj = new GameObject;
	m_thumbnailObj->SetVisible(false);
	m_thumbnailObj->SetTag("play");
	m_thumbnailObj->transform->SetPos(1400.0f, 300.0f);
	m_thumbnailObj->transform->SetSize(465, 265);
	m_thumbnailObj->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\HOME\\no_thumbnail.png");

	// ��������
	m_limitTimeObj = new GameObject;
	m_limitTimeObj->SetVisible(false);
	m_limitTimeObj->SetTag("play");
	m_limitTimeObj->transform->SetPos(1400.0f, 600.0f);
	m_limitTimeObj->transform->SetSize(465, 265);
	m_limitTimeObj->AddComponent<CText>();
	m_limitTimeObj->GetComponent<CText>()->SetFontSize(50);
	m_limitTimeObj->GetComponent<CText>()->SetFont("�x�X�g�e��-CRT");

	// ���[���h��ǂݍ���
	LoadFiles();
}



//=============================================================
// [CFileListContent] ������
//=============================================================
void CFileListContent::Init()
{
	// �g�쐬
	m_pFileBG = new GameObject;
	m_pFileEdge = new GameObject;
	m_pFileBG->SetPriority(7);
	m_pFileEdge->SetPriority(6);
	m_pFileBG->SetTag("play");
	m_pFileEdge->SetTag("play");
	m_pFileBG->SetParent(gameObject);
	m_pFileEdge->SetParent(gameObject);
	m_pFileBG->AddComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));
	m_pFileEdge->AddComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	m_pFileBG->transform->SetSize(FILELIST_WIDTH - FILELIST_EDGE * 2, FILELIST_HEIGHT - FILELIST_EDGE * 2);
	m_pFileEdge->transform->SetSize(FILELIST_WIDTH, FILELIST_HEIGHT);
	m_pFileBG->transform->SetPos(FILELIST_EDGE, FILELIST_EDGE);

	// �^�C�g���쐬
	m_pFileTitle = new GameObject;
	m_pFileTitle->transform->SetPos(120.0f, 25.0f);
	m_pFileTitle->SetPriority(8);
	m_pFileTitle->SetTag("play");
	m_pFileTitle->SetParent(gameObject);
	CText* m_pFileTitleText = m_pFileTitle->AddComponent<CText>();
	m_pFileTitleText->SetFont("JF�h�b�gM+12");
	m_pFileTitleText->SetText("");
	m_pFileTitleText->SetFontSize(60);

	// �A�C�R��
	m_pFileIcon = new GameObject;
	m_pFileIcon->transform->SetSize(FILELIST_ICONSIZE, FILELIST_ICONSIZE);
	m_pFileIcon->transform->SetPos(20.0f, 15.0f);
	m_pFileIcon->SetPriority(8);
	m_pFileIcon->SetParent(gameObject);
	m_pFileIcon->SetTag("play");
	m_pFileIcon->AddComponent<CPolygon>();
}

//=============================================================
// [CFileListContent] �X�V
//=============================================================
void CFileListContent::Update()
{
	// �F��ς���
	if (COperationSystem::GetSelectedFileContent() == this)
	{ // �I������Ă���Ƃ�
		m_pFileBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(150, 150, 150, 255));
		m_pFileEdge->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(0, 255, 0, 255));
		m_pFileIcon->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));
		m_pFileTitle->GetComponent<CText>()->SetFontColor(D3DCOLOR_RGBA(0, 255, 0, 255));
	}
	else
	{ // �I������Ă��Ȃ��Ƃ�
		m_pFileBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));
		m_pFileEdge->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
		m_pFileIcon->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
		m_pFileTitle->GetComponent<CText>()->SetFontColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	}

	// �_�u���N���b�N����
	if (m_nDoubleCounter > 0)
	{
		m_nDoubleCounter--;
	}
}

//=============================================================
// [CFileListContent] �^�C�g����ݒ肷��
//=============================================================
void CFileListContent::SetTitle(const std::string& title)
{
	m_pFileTitle->GetComponent<CText>()->SetText(title);
	m_sName = title;
}

//=============================================================
// [CFileListContent] �^�C�g����ݒ肷��
//=============================================================
void CFileListContent::SetType(const FILETYPE& type)
{
	std::string sIconPath = "";
	m_fileType = type;

	switch (type)
	{
	case FOLDER:
		sIconPath = "data\\TEXTURE\\HOME\\folder_icon.png";
		break;

	case WORLD:
		sIconPath = "data\\TEXTURE\\HOME\\world_icon.png";
		break;

	case BACK:
		sIconPath = "data\\TEXTURE\\HOME\\back_icon.png";
		break;
	}

	// �A�C�R����ύX����
	if (sIconPath != "")
	{
		m_pFileIcon->GetComponent<CPolygon>()->SetTexture(sIconPath);
	}
}

//=============================================================
// [CFileListContent] �I��̈悩���擾����
//=============================================================
bool CFileListContent::GetSelectedArea(const POINT& point)
{
	D3DXVECTOR3 areaPos = m_pFileEdge->transform->GetWPos();
	if (areaPos.x <= point.x && point.x <= areaPos.x + FILELIST_WIDTH &&
		areaPos.y <= point.y && point.y <= areaPos.y + FILELIST_HEIGHT)
	{
		return true;
	}
	return false;
}

//=============================================================
// [CFileListContent] �N���b�N�����Ƃ�
//=============================================================
void CFileListContent::OnClick()
{
	m_nDoubleCounter = DOUBLECLICK_TIME;
}

//=============================================================
// [CFileListContent] �_�u���N���b�N��
//=============================================================
bool CFileListContent::GetDoubleClick()
{
	return (m_nDoubleCounter > 0);
}

//=============================================================
// [COperationSystem] �t�@�C���̓ǂݍ���
//=============================================================
void COperationSystem::LoadFiles()
{
	HANDLE hFind;
	WIN32_FIND_DATA tFindFileData;

	// World�t�H���_�̈ꗗ���擾����
	std::string sWildPath = m_sCurrentDirectory;
	sWildPath += "*";
	hFind = FindFirstFile(sWildPath.c_str(), &tFindFileData);

	// �t�@�C���R���e���c�̃��X�g
	std::vector<FileContent> pFileContents;

	// �߂鍀�ڂ��쐬���邩
	if (strcmp(&m_sCurrentDirectory[0], LOAD_PATH) == 1)
	{ // �쐬����
		FileContent backContent;
		backContent.sName = BACK_NAME;
		backContent.sPath = "";
		backContent.fileType = CFileListContent::FILETYPE::BACK;
		pFileContents.push_back(backContent);
	}

	do
	{
		// �t�H���_�[�����f
		struct stat st;

		// �p�X���쐬
		char sFilePath[MAX_PATH];
		strcpy(&sFilePath[0], &m_sCurrentDirectory[0]);
		strcat(&sFilePath[0], &tFindFileData.cFileName[0]);

		// ��Ԃ̎擾
		stat(&sFilePath[0], &st);

		// ���[���h��ǂݍ���
		if ((st.st_mode & S_IFMT) == S_IFDIR && strcmp(&tFindFileData.cFileName[0], ".") == 1 && strcmp(&tFindFileData.cFileName[0], "..") == 1)
		{ // �t�H���_�[�̂Ƃ�
			// ���t�@�C���ƃ��[���h�t�@�C�������݂��邩
			char sTargetDirectory[MAX_PATH];
			strcpy(&sTargetDirectory[0], &m_sCurrentDirectory[0]);
			strcat(&sTargetDirectory[0], tFindFileData.cFileName);
			strcat(&sTargetDirectory[0], "\\");

			char sInfoPath[MAX_PATH];
			strcpy(&sInfoPath[0], sTargetDirectory);
			strcat(&sInfoPath[0], COperationSystem::INFO_FILENAME);

			char sWorldPath[MAX_PATH];
			strcpy(&sWorldPath[0], sTargetDirectory);
			strcat(&sWorldPath[0], COperationSystem::WORLD_FILENAME);

			if (PathFileExistsA(sInfoPath) && PathFileExistsA(sWorldPath))
			{ // ���[���h�Ƃ��ĕK�v�ȃt�@�C�����܂܂�Ă���Ƃ�
				FileContent fileContent;
				fileContent.fileType = CFileListContent::FILETYPE::WORLD;
				fileContent.sPath = sTargetDirectory;
				fileContent.sName = tFindFileData.cFileName;
				pFileContents.push_back(fileContent);
			}
			else
			{ // ���[���h�Ƃ��Đ��藧���Ȃ��Ƃ�
				FileContent fileContent;
				fileContent.fileType = CFileListContent::FILETYPE::FOLDER;
				fileContent.sPath = sTargetDirectory;
				fileContent.sName = tFindFileData.cFileName;
				pFileContents.push_back(fileContent);
			}
		}
	} while (FindNextFile(hFind, &tFindFileData));

	// �f�B���N�g���������I������
	FindClose(hFind);

	// ���łɂ���t�@�C���I�u�W�F�N�g���폜����
	std::vector<GameObject*> pAllObjects = GameObject::GetAllGameObjects();
	for (int i = 0; i < pAllObjects.size(); i++)
	{
		if (pAllObjects[i]->GetTag() == "file")
		{
			pAllObjects[i]->Destroy(true);
		}
	}

	// �I�u�W�F�N�g�𐶐�����
	for (int i = 0; i < pFileContents.size(); i++)
	{
		GameObject* pFileContent = new GameObject;
		pFileContent->SetTag("file");
		pFileContent->transform->SetPos(460.0f, 350.0f + i * 120.0f);
		CFileListContent* pContent = pFileContent->AddComponent<CFileListContent>();
		pContent->SetTitle(pFileContents[i].sName);
		pContent->SetType(pFileContents[i].fileType);
		pContent->SetPath(pFileContents[i].sPath);
		pContent->SetDefaultPos({ 460.0f, 350.0f + i * 120.0f });
	}
}


//=============================================================
// [COperationSystem] �v���C�^�u�̍X�V����
//=============================================================
void COperationSystem::UpdatePlayTab()
{
	// �t�@�C�����X�g�R���|�[�l���g���擾����
	std::vector<CFileListContent*> pFileListComp = Component::GetComponents<CFileListContent>();

	// �E�B���h�E����O�ɂ���Ƃ�
	if (CWindowObject::GetSelectedAllArea(m_pCursor->GetPos()))
	{
		return;	// �������X�L�b�v
	}

	// �X�N���[���l����������
	m_nScroll -= CManager::GetMouseWheel() * 0.5f;
	m_nScroll -= INPUT_INSTANCE->GetInstance()->GetInputDevice<CGamepadDevice>()->GetState().Gamepad.sThumbRY * 0.0003f;
	if (m_nScroll < 0 || pFileListComp.size() < 6)
	{ // ���
		m_nScroll = 0;
	}
	else if ((pFileListComp.size() - 5) * 120.0f < m_nScroll)
	{ // ����
		m_nScroll = static_cast<int>((pFileListComp.size() - 5) * 120.0f);
	}

	// ���炷
	for (int i = 0; i < pFileListComp.size(); i++)
	{
		pFileListComp[i]->transform->SetPos(pFileListComp[i]->GetDefaultPos().x, pFileListComp[i]->GetDefaultPos().y - m_nScroll);
	}


	for (int i = 0; i < pFileListComp.size(); i++)
	{
		// �I��̈�ɃJ�[�\�������邩���m���߂�
		if (pFileListComp[i]->GetSelectedArea(m_pCursor->GetPos()) && INPUT_INSTANCE->onInput("click"))
		{
			// �N���b�N��
			if (!pFileListComp[i]->GetDoubleClick() ||
				(pFileListComp[i]->GetDoubleClick() && pFileListComp[i]->GetType() != CFileListContent::WORLD))
			{
				GameObject* pSound = new GameObject;
				pSound->AddComponent<CSound>();
				pSound->GetComponent<CSound>()->LoadWav("data\\SOUND\\SE\\click.wav");
				pSound->GetComponent<CSound>()->Play();
				pSound->GetComponent<CSound>()->IsStoppedDestroy();
			}

			if (pFileListComp[i]->GetDoubleClick())
			{ // �t�@�C���̎�ނ��Ƃ̏����i�_�u���N���b�N�j
				std::string sWorldPath = pFileListComp[i]->GetPath();
				GameObject* pWorldWindow;
				switch (pFileListComp[i]->GetType())
				{
				case CFileListContent::FILETYPE::WORLD:
					pWorldWindow = new GameObject;
					pWorldWindow->AddComponent<CWorldWindow>();
					pWorldWindow->GetComponent<CWorldWindow>()->SetWorldName(pFileListComp[i]->GetName());
					pWorldWindow->GetComponent<CWorldWindow>()->SetWorldPath(sWorldPath);
					break;

				case CFileListContent::FILETYPE::FOLDER:
					strcpy(&m_sCurrentDirectory[0], pFileListComp[i]->GetPath().c_str());	// �p�X���X�V����
					LoadFiles();																						// �\�����X�V����
					break;

				case CFileListContent::FILETYPE::BACK:
					RemovePathSplit(m_sCurrentDirectory, 2);							// �p�X�����
					strcat(&m_sCurrentDirectory[0], "\\");								// ��؂�̒ǉ�
					LoadFiles();																		// �\�����X�V����
					break;
				}
			}
			else
			{ // �t�@�C���̎�ނ��Ƃ̏����i�N���b�N�j

				// ���Z�b�g���
				m_thumbnailObj->SetVisible(false);
				m_limitTimeObj->SetVisible(false);

				switch (pFileListComp[i]->GetType())
				{
				case CFileListContent::FILETYPE::WORLD:
					// �I�u�W�F�N�g��\������
					m_thumbnailObj->SetVisible(true);
					m_limitTimeObj->SetVisible(true);

					// �T���l�C��
					char sThumbnailPath[MAX_PATH];
					strcpy(&sThumbnailPath[0], pFileListComp[i]->GetPath().c_str());
					strcat(&sThumbnailPath[0], "thumb.png");
					if (PathFileExists(sThumbnailPath))
					{ // �T���l�����݂���Ƃ�
						m_thumbnailObj->GetComponent<CPolygon>()->SetTexture(sThumbnailPath);
					}
					else
					{ // �T���l�����݂��Ȃ��Ƃ�
						m_thumbnailObj->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\HOME\\no_thumbnail.png");
					}

					// �t�@�C����ǂݍ���
					std::string sInfoPath = pFileListComp[i]->GetPath() + INFO_FILENAME;
					std::ifstream ifs(sInfoPath.c_str());
					std::string sInputData((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
					if (!sInputData.empty())
					{
						auto jInfo = json::parse(sInputData);

						// ��������
						int nLimitTime = 120;
						if (jInfo.contains("time"))
						{
							nLimitTime = jInfo["time"];
						}
						char sSec[3];
						sprintf(&sSec[0], "%02d", nLimitTime%60);
						m_limitTimeObj->GetComponent<CText>()->SetText("��������: " + std::to_string(nLimitTime / 60) + ":" + sSec);
					}
					
					break;
				}

				// �N���b�N
				pFileListComp[i]->OnClick();
				m_pSelectedFileContent = pFileListComp[i];
			}
		}
	}
}

//=============================================================
// [COperationSystem] ���[���h��ݒ肷��
//=============================================================
void COperationSystem::SetWorld(const std::string& sPath)
{
	// �V�[�����擾����
	CGameScene* pGameScene = (CGameScene*)CSceneManager::GetInstance()->GetScene("game")->pScene;

	// ���[���h�̃p�X��ݒ肷��
	pGameScene->SetWorld(sPath + WORLD_FILENAME);

	// �t�@�C����ǂݍ���
	std::string sInfoPath = sPath + INFO_FILENAME;
	std::ifstream ifs(sInfoPath.c_str());

	// json�`���ɕϊ�
	std::string sInputData((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	if (!sInputData.empty())
	{
		auto jInfo = json::parse(sInputData);

		// �������Ԃ�ݒ肷��
		if (jInfo.contains("time"))
		{
			pGameScene->SetLimitTime(jInfo["time"]);
		}
		else
		{
			pGameScene->SetLimitTime(120);
		}

		// �X�J�C�{�b�N�X��ݒ肷��
		if (jInfo.contains("skybox"))
		{
			pGameScene->SetSkybox(jInfo["skybox"]);
		}
		else
		{
			pGameScene->SetSkybox("");
		}

		// ���邳��ݒ肷��
		if (jInfo.contains("ambient"))
		{
			pGameScene->SetAmbient(jInfo["ambient"]);
		}
		else
		{
			pGameScene->SetAmbient(0.8f);
		}

		// BGM��ݒ肷��
		if (jInfo.contains("bgm"))
		{
			if (jInfo.contains("volume"))
			{ // ���ʐݒ肪�܂܂�Ă���Ƃ�
				pGameScene->SetBGM(jInfo["bgm"], jInfo["volume"]);
			}
			else
			{
				pGameScene->SetBGM(jInfo["bgm"], 1.0f);
			}
		}
		else
		{
			pGameScene->SetBGM("", 0.0f);
		}
	}

	// ����
	ifs.close();
}

//=============================================================
// [COperationSystem] �J�[�\���ʒu���擾����
//=============================================================
POINT COperationSystem::GetCursorPoint()
{
	return m_pCursor->GetPos();
}

//=============================================================
// [CWindowObject] ������
//=============================================================
void CWindowObject::Init()
{
	if (m_pWindowObjects.size() >= 1)
	{
		m_pWindowObjects[0]->WindowClose();
	}

	// �g�b�v�E�B���h�E�ɂ���
	m_pTopWindow = this;

	// ���X�g�ɒǉ�����
	m_pWindowObjects.push_back(this);

	// ���̏�����
	m_windowSize = { 300, 250 };
	m_bVisible = true;
	COperationSystem* pOS = (COperationSystem*)CSceneManager::GetInstance()->GetScene("os")->pScene;
	m_oldPoint = pOS->GetCursorPoint();

	// �^�C�g���o�[�̍쐬
	m_pTitleBarObj = new GameObject;
	m_pTitleBarObj->SetPriority(9);
	m_pTitleBarObj->AddComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(110, 110, 110, 255));
	m_pTitleBarObj->transform->SetSize({m_windowSize.x, TITLEBAR_HEIGHT});

	// �E�B���h�E�̈�
	m_pWindowObj = new GameObject;
	m_pWindowObj->transform->Translate(0.0f, TITLEBAR_HEIGHT, 0.0f);
	m_pWindowObj->SetPriority(9);
	m_pWindowObj->SetParent(m_pTitleBarObj);
	m_pWindowObj->AddComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	m_pWindowObj->transform->SetSize({ m_windowSize.x, m_windowSize.y });

	// �^�C�g���e�L�X�g�̍쐬
	m_pTitleTextObj = new GameObject;
	m_pTitleTextObj->transform->SetPos(20.0f, 10.0f);
	m_pTitleTextObj->SetPriority(10);
	m_pTitleTextObj->SetParent(m_pTitleBarObj);
	m_pTitleTextObj->AddComponent<CText>()->SetText("No Title");
	m_pTitleTextObj->GetComponent<CText>()->SetFont("�x�X�g�e��-CRT");
	m_pTitleTextObj->GetComponent<CText>()->SetFontSize(40);

	// �N���[�Y�{�^���̍쐬
	m_pCloseButtonObj = new GameObject;
	m_pCloseButtonObj->transform->SetSize(CLOSEBUTTON_SIZE, CLOSEBUTTON_SIZE);
	m_pCloseButtonObj->transform->SetPos(m_windowSize.x - CLOSEBUTTON_SIZE, 0.0f);
	m_pCloseButtonObj->SetPriority(10);
	m_pCloseButtonObj->SetParent(m_pTitleBarObj);
	m_pCloseButtonObj->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\HOME\\close_button.png");

	// ����炷
	GameObject* pSound = new GameObject;
	pSound->AddComponent<CSound>();
	pSound->GetComponent<CSound>()->LoadWav("data\\SOUND\\SE\\popup.wav");
	pSound->GetComponent<CSound>()->Play();
	pSound->GetComponent<CSound>()->IsStoppedDestroy();
}

//=============================================================
// [CWindowObject] �I��
//=============================================================
void CWindowObject::Uninit()
{
	// ���X�g�������
	for (int i = 0; i < m_pWindowObjects.size(); i++)
	{
		if (m_pWindowObjects[i] == this)
		{
			m_pWindowObjects.erase(m_pWindowObjects.begin() + i);
			break;
		}
	}

	if (m_pTopWindow == this)
	{
		m_pTopWindow = nullptr;
	}

	if (m_pDragging == this)
	{
		m_pDragging = nullptr;
	}
}

//=============================================================
// [CWindowObject] �X�V
//=============================================================
void CWindowObject::Update()
{
	if (!m_bVisible)
	{ // �\�����Ȃ��Ƃ�
		return;
	}

	// �N���[�Y�{�^�����������Ƃ��̏���
	COperationSystem* pOS = (COperationSystem*)CSceneManager::GetInstance()->GetScene("os")->pScene;
	POINT point = pOS->GetCursorPoint();
	if (m_pTitleBarObj->transform->GetWPos().x + m_windowSize.x - CLOSEBUTTON_SIZE <= point.x && point.x <= m_pTitleBarObj->transform->GetWPos().x + m_windowSize.x &&
		m_pTitleBarObj->transform->GetWPos().y <= point.y && point.y <= m_pTitleBarObj->transform->GetWPos().y + CLOSEBUTTON_SIZE)
	{ // �����ꂽ�Ƃ�
		if (INPUT_INSTANCE->onInput("click"))
		{
			// �E�B���h�E�����
			WindowClose();

			// ����炷
			GameObject* pSound = new GameObject;
			pSound->AddComponent<CSound>();
			pSound->GetComponent<CSound>()->LoadWav("data\\SOUND\\SE\\close.wav");
			pSound->GetComponent<CSound>()->Play();
			pSound->GetComponent<CSound>()->IsStoppedDestroy();
		}

		m_pCloseButtonObj->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(180, 180, 180, 255));
	}
	else
	{
		m_pCloseButtonObj->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	}

	// �D��x���X�V����
	if (m_pTopWindow == this)
	{
		m_pTitleBarObj->SetPriority(11);

		std::vector<GameObject*> pChildren = m_pTitleBarObj->GetChildren(true);
		for (int i = 0; i < pChildren.size(); i++)
		{
			pChildren[i]->SetPriority(12);
		}

		m_pWindowObj->SetPriority(11);
	}
	else
	{
		m_pTitleBarObj->SetPriority(9);

		std::vector<GameObject*> pChildren = m_pTitleBarObj->GetChildren(true);
		for (int i = 0; i < pChildren.size(); i++)
		{
			pChildren[i]->SetPriority(10);
		}

		m_pWindowObj->SetPriority(9);
	}

	// �E�B���h�E�̈ړ�����
	Move();
}

//=============================================================
// [CWindowObject] �E�B���h�E�����
//=============================================================
void CWindowObject::WindowClose()
{
	gameObject->Destroy(true);
	m_pTitleBarObj->Destroy(true);
}

//=============================================================
// [CWindowObject] �ړ�
//=============================================================
void CWindowObject::Move()
{
	if (GetActiveWindow()  != CManager::GetHWND())
	{ // �A�N�e�B�u�łȂ��Ƃ�
		m_pDragging = nullptr;
		m_oldPoint.x = -1.0f;
		m_oldPoint.y = -1.0f;
		return;	// �X�L�b�v
	}
		
	// ��ʏ�̃N���b�N�ʒu���擾����
	COperationSystem* pOS = (COperationSystem*)CSceneManager::GetInstance()->GetScene("os")->pScene;
	POINT point = pOS->GetCursorPoint();

	// �E�B���h�E�O����
	if ((point.x < 0.0f || CManager::GetRenderer()->SCREEN_WIDTH < point.x ||
		point.y < 0.0f || CManager::GetRenderer()->SCREEN_HEIGHT < point.y) &&
		m_pDragging == this)
	{
		m_pDragging = nullptr;
		return;
	}

	// �^�C�g���o�[��͂�ł��邩
	D3DXVECTOR3 windowPos = m_pTitleBarObj->transform->GetWPos();		// �E�B���h�E�ʒu

	// �h���b�O���Ă��邩
	if ((m_pDragging == nullptr && 
		(int)windowPos.x <= point.x && point.x <= (int)windowPos.x + m_windowSize.x &&
		(int)windowPos.y - TITLEBAR_HEIGHT / 4 <= point.y && point.y <= (int)windowPos.y + TITLEBAR_HEIGHT - TITLEBAR_HEIGHT / 4 &&
		(GetKeyState(VK_LBUTTON) & 0x80 || INPUT_INSTANCE->onPress("p:a"))) ||
		(m_pDragging == this && (GetKeyState(VK_LBUTTON) & 0x80 || INPUT_INSTANCE->onPress("p:a"))))
	{
		m_pDragging = this;
		m_pTopWindow = this;
	}
	else if (m_pDragging == this)
	{
		m_pDragging = nullptr;
	}

	// �E�B���h�E���ړ�����
	if (m_pDragging == this && !(m_oldPoint.x == -1.0f && m_oldPoint.y == -1.0f))
	{
		m_pTitleBarObj->transform->Translate(point.x - m_oldPoint.x, point.y - m_oldPoint.y, 0.0f);
	}

	// �ړ�����
	if (windowPos.x < 0.0f)
	{
		m_pTitleBarObj->transform->Translate(-windowPos.x, 0.0f, 0.0f);
	}
	if (windowPos.x + m_windowSize.x > CRenderer::SCREEN_WIDTH)
	{
		m_pTitleBarObj->transform->Translate(-(windowPos.x + m_windowSize.x - CRenderer::SCREEN_WIDTH), 0.0f, 0.0f);
	}
	if (windowPos.y< 0.0f)
	{
		m_pTitleBarObj->transform->Translate(0.0f, -windowPos.y, 0.0f);
	}
	if (windowPos.y + m_windowSize.y > CRenderer::SCREEN_HEIGHT)
	{
		m_pTitleBarObj->transform->Translate(0.0f, -(windowPos.y + m_windowSize.y - CRenderer::SCREEN_HEIGHT), 0.0f);
	}

	// �}�E�X�ʒu��ۑ����Ă���
	m_oldPoint = point;
}

//=============================================================
// [CWindowObject] �I��̈悩���擾����
//=============================================================
bool CWindowObject::GetSelectedArea(const POINT& point)
{
	D3DXVECTOR3 areaPos = m_pTitleBarObj->transform->GetWPos();
	if (areaPos.x <= point.x && point.x <= areaPos.x + m_windowSize.x &&
		areaPos.y <= point.y && point.y <= areaPos.y + m_windowSize.y)
	{
		return true;
	}
	return false;
}

//=============================================================
// [CWindowObject] ���ׂĂ̗̈�̓��A�I��̈悩���擾����
//=============================================================
bool CWindowObject::GetSelectedAllArea(const POINT& point)
{
	for (int i = 0; i < m_pWindowObjects.size(); i++)
	{
		if (m_pWindowObjects[i]->GetSelectedArea(point))
		{
			return true;
		}
	}
	return false;
}

//=============================================================
// [CWindowObject] �\�����邩
//=============================================================
void CWindowObject::SetVisible(const bool& bShow)
{
	m_bVisible = bShow;

	// �\�����X�V
	std::vector<GameObject*> pChildren = m_pTitleBarObj->GetChildren(true);
	for (int i = 0; i < pChildren.size(); i++)
	{
		pChildren[i]->SetActive(m_bVisible);
	}
}

//=============================================================
// [CWindowObject] �ʒu��ύX����
//=============================================================
void CWindowObject::SetPos(const float& x, const float& y)
{
	m_pTitleBarObj->transform->SetPos(x, y);
}

//=============================================================
// [CWindowObject] �T�C�Y��ύX����
//=============================================================
void CWindowObject::SetSize(const D3DXVECTOR2& size)
{
	m_windowSize = size;

	// �T�C�Y��ύX����
	m_pTitleBarObj->transform->SetSize({ m_windowSize.x, TITLEBAR_HEIGHT });
	m_pWindowObj->transform->SetSize({ m_windowSize.x, m_windowSize.y });
	m_pCloseButtonObj->transform->SetPos(m_windowSize.x - CLOSEBUTTON_SIZE, 0.0f);
}

//=============================================================
// [CWindowObject] �^�C�g����ύX����
//=============================================================
void CWindowObject::SetTitle(const std::string& title)
{
	m_pTitleTextObj->GetComponent<CText>()->SetText(title);
}

//=============================================================
// [CWindowObject] �w�i�J���[��ύX����
//=============================================================
void CWindowObject::SetColorBG(const D3DXCOLOR& color)
{
	m_pWindowObj->GetComponent<CPolygon>()->SetColor(color);
}

//=============================================================
// [CWindowObject] �E�B���h�E�ɃI�u�W�F�N�g��ǉ�����
//=============================================================
void CWindowObject::AddWindowObj(GameObject* pGameObject)
{
	pGameObject->SetParent(m_pTitleBarObj);

	if (m_pTopWindow == this)
		pGameObject->SetPriority(12);
	else
		pGameObject->SetPriority(10);
}


//=============================================================
// [CCloseWindow] ������
//=============================================================
void CCloseWindow::Init()
{
	// �E�B���h�E�̐���
	CWindowObject::Init();

	SetPos(CRenderer::SCREEN_WIDTH / 2 - 275.0f, CRenderer::SCREEN_HEIGHT / 2 - 140.0f);
	SetTitle("�ŏI�m�F");
	SetSize(D3DXVECTOR2(550, 280));
	SetColorBG(D3DCOLOR_RGBA(100, 0, 0, 255));

	m_pConfirmText = new GameObject;
	m_pConfirmText->transform->SetPos(160.0f, 110.0f);
	AddWindowObj(m_pConfirmText);
	m_pConfirmText->AddComponent<CText>()->SetText("�I�����܂����H");
	m_pConfirmText->GetComponent<CText>()->SetFont("�x�X�g�e��-CRT");
	m_pConfirmText->GetComponent<CText>()->SetFontSize(60);

	m_pWarningIcon = new GameObject;
	m_pWarningIcon->transform->SetPos(30.0f, 85.0f);
	m_pWarningIcon->transform->SetSize(100.0f, 100.0f);
	AddWindowObj(m_pWarningIcon);
	m_pWarningIcon->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\HOME\\warning_icon.png");

	m_pYesObj = new GameObject;
	m_pYesObj->transform->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	m_pYesObj->transform->SetPos(80.0f, 220.0f);
	m_pYesObj->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\HOME\\yes.png");
	AddWindowObj(m_pYesObj);

	m_pNoObj = new GameObject;
	m_pNoObj->transform->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	m_pNoObj->transform->SetPos(300.0f, 220.0f);
	m_pNoObj->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\HOME\\no.png");
	AddWindowObj(m_pNoObj);
}

//=============================================================
// [CCloseWindow] �X�V
//=============================================================
void CCloseWindow::Update()
{
	// �E�B���h�E�̍X�V
	CWindowObject::Update();

	// �J�[�\�����擾����
	COperationSystem* pOS = (COperationSystem*)CSceneManager::GetInstance()->GetScene("os")->pScene;
	POINT point = pOS->GetCursorPoint();
	bool bClick = INPUT_INSTANCE->onInput("click");

	// �͂��̔���
	if (m_pYesObj->transform->GetWPos().x <= point.x && point.x <= m_pYesObj->transform->GetWPos().x + BUTTON_WIDTH &&
		m_pYesObj->transform->GetWPos().y <= point.y && point.y <= m_pYesObj->transform->GetWPos().y + BUTTON_HEIGHT)
	{
		if (bClick)
		{ // �N���b�N�����Ƃ�
			DestroyWindow(CManager::GetHWND());
		}

		m_pYesObj->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(100, 100, 100, 255));
	}
	else
	{
		m_pYesObj->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	}

	// �������̔���
	if (m_pNoObj->transform->GetWPos().x <= point.x && point.x <= m_pNoObj->transform->GetWPos().x + BUTTON_WIDTH &&
		m_pNoObj->transform->GetWPos().y <= point.y && point.y <= m_pNoObj->transform->GetWPos().y + BUTTON_HEIGHT)
	{
		if (bClick)
		{ // �N���b�N�����Ƃ�
			WindowClose();
		}

		m_pNoObj->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(100, 100, 100, 255));
	}
	else
	{
		m_pNoObj->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	}
}


//=============================================================
// [CWorldWindow] ������
//=============================================================
void CWorldWindow::Init()
{
	// �E�B���h�E�̐���
	CWindowObject::Init();

	SetPos(CRenderer::SCREEN_WIDTH / 2 - 275.0f, CRenderer::SCREEN_HEIGHT / 2 - 140.0f);
	SetTitle("�m�F");
	SetSize(D3DXVECTOR2(550, 310));
	SetColorBG(D3DCOLOR_RGBA(11, 32, 82, 255));

	// �͂��{�^��
	m_pYesObj = new GameObject;
	m_pYesObj->transform->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	m_pYesObj->transform->SetPos(90.0f, 260.0f);
	m_pYesObj->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\HOME\\yes.png");
	AddWindowObj(m_pYesObj);

	// �������{�^��
	m_pNoObj = new GameObject;
	m_pNoObj->transform->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	m_pNoObj->transform->SetPos(310.0f, 260.0f);
	m_pNoObj->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\HOME\\no.png");
	AddWindowObj(m_pNoObj);

	// �X�e�[�W���\��
	m_pStageNameObj = new GameObject;
	m_pStageNameObj->transform->SetPos(30.0f, 90.0f);
	m_pStageNameObj->AddComponent<CText>();
	m_pStageNameObj->GetComponent<CText>()->SetFont("JF�h�b�gM+12");
	m_pStageNameObj->GetComponent<CText>()->SetFontSize(60);
	AddWindowObj(m_pStageNameObj);

	// �m�F���b�Z�[�W
	m_pConfirmObj = new GameObject;
	m_pConfirmObj->transform->SetPos(30.0f, 180.0f);
	m_pConfirmObj->AddComponent<CText>();
	m_pConfirmObj->GetComponent<CText>()->SetFont("�x�X�g�e��-CRT");
	m_pConfirmObj->GetComponent<CText>()->SetFontSize(50);
	m_pConfirmObj->GetComponent<CText>()->SetText("�N�����܂����H");
	AddWindowObj(m_pConfirmObj);
}

//=============================================================
// [CWorldWindow] �X�V
//=============================================================
void CWorldWindow::Update()
{
	// �E�B���h�E�̍X�V
	CWindowObject::Update();

	// �J�[�\�����擾����
	COperationSystem* pOS = (COperationSystem*)CSceneManager::GetInstance()->GetScene("os")->pScene;
	POINT point = pOS->GetCursorPoint();
	bool bClick = INPUT_INSTANCE->onInput("click");

	// �e�L�X�g�̍X�V
	m_pStageNameObj->GetComponent<CText>()->SetText("<color=247,116,64>" + m_sName + " <color=160,160,160><size=50>��");

	// �͂��̔���
	if (m_pYesObj->transform->GetWPos().x <= point.x && point.x <= m_pYesObj->transform->GetWPos().x + BUTTON_WIDTH &&
		m_pYesObj->transform->GetWPos().y <= point.y && point.y <= m_pYesObj->transform->GetWPos().y + BUTTON_HEIGHT)
	{
		if (bClick)
		{ // �N���b�N�����Ƃ�
			COperationSystem::SetWorld(m_sPath);
			CSceneManager::GetInstance()->SetScene("game");
		}

		m_pYesObj->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(100, 100, 100, 255));
	}
	else
	{
		m_pYesObj->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	}

	// �������̔���
	if (m_pNoObj->transform->GetWPos().x <= point.x && point.x <= m_pNoObj->transform->GetWPos().x + BUTTON_WIDTH &&
		m_pNoObj->transform->GetWPos().y <= point.y && point.y <= m_pNoObj->transform->GetWPos().y + BUTTON_HEIGHT)
	{
		if (bClick)
		{ // �N���b�N�����Ƃ�
			WindowClose();
		}

		m_pNoObj->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(100, 100, 100, 255));
	}
	else
	{
		m_pNoObj->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	}
}