//===================================================================================
//
// Blot [main.h]
// Author: Ito Kotaro
//
//===================================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

// インクルード
#include "game.h"

// プロセスクラスの定義
class CProcess
{
public:
	CProcess();		// コンストラクタ
	~CProcess();		// デストラクタ

	// モード
	typedef enum
	{
		MODE_START = 0,		// スタート
		MODE_END,				// 終了
		MODE_MAX
	} MODE;

	void SelectMode();						// モードセレクト
	void LookCursor(bool isLook);		// カーソルの表示・非表示
	void PrintTitle();							// タイトルの描画
	void ScreenCheck();					// スクリーンチェック

	// 定義
	static const int SELECT_WIDTH = 75;				// 選択肢の横
	static const int SELECT_HEIGHT = 40;				// 選択肢の高さ
	static const int TITLE_WIDTH = 74;					// タイトルの横
	static const int TITLE_HEIGHT = 10;				// タイトルの高さ
	static const int SCREEN_UPDATE = 500;			// スクリーンチェックの描画更新間隔
	static const int TITLE_TO_GAME = 500;
private:
	void StartGame();						// ゲームを始める
	void Uninit();								// 終了
	void PrintScreenDescription();		// スクリーンチェック時の説明

	CGameManager* m_pGameManager;		// ゲームマネージャー
};

#endif // !_MAIN_H_
