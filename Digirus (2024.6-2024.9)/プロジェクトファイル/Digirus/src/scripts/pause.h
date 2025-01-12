//=============================================================
//
// ポーズ [pause.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "gameobject.h"
#include "component.h"
#include "scripts/cursor.h"

// ポーズ管理クラス
class CPause : public Component
{
public:
	void Init();
	void Uninit();
	void Update();

	void SetPause(const bool& bPause);
	bool GetIsPause() { return m_bIsPause; }

	static const int BUTTON_WIDTH;		// ボタンの横幅
	static const int BUTTON_HEIGHT;	// ボタンの高さ
	static const int BUTTON_SPACE;		// 余白
private:
	void SetShow(const bool& bShow);
	bool m_bIsPause;			// ポーズしているか
	CCursor* m_pCursor;		// カーソル

	GameObject* m_pBG;					// 背景
	GameObject* m_pPauseText;			// テキスト
	GameObject* m_pPauseTextBG;		// ポーズテキストの背景
	GameObject* m_pContinue;			// 続けるボタン
	GameObject* m_pRetry;					// リトライボタン
	GameObject* m_pBackHome;			// ホームへ戻るボタン
};

#endif // !_PAUSE_H_
