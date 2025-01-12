//===================================================================================
//
// Blot [main.cpp]
// Author: Ito Kotaro
//
//===================================================================================
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// 定義
const int CProcess::SELECT_WIDTH;
const int CProcess::SELECT_HEIGHT;
const int CProcess::TITLE_WIDTH;
const int CProcess::TITLE_HEIGHT;
const int CProcess::SCREEN_UPDATE;
const int CProcess::TITLE_TO_GAME;

//=======================================================
// メイン
//=======================================================
int main(void)
{
	// フルスクリーンに強制変更
	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);

	// 画面をクリア
	system("cls");

	// プロセスクラスを作成する
	CProcess process;

	// 画面のスクリーンチェック
	process.ScreenCheck();

	// カーソルの非表示
	process.LookCursor(false);

	// モードセレクト
	process.SelectMode();

	return 0;
}


//=======================================================
// [CProcess] モードセレクト
//=======================================================
void CProcess::SelectMode()
{
	// 変数宣言
	MODE selectMode = MODE_START;

	while (1)
	{
		// タイトルをプリント
		PrintTitle();

		switch (selectMode)
		{
		case MODE_START:
			printf("\033[%d;%dH\033[36m┏━━━━━━━━━━━━━━━┓\033[39m     ┌───────────────┐"	, SELECT_HEIGHT, SELECT_WIDTH);
			printf("\033[%d;%dH\033[36m┃     始める    ┃\033[39m     │      終了     │"															, SELECT_HEIGHT + 1, SELECT_WIDTH);
			printf("\033[%d;%dH\033[36m┗━━━━━━━━━━━━━━━┛\033[39m     └───────────────┘"	, SELECT_HEIGHT + 2, SELECT_WIDTH);

			if ((GetKeyState(VK_RIGHT) & 0x8000) > 0)
			{
				selectMode = MODE_END;
			}
			break;

		case MODE_END:
			printf("\033[%d;%dH┌───────────────┐     \033[31m┏━━━━━━━━━━━━━━━┓\033[39m"	, SELECT_HEIGHT, SELECT_WIDTH);
			printf("\033[%d;%dH│     始める    │     \033[31m┃      終了     ┃\033[39m"															, SELECT_HEIGHT + 1, SELECT_WIDTH);
			printf("\033[%d;%dH└───────────────┘     \033[31m┗━━━━━━━━━━━━━━━┛\033[39m"	, SELECT_HEIGHT + 2, SELECT_WIDTH);

			if ((GetKeyState(VK_LEFT) & 0x8000) > 0)
			{
				selectMode = MODE_START;
			}
			break;
		}

		// 決定する
		if ((GetKeyState(VK_RETURN) & 0x8000) > 0 ||
			(GetKeyState(VK_SPACE) & 0x8000) > 0)
		{
			switch (selectMode)
			{
			case MODE_START:
				StartGame();		// ゲーム開始
				break;

			case MODE_END:
				Uninit();			// 終了処理
				return;
			}
		}

	}
}

//=======================================================
// [CProcess] コンストラクタ
//=======================================================
CProcess::CProcess()
{

}

//=======================================================
// [CProcess] デストラクタ
//=======================================================
CProcess::~CProcess()
{
	// ゲームマネージャーを破棄する
	if (m_pGameManager != NULL)
	{
		delete m_pGameManager;
		m_pGameManager = NULL;
	}
}

//=======================================================
// [CProcess] ゲームスタート
//=======================================================
void CProcess::StartGame()
{
	// ゲームマネージャーを生成する
	m_pGameManager = new CGameManager();

	// ゲームマネージャーに開始の命令をする
	m_pGameManager->StartGame();

	// ゲームマネージャーを破棄する
	if (m_pGameManager != NULL)
	{
		delete m_pGameManager;
		m_pGameManager = NULL;
	}
}

//=======================================================
// [CProcess] 終了
//=======================================================
void CProcess::Uninit()
{

}

