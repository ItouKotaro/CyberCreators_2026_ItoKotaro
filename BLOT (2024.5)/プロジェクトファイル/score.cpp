//===================================================================================
//
// スコア [score.cpp]
// Author: Ito Kotaro
//
//===================================================================================
#include "score.h"
#include <stdio.h>
#include <math.h>
#include <sys/stat.h>  

// 定義
const int CScore::NUM_WIDTH;
const int CScore::NUM_HEIGHT;
const int CScore::NUM_SPACE;
const int CScore::SCORE_HEIGHT;
const int CScore::CENTER;
const int CScore::MAX_RANKING;

//=======================================================
// [CScore] コンストラクタ
//=======================================================
CScore::CScore() : m_nScore(0)
{

}

//=======================================================
// [CScore] スコアを登録
//=======================================================
int CScore::RegisterScore()
{
	// dataフォルダが存在するか
	struct stat st;
	if (stat("data", &st) != 0)
		return -1;	// 存在しないときはランキングに登録しない

	// スコアが0のときは登録しない
	if (m_nScore == 0)
		return -1;

	//-----------------------------------------------------------------
	// 記録ファイルを読み込む

	int aScore[MAX_RANKING] = {};

	// 変数を初期化する
	for (int i = 0; i < MAX_RANKING; i++)
		aScore[i] = 0;

	// ファイルを開く
	FILE* pFile = NULL;
	pFile = fopen(RANKING_PATH, "rb");

	if (pFile != NULL)
	{ // ファイルが見つかったとき

		// ファイルからデータを取り出す
		fread(&aScore, sizeof(int), MAX_RANKING, pFile);

		// ファイルを閉じる
		fclose(pFile);
	}

	//-----------------------------------------------------------------
	// 記録を更新する場合は記録ファイルに書き込む
	int nRank = 0;
	if (aScore[MAX_RANKING - 1] <= m_nScore)
	{ // 最高記録のとき

		// 最高記録に代入する
		aScore[MAX_RANKING - 1] = m_nScore;

		// ランキングを入れ替える
		for (int nCnt = 0; nCnt < MAX_RANKING - 1; nCnt++)
		{
			if (aScore[MAX_RANKING - (nCnt + 1)] > aScore[MAX_RANKING - (nCnt + 2)])
			{
				int nSaveNum = aScore[MAX_RANKING - (nCnt + 1)];
				aScore[MAX_RANKING - (nCnt + 1)] = aScore[MAX_RANKING - (nCnt + 2)];
				aScore[MAX_RANKING - (nCnt + 2)] = nSaveNum;
				nRank++;
			}
		}

		// 何位かを計算する
		nRank = MAX_RANKING - nRank;

		// ファイルを書き出し状態で開く
		pFile = fopen(RANKING_PATH, "wb");

		// 記録を書き込む
		fwrite(&aScore, sizeof(int), MAX_RANKING, pFile);

		// ファイルを閉じる
		fclose(pFile);

		return nRank;
	}

	return -1;
}

