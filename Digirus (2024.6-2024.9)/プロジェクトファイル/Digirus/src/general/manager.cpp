//=============================================================
//
// マネージャー [manager.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "manager.h"
#include "gameobject.h"
#include "scene.h"
#include "input_system.h"
#include "data_field.h"
#include "component/2d/text.h"

// 静的メンバの初期化
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
// [CManager] コンストラクタ
//=============================================================
CManager::CManager()
{

}

//=============================================================
// [CManager] デストラクタ
//=============================================================
CManager::~CManager()
{

}

//=============================================================
// [CManager] 初期化
//=============================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// ハンドルの代入
	m_hwnd = hWnd;

	// レンダラーの生成
	m_pRenderer = new CRenderer();
	m_pRenderer->Init(hInstance, hWnd, bWindow);

	// フォントの読み込み
	CText::InitLoadFont();

	// 物理の生成
	m_pPhysics = new CPhysics();
	m_pPhysics->Init();

	// データマネージャーの生成
	m_pDataManager = new CDataManager();
	m_pDataManager->Init();

	// オーディオマネージャーの生成
	m_pAudioManager = new CAudioManager();
	m_pAudioManager->Init();

	// 入力システムの生成
	CInputSystem::GetInstance()->Init(hInstance, hWnd);

	// シーンマネージャーの生成
	CSceneManager::GetInstance()->Init();
	CSceneManager::GetInstance()->SetScene(CSceneManager::START_SCENE_NAME);

	return S_OK;
}

//=============================================================
// [CManager] 終了
//=============================================================
void CManager::Uninit()
{
	// シーンマネージャーの終了
	CSceneManager::GetInstance()->Uninit();

	// すべてのオブジェクトを解放する
	GameObject::DestroyAll(true);
	GameObject::DestroyDeathFlag();

	// 入力システムの終了
	CInputSystem::GetInstance()->Uninit();

	// レンダラーを破棄する
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	// 物理を破棄する
	if (m_pPhysics != nullptr)
	{
		m_pPhysics->Uninit();
		delete m_pPhysics;
		m_pPhysics = nullptr;
	}

	// オーディオマネージャーを破棄する
	if (m_pAudioManager != nullptr)
	{
		m_pAudioManager->Uninit();
		delete m_pAudioManager;
		m_pAudioManager = nullptr;
	}

	// データ管理を破棄する
	if (m_pDataManager != nullptr)
	{
		m_pDataManager->Uninit();
		delete m_pDataManager;
		m_pDataManager = nullptr;
	}
}

//=============================================================
// [CManager] 更新
//=============================================================
void CManager::Update()
{
	// 入力システムの更新
	CInputSystem::GetInstance()->Update();

	// オーディオの更新
	m_pAudioManager->Update();

	// レンダラーの更新
	m_pRenderer->Update();

	if (!m_bPause)
	{
		// 物理の更新
		m_pPhysics->Update();
	}

	// シーンの更新
	CSceneManager::GetInstance()->Update();

	// カーソルの非表示
	if (GetActiveWindow() == CManager::GetHWND())
	{
		ShowCursor(FALSE); // 非表示
	}
	else
	{
		ShowCursor(TRUE); // 表示
	}
}

//=============================================================
// [CManager] 描画
//=============================================================
void CManager::Draw()
{
	// レンダラーの描画
	m_pRenderer->Draw();

	// シーンの描画
	CSceneManager::GetInstance()->Draw();
}