//=======================================================
// [CProcess] 表示フラグ
//=======================================================
void CProcess::LookCursor(bool isLook)
{
	HANDLE hOut;
	CONSOLE_CURSOR_INFO cci;

	//出力用ハンドルの取得
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	//CONSOLE_CURSOR_INFO構造体の現在の状態を取得する
	GetConsoleCursorInfo(hOut, &cci);

	//メンバ変数であるbVisibleがカーソルの表示・非表示を設定する
	cci.bVisible = isLook;

	//変更した構造体情報をコンソールWindowにセットする
	SetConsoleCursorInfo(hOut, &cci);
}

//=======================================================
// [CProcess] タイトル出力
//=======================================================
void CProcess::PrintTitle()
{
	printf("\033[%d;%dH _______    __          ______    ________ ", TITLE_HEIGHT + 0, TITLE_WIDTH);
	printf("\033[%d;%dH/       | /   |        /      \\  /        |", TITLE_HEIGHT + 1, TITLE_WIDTH);
	printf("\033[%d;%dH$$$$$$$ | |$$ |       /$$$$$$  | $$$$$$$$/ ", TITLE_HEIGHT + 2, TITLE_WIDTH);
	printf("\033[%d;%dH$$ |__$$| |$$ |       $$ |  $$ |    $$ |   ", TITLE_HEIGHT + 3, TITLE_WIDTH);
	printf("\033[%d;%dH$$    $<  |$$ |       $$ |  $$ |    $$ |   ", TITLE_HEIGHT + 4, TITLE_WIDTH);
	printf("\033[%d;%dH$$$$$$$ | |$$ |       $$ |  $$ |    $$ |   ", TITLE_HEIGHT + 5, TITLE_WIDTH);
	printf("\033[%d;%dH$$ |__$$| |$$ |_____  $$ |__$$ |    $$ |   ", TITLE_HEIGHT + 6, TITLE_WIDTH);
	printf("\033[%d;%dH$$    $$/ |$$       | $$    $$/     $$ |   ", TITLE_HEIGHT + 7, TITLE_WIDTH);
	printf("\033[%d;%dH$$$$$$$/  |$$$$$$$$/   $$$$$$/      $$/    ", TITLE_HEIGHT + 8, TITLE_WIDTH);
}

//=======================================================
// [CProcess] スクリーンチェック時の説明出力
//=======================================================
void CProcess::PrintScreenDescription()
{
	printf("\033[%d;%dH", CGameManager::DESCRIPTION_HEIGHT, CGameManager::DESCRIPTION_WIDTH + 2);
	printf("表示が崩れないサイズに変更してください");
	printf("\033[%d;%dH", CGameManager::DESCRIPTION_HEIGHT + 1, CGameManager::DESCRIPTION_WIDTH + 14);
	printf("(F11で最大化)");
	printf("\033[%d;%dH", CGameManager::DESCRIPTION_HEIGHT + 4, CGameManager::DESCRIPTION_WIDTH + 10);
	printf("SPACEキーで続行します");
}

//=======================================================
// [CProcess] スクリーンチェック
//=======================================================
void CProcess::ScreenCheck()
{
	// ゲームマネージャーを生成する
	m_pGameManager = new CGameManager();

	// 説明を消す
	m_pGameManager->HideDescription();

	// ブロックを敷き詰める
	CField* pField = m_pGameManager->GetField();
	for (int i = 0; i < CField::FIELD_LENGTH; i++)
	{
		pField->Set(i, CField::BLOCK_PLAYER);
	}

	// 更新時間を設定する
	DWORD updateTime = timeGetTime();

	while (1)
	{
		// 更新時間が来たら描画を更新
		if (timeGetTime() >= updateTime)
		{
			// 描画する
			m_pGameManager->Draw();

			// 説明を描画する
			PrintScreenDescription();

			// 次の更新時間を設定
			updateTime = timeGetTime() + SCREEN_UPDATE;
		}
		
		// プレイヤーの入力
		if ((GetKeyState(VK_SPACE) & 0x8000) > 0 ||
			(GetKeyState(VK_RETURN) & 0x8000) > 0)
		{
			break;
		}
	}

	// ゲームマネージャーを破棄する
	if (m_pGameManager != NULL)
	{
		delete m_pGameManager;
		m_pGameManager = NULL;
	}

	// 画面をクリア
	system("cls");

	// 待機
	Sleep(TITLE_TO_GAME);
}