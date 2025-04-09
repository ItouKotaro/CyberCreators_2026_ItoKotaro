//=============================================================
//
// マネージャー [manager.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "renderer.h"
#include "scene.h"
#include "internal/physics.h"
#include "internal/data_manager.h"
#include "internal/audio_manager.h"
#include "gameobject.h"

// マネージャークラスの定義
class CManager
{
public:
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);	// 初期化
	void Uninit();																						// 終了
	void Update();																					// 更新
	void Draw();																						// 描画

	void SetFPS(const int& nFPS) { m_nFPS = nFPS; }											// FPSの設定（ユーザーの使用禁止）
	static const int GetFPS() { return m_nFPS; }													// FPSの取得
	void SetDeltaTime(const float& fDeltaTime) { m_fDeltaTime = fDeltaTime; }		// デルタタイムの設定（ユーザーの使用禁止）
	static const float GetDeltaTime() { return m_fDeltaTime; }								// デルタタイムの取得
	static void SetPause(const bool& bPause) { m_bPause = bPause; }					// ポーズの設定
	static const bool GetPause() { return m_bPause; }											// ポーズの取得
	void SetMouseWheel(const int& nMouseWheel) { m_nMouseWheel = nMouseWheel; }	// マウスホイールの設定（ユーザーの使用禁止）
	static const int GetMouseWheel() { return m_nMouseWheel; }							// マウスホイールの取得
	void SetShowCursor(const bool& show);

	struct CursorPos
	{
		float x;
		float y;
	};

	CursorPos GetCursorClientPos();																		// カーソル位置の取得
	void SetCursorClientPos(float x, float y);															// カーソル位置の設定
	D3DXVECTOR2 GetWindowSize();																	// ウィンドウサイズの取得

	static HWND GetHWND() { return m_hwnd; }													// ハンドルの取得
	static CRenderer* GetRenderer() { return m_pRenderer; }								// レンダラーの取得
	static CPhysics* GetPhysics() { return m_pPhysics; }										// 物理の取得
	static CDataManager* GetDataManager() { return m_pDataManager; }				// データマネージャーの取得
private:
	static int m_nFPS;												// FPS値
	static float m_fDeltaTime;									// デルタタイム
	static int m_nMouseWheel;									// マウスホイール値
	static bool m_bPause;											// ポーズ
	static bool m_bShowCursor;								// カーソルの表示
	static HWND m_hwnd;										// ハンドル
	static CRenderer* m_pRenderer;							// レンダラー
	static CPhysics* m_pPhysics;								// 物理
	static CDataManager* m_pDataManager;				// データマネージャー
	static CAudioManager* m_pAudioManager;			// オーディオマネージャー
};

#endif // !_MANAGER_H_