//=======================================================
// [CScore] 描画
//=======================================================
void CScore::Draw(int nHeight)
{
	// 変数宣言
	int nDigit = 1;			// 桁数
	int nStartWidth = 0;	// 出力の始まりの位置

	// スコアが何桁あるか調べる
	if (m_nScore != 0)
		nDigit = log10(m_nScore) + 1;

	// 中央に表示するために書き始めの位置を計算する
	nStartWidth = CENTER - (NUM_WIDTH * nDigit + NUM_SPACE * (nDigit - 1)) / 2;

	// 描画する
	for (int nCntDigit = 1; nCntDigit <= nDigit; nCntDigit++)
	{
		// 位の数字を取得する
		int nNum = 0;
		int nPow = (int)pow(10, (double)nDigit - (double)nCntDigit);
		nNum = m_nScore % (nPow *10) / nPow;

		// 描画位置を計算する
		int nWriteWidth = nStartWidth + NUM_WIDTH * (nCntDigit - 1) + NUM_SPACE *(nCntDigit - 1);

		// 数字の描画
		switch (nNum)
		{
		case 0:
			printf("\033[%d;%dH┏━━━━┓", nHeight + 0, nWriteWidth);
			printf("\033[%d;%dH┃ ┏┓ ┃", nHeight + 1, nWriteWidth);
			printf("\033[%d;%dH┃ ┃┃ ┃", nHeight + 2, nWriteWidth);
			printf("\033[%d;%dH┃ ┃┃ ┃", nHeight + 3, nWriteWidth);
			printf("\033[%d;%dH┃ ┗┛ ┃", nHeight + 4, nWriteWidth);
			printf("\033[%d;%dH┗━━━━┛", nHeight + 5, nWriteWidth);
			break;

		case 1:
			printf("\033[%d;%dH  ┏━┓  ", nHeight + 0, nWriteWidth);
			printf("\033[%d;%dH ┏┛ ┃ ", nHeight + 1, nWriteWidth);
			printf("\033[%d;%dH ┗┫ ┃ ", nHeight + 2, nWriteWidth);
			printf("\033[%d;%dH  ┃ ┃  ", nHeight + 3, nWriteWidth);
			printf("\033[%d;%dH ┏┛ ┗┓", nHeight + 4, nWriteWidth);
			printf("\033[%d;%dH ┗━━━┛", nHeight + 5, nWriteWidth);
			break;

		case 2:
			printf("\033[%d;%dH┏━━━━┓", nHeight + 0, nWriteWidth);
			printf("\033[%d;%dH┗━━┓ ┃", nHeight + 1, nWriteWidth);
			printf("\033[%d;%dH┏━━┛ ┃", nHeight + 2, nWriteWidth);
			printf("\033[%d;%dH┃ ┏━━┛", nHeight + 3, nWriteWidth);
			printf("\033[%d;%dH┃ ┗━━┓", nHeight + 4, nWriteWidth);
			printf("\033[%d;%dH┗━━━━┛", nHeight + 5, nWriteWidth);
			break;

		case 3:
			printf("\033[%d;%dH┏━━━━┓", nHeight + 0, nWriteWidth);
			printf("\033[%d;%dH┗━━┓ ┃", nHeight + 1, nWriteWidth);
			printf("\033[%d;%dH┏━━┛ ┃", nHeight + 2, nWriteWidth);
			printf("\033[%d;%dH┗━━┓ ┃", nHeight + 3, nWriteWidth);
			printf("\033[%d;%dH┏━━┛ ┃", nHeight + 4, nWriteWidth);
			printf("\033[%d;%dH┗━━━━┛", nHeight + 5, nWriteWidth);
			break;

		case 4:
			printf("\033[%d;%dH┏━┓┏━┓", nHeight + 0, nWriteWidth);
			printf("\033[%d;%dH┃ ┃┃ ┃", nHeight + 1, nWriteWidth);
			printf("\033[%d;%dH┃ ┗┛ ┃", nHeight + 2, nWriteWidth);
			printf("\033[%d;%dH┗━━┓ ┃", nHeight + 3, nWriteWidth);
			printf("\033[%d;%dH   ┃ ┃", nHeight + 4, nWriteWidth);
			printf("\033[%d;%dH   ┗━┛", nHeight + 5, nWriteWidth);
			break;

		case 5:
			printf("\033[%d;%dH┏━━━━┓", nHeight + 0, nWriteWidth);
			printf("\033[%d;%dH┃ ┏━━┛", nHeight + 1, nWriteWidth);
			printf("\033[%d;%dH┃ ┗━━┓", nHeight + 2, nWriteWidth);
			printf("\033[%d;%dH┗━━┓ ┃", nHeight + 3, nWriteWidth);
			printf("\033[%d;%dH┏━━┛ ┃", nHeight + 4, nWriteWidth);
			printf("\033[%d;%dH┗━━━━┛", nHeight + 5, nWriteWidth);
			break;

		case 6:
			printf("\033[%d;%dH┏━━━━┓", nHeight + 0, nWriteWidth);
			printf("\033[%d;%dH┃ ┏━━┛", nHeight + 1, nWriteWidth);
			printf("\033[%d;%dH┃ ┗━━┓", nHeight + 2, nWriteWidth);
			printf("\033[%d;%dH┃ ┏┓ ┃", nHeight + 3, nWriteWidth);
			printf("\033[%d;%dH┃ ┗┛ ┃", nHeight + 4, nWriteWidth);
			printf("\033[%d;%dH┗━━━━┛", nHeight + 5, nWriteWidth);
			break;

		case 7:
			printf("\033[%d;%dH┏━━━━┓", nHeight + 0, nWriteWidth);
			printf("\033[%d;%dH┗━━┓ ┃", nHeight + 1, nWriteWidth);
			printf("\033[%d;%dH   ┃ ┃", nHeight + 2, nWriteWidth);
			printf("\033[%d;%dH   ┃ ┃", nHeight + 3, nWriteWidth);
			printf("\033[%d;%dH   ┃ ┃", nHeight + 4, nWriteWidth);
			printf("\033[%d;%dH   ┗━┛", nHeight + 5, nWriteWidth);
			break;

		case 8:
			printf("\033[%d;%dH┏━━━━┓", nHeight + 0, nWriteWidth);
			printf("\033[%d;%dH┃ ┏┓ ┃", nHeight + 1, nWriteWidth);
			printf("\033[%d;%dH┃ ┗┛ ┃", nHeight + 2, nWriteWidth);
			printf("\033[%d;%dH┃ ┏┓ ┃", nHeight + 3, nWriteWidth);
			printf("\033[%d;%dH┃ ┗┛ ┃", nHeight + 4, nWriteWidth);
			printf("\033[%d;%dH┗━━━━┛", nHeight + 5, nWriteWidth);
			break;

		case 9:
			printf("\033[%d;%dH┏━━━━┓", nHeight + 0, nWriteWidth);
			printf("\033[%d;%dH┃ ┏┓ ┃", nHeight + 1, nWriteWidth);
			printf("\033[%d;%dH┃ ┗┛ ┃", nHeight + 2, nWriteWidth);
			printf("\033[%d;%dH┗━━┓ ┃", nHeight + 3, nWriteWidth);
			printf("\033[%d;%dH┏━━┛ ┃", nHeight + 4, nWriteWidth);
			printf("\033[%d;%dH┗━━━━┛", nHeight + 5, nWriteWidth);
			break;
		}
	}
}