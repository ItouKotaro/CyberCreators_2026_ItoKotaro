//=============================================================
//
// 死亡画面 [dead.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _DEAD_H_
#define _DEAD_H_

#include "scene.h"
#include "gameobject.h"
#include "scripts/cursor.h"

// 死亡シーン
class CDeadScene : public CScene
{
public:
	void Init() override;
	void Update() override;
	void Uninit() override;
private:
	GameObject* m_pUpBG;		// 上
	GameObject* m_pDownBG;	// 下
	GameObject* m_pLeftBG;		// 左
	GameObject* m_pRightBG;		// 右
	GameObject* m_pRetry;			// リトライ
	GameObject* m_pRetryIcon;	// リトライアイコン
	GameObject* m_pHome;		// ホーム
	GameObject* m_pHomeIcon;	// ホームアイコン

	float m_fRollingRetryIcon;		// リトライアイコンの回転

	int m_nTextCounter;				// テキストカウンター
	float m_fAlpha;						// 不透明度
	CCursor* m_pCursor;				// カーソル
};

#endif // !_DEAD_H_
