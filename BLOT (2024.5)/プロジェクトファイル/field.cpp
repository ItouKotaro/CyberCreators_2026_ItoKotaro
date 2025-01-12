//===================================================================================
//
// フィールド [field.cpp]
// Author: Ito Kotaro
//
//===================================================================================
#include "field.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// 定義
const int CField::FIELD_LENGTH;
const int CField::TILE_WIDTH;
const int CField::TILE_HEIGHT;
const int CField::FIELD_WIDTH;
const int CField::FIELD_HEIGHT;
const int CField::MIN_DISTANCE;
const int CField::MAX_SPACE;
const int CField::SUPPORT_DISTANCE;
const int CField::CHANCE_DISTANCE;

//=======================================================
// [CField] コンストラクタ
//=======================================================
CField::CField()
{
	// フィールドの初期化
	for (int i = 0; i < FIELD_LENGTH; i++)
		m_aBlock[i] = BLOCK_NONE;

	Set(0, BLOCK_FIELD);
}

//=======================================================
// [CField] 生成
//=======================================================
void CField::Generate(int nScore)
{
	// ランダムシードを設定する
	srand((unsigned int)clock());

	// ブロックのある最終地点を取得する
	int nLastBlock = 0;
	bool bPassNone = false;
	for (int i = 0; i < FIELD_LENGTH; i++)
	{
		if (m_aBlock[i] == BLOCK_FIELD)
		{
			if (!bPassNone)
				nLastBlock = i;
			else
				return;
		}
		else if (bPassNone == false)
		{
			bPassNone = true;
		}
	}

	// 何ブロック分のスペースを用意するか設定する
	int nSpace = 0;

	// 最終地点から離す距離を設定する
	int nDistance;
	nDistance = nLastBlock + MIN_DISTANCE;
	

	// 難易度調整
	if (nScore <= SUPPORT_DISTANCE)
	{ // サポート
		nDistance += rand() % 5;
		nSpace = rand() % 3 + 2;
	}
	else
	{ // 通常
		nDistance += rand() % (CPlayer::MAX_STACK - MIN_DISTANCE * 2);
		nSpace = rand() % MAX_SPACE + 1;

		// 距離が10を超えたとき、チャンスを与える
		if (nDistance >= CHANCE_DISTANCE)
			nSpace += rand() % 3;
	}

	// ブロックを設置する
	for (int i = 0; i < nSpace; i++)
	{
		Set(nLastBlock + nDistance + i, BLOCK_FIELD);
	}
}

//=======================================================
// [CField] 設置
//=======================================================
void CField::Set(int nPos, BLOCK type)
{
	if (0 <= nPos && nPos < FIELD_LENGTH)
		m_aBlock[nPos] = type;
}

//=======================================================
// [CField] 取得
//=======================================================
CField::BLOCK CField::Get(int nPos)
{
	if (0 <= nPos && nPos < FIELD_LENGTH)
		return m_aBlock[nPos];

	return BLOCK_NONE;
}

//=======================================================
// [CField] 描画
//=======================================================
void CField::Draw(int nHeight)
{
	for (int nCntBlock = 0; nCntBlock < FIELD_LENGTH; nCntBlock++)
	{
		for (int i = 0; i < TILE_HEIGHT; i++)
		{
			printf("\033[%d;%dH", nHeight + i, FIELD_WIDTH + CField::TILE_WIDTH * nCntBlock);

			switch (m_aBlock[nCntBlock])
			{
			case BLOCK_NONE:
				printf("     ");
				break;

			case BLOCK_FIELD:
				if (i == 0)
					printf(BLOCK_FIELD_CHAR1);
				else if (i == 1)
					printf(BLOCK_FIELD_CHAR2);
				else if (i == 2)
					printf(BLOCK_FIELD_CHAR3);
				break;

			case BLOCK_PLAYER:
				if (i == 0)
					printf(BLOCK_PLAYER_CHAR1);
				else if (i == 1)
					printf(BLOCK_PLAYER_CHAR2);
				else if (i == 2)
					printf(BLOCK_PLAYER_CHAR3);
				break;
			}

		}
	}
}