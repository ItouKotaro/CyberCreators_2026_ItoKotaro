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
bool CManager::m_bShowCursor = true;

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

	// データ管理を破棄する
	if (m_pDataManager != nullptr)
	{
		m_pDataManager->Uninit();
		delete m_pDataManager;
		m_pDataManager = nullptr;
	}

	// オーディオマネージャーを破棄する
	if (m_pAudioManager != nullptr)
	{
		m_pAudioManager->Uninit();
		delete m_pAudioManager;
		m_pAudioManager = nullptr;
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

//=============================================================
// [CManager] カーソルの表示
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
// [CManager] カーソル位置を取得する
//=============================================================
CManager::CursorPos CManager::GetCursorClientPos()
{
	POINT points;
	GetCursorPos(&points);

	// スクリーン上で見た左上の座標を取得する
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
// [CManager] カーソル位置を設定する
//=============================================================
void CManager::SetCursorClientPos(float x, float y)
{
	CursorPos cPos;
	cPos.x = x;
	cPos.y = y;

	// スクリーン上で見た左上の座標を取得する
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
// [CManager] ウィンドウサイズ
//=============================================================
D3DXVECTOR2 CManager::GetWindowSize()
{
	RECT rect;
	GetWindowRect(GetManager()->GetHWND(), &rect);
	return D3DXVECTOR2(static_cast<float>(rect.right - rect.left), static_cast<float>(rect.bottom - rect.top));
}