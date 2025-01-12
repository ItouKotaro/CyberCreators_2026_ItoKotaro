//=============================================================
//
// �}�l�[�W���[ [manager.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "manager.h"
#include "gameobject.h"
#include "scene.h"
#include "input_system.h"
#include "data_field.h"
#include "component/2d/text.h"

// �ÓI�����o�̏�����
CRenderer* CManager::m_pRenderer = nullptr;
CPhysics* CManager::m_pPhysics = nullptr;
CDataManager* CManager::m_pDataManager = nullptr;
CAudioManager* CManager::m_pAudioManager = nullptr;
HWND CManager::m_hwnd = nullptr;
int CManager::m_nFPS = 0;
int CManager::m_nMouseWheel = 0;
float CManager::m_fDeltaTime = 0.0f;
bool CManager::m_bPause = false;

//=============================================================
// [CManager] �R���X�g���N�^
//=============================================================
CManager::CManager()
{

}

//=============================================================
// [CManager] �f�X�g���N�^
//=============================================================
CManager::~CManager()
{

}

//=============================================================
// [CManager] ������
//=============================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// �n���h���̑��
	m_hwnd = hWnd;

	// �����_���[�̐���
	m_pRenderer = new CRenderer();
	m_pRenderer->Init(hInstance, hWnd, bWindow);

	// �t�H���g�̓ǂݍ���
	CText::InitLoadFont();

	// �����̐���
	m_pPhysics = new CPhysics();
	m_pPhysics->Init();

	// �f�[�^�}�l�[�W���[�̐���
	m_pDataManager = new CDataManager();
	m_pDataManager->Init();

	// �I�[�f�B�I�}�l�[�W���[�̐���
	m_pAudioManager = new CAudioManager();
	m_pAudioManager->Init();

	// ���̓V�X�e���̐���
	CInputSystem::GetInstance()->Init(hInstance, hWnd);

	// �V�[���}�l�[�W���[�̐���
	CSceneManager::GetInstance()->Init();
	CSceneManager::GetInstance()->SetScene(CSceneManager::START_SCENE_NAME);

	return S_OK;
}

//=============================================================
// [CManager] �I��
//=============================================================
void CManager::Uninit()
{
	// �V�[���}�l�[�W���[�̏I��
	CSceneManager::GetInstance()->Uninit();

	// ���ׂẴI�u�W�F�N�g���������
	GameObject::DestroyAll(true);
	GameObject::DestroyDeathFlag();

	// ���̓V�X�e���̏I��
	CInputSystem::GetInstance()->Uninit();

	// �����_���[��j������
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	// ������j������
	if (m_pPhysics != nullptr)
	{
		m_pPhysics->Uninit();
		delete m_pPhysics;
		m_pPhysics = nullptr;
	}

	// �I�[�f�B�I�}�l�[�W���[��j������
	if (m_pAudioManager != nullptr)
	{
		m_pAudioManager->Uninit();
		delete m_pAudioManager;
		m_pAudioManager = nullptr;
	}

	// �f�[�^�Ǘ���j������
	if (m_pDataManager != nullptr)
	{
		m_pDataManager->Uninit();
		delete m_pDataManager;
		m_pDataManager = nullptr;
	}
}

//=============================================================
// [CManager] �X�V
//=============================================================
void CManager::Update()
{
	// ���̓V�X�e���̍X�V
	CInputSystem::GetInstance()->Update();

	// �I�[�f�B�I�̍X�V
	m_pAudioManager->Update();

	// �����_���[�̍X�V
	m_pRenderer->Update();

	if (!m_bPause)
	{
		// �����̍X�V
		m_pPhysics->Update();
	}

	// �V�[���̍X�V
	CSceneManager::GetInstance()->Update();

	// �J�[�\���̔�\��
	if (GetActiveWindow() == CManager::GetHWND())
	{
		ShowCursor(FALSE); // ��\��
	}
	else
	{
		ShowCursor(TRUE); // �\��
	}
}

//=============================================================
// [CManager] �`��
//=============================================================
void CManager::Draw()
{
	// �����_���[�̕`��
	m_pRenderer->Draw();

	// �V�[���̕`��
	CSceneManager::GetInstance()->Draw();
}
