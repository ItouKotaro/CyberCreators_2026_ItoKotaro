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
bool CManager::m_bShowCursor = true;

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

	// �f�[�^�Ǘ���j������
	if (m_pDataManager != nullptr)
	{
		m_pDataManager->Uninit();
		delete m_pDataManager;
		m_pDataManager = nullptr;
	}

	// �I�[�f�B�I�}�l�[�W���[��j������
	if (m_pAudioManager != nullptr)
	{
		m_pAudioManager->Uninit();
		delete m_pAudioManager;
		m_pAudioManager = nullptr;
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

//=============================================================
// [CManager] �J�[�\���̕\��
//=============================================================
void CManager::SetShowCursor(const bool& show)
{
	if (m_bShowCursor != show)
	{
		ShowCursor(show ? TRUE : FALSE);
		m_bShowCursor = show;
	}
}

//=============================================================
// [CManager] �J�[�\���ʒu���擾����
//=============================================================
CManager::CursorPos CManager::GetCursorClientPos()
{
	POINT points;
	GetCursorPos(&points);

	// �X�N���[����Ō�������̍��W���擾����
	POINT startPos;
	startPos.x = 0;
	startPos.y = 0;
	ClientToScreen(m_hwnd, &startPos);

	CursorPos cPos;
	cPos.x = static_cast<float>(points.x - startPos.x);
	cPos.y = static_cast<float>(points.y - startPos.y);


	D3DXVECTOR2 rect = GetWindowSize();
	cPos.x *= static_cast<float>(CRenderer::SCREEN_WIDTH / (float)rect.x);
	cPos.y *= static_cast<float>(CRenderer::SCREEN_HEIGHT / (float)rect.y);
	return cPos;
}

//=============================================================
// [CManager] �J�[�\���ʒu��ݒ肷��
//=============================================================
void CManager::SetCursorClientPos(float x, float y)
{
	CursorPos cPos;
	cPos.x = x;
	cPos.y = y;

	// �X�N���[����Ō�������̍��W���擾����
	POINT startPos;
	startPos.x = 0;
	startPos.y = 0;
	ClientToScreen(m_hwnd, &startPos);

	D3DXVECTOR2 rect = GetWindowSize();
	cPos.x *= static_cast<float>(rect.x / (float)CRenderer::SCREEN_WIDTH);
	cPos.y *= static_cast<float>(rect.y / (float)CRenderer::SCREEN_HEIGHT);

	cPos.x += startPos.x;
	cPos.y += startPos.y;

	SetCursorPos(static_cast<int>(cPos.x), static_cast<int>(cPos.y));
}

//=============================================================
// [CManager] �E�B���h�E�T�C�Y
//=============================================================
D3DXVECTOR2 CManager::GetWindowSize()
{
	RECT rect;
	GetWindowRect(GetManager()->GetHWND(), &rect);
	return D3DXVECTOR2(static_cast<float>(rect.right - rect.left), static_cast<float>(rect.bottom - rect.top));
}