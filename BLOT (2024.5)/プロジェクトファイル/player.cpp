//===================================================================================
//
// プレイヤー [player.cpp]
// Author: Ito Kotaro
//
//===================================================================================
#include "player.h"
#include "field.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#pragma comment(lib, "winmm.lib")	// システム時刻取得に必要

// 定義
const int CPlayer::PLAYER_WIDTH;
const int CPlayer::PLAYER_HEIGHT;
const int CPlayer::MAX_STACK;
const int CPlayer::STACK_INTERVAL;
const int CPlayer::STEP_INTERVAL;

//=======================================================
// [CPlayer] コンストラクタ
//=======================================================
CPlayer::CPlayer() : m_nStacks(0), m_nPos(0)
{

}

//=======================================================
// [CPlayer] デストラクタ
//=======================================================
CPlayer::~CPlayer()
{

}

//=======================================================
// [CPlayer] 積み重ね処理
//=======================================================
void CPlayer::InputStacks()
{
	// 変数宣言
	DWORD nextStack = timeGetTime() + STACK_INTERVAL;

	//　キー入力を受け付ける
	bool bFirst = true;
	while (1)
	{
		if ((GetKeyState(VK_SPACE) & 0x8000) > 0)
		{	// スペースを押しているとき
			if (timeGetTime() >= nextStack)
			{
				m_nStacks++;	// ブロックを積み上げる
				bFirst = false;

				// 次のスタック時間を設定
				nextStack = timeGetTime() + STACK_INTERVAL;
			}

			// スタックの最大数を迎えたとき
			if (m_nStacks >= MAX_STACK)
				break;	// 終了
		}
		else if (!bFirst)
		{	// 離したとき
			Sleep(STACK_INTERVAL);	// 進み始めるまでの遅延
			break;
		}

		// 描画を更新する
		Draw(PLAYER_HEIGHT);
	}
}

//=======================================================
// [CPlayer] 描画
//=======================================================
void CPlayer::Draw(int nHeight)
{
	// スタックブロックの描画
	for (int nCntBlock = 1; nCntBlock < m_nStacks + 1; nCntBlock++)
	{
		for (int i = 0; i < CField::TILE_HEIGHT; i++)
		{
			printf("\033[%d;%dH", nHeight - nCntBlock * CField::TILE_HEIGHT + i + (nCntBlock - 1), PLAYER_WIDTH + m_nPos * CField::TILE_WIDTH);
			if (i == 0 && m_nStacks == nCntBlock)
				printf(STACK_CHAR1);			// 1行目（スタックの上のみ描画）
			else if (i == 1)
				printf(STACK_CHAR2);			// 2行目
			else if (i == 2 && nCntBlock == 1)
				printf(STACK_CHAR3);			// 3行目
			else if (i == 2)
				printf(STACK_LINK);				// 3行目（つなぎ）
		}
	}

	// プレイヤーを描画
	for (int i = 0; i < CField::TILE_HEIGHT; i++)
	{
		int nNoStacks = bool(m_nStacks == 0);
		printf("\033[%d;%dH", 
			nHeight - CField::TILE_HEIGHT * (m_nStacks + 1) + (m_nStacks - 1) + i + nNoStacks,
			PLAYER_WIDTH + CField::TILE_WIDTH * m_nPos);		// 初期位置に移動

		if (i == 0)
			printf(PLAYER_CHAR1);			// 1行目
		else if (i == 1)
			printf(PLAYER_CHAR2);			// 2行目
		else if (i == 2)
			printf(PLAYER_CHAR3);			// 3行目
	}
}