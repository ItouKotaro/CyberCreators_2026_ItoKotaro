//===================================================================================
//
// ゲームマネージャー [game.cpp]
// Author: Ito Kotaro
//
//===================================================================================
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

// 変数の初期化
const int CGameManager::STAGEMOVE_INTERVAL;
const int CGameManager::STAGEFALL_INTERVAL;
const int CGameManager::STAGEFALL_COUNT;
const int CGameManager::PLAYER_FALLSPEED;
const int CGameManager::RESULT_SCORE_HEIGHT;
const int CGameManager::RESULT_RANKING_WIDTH;
const int CGameManager::RESULT_RANKING_HEIGHT;
const int CGameManager::DESCRIPTION_WIDTH;
const int CGameManager::DESCRIPTION_HEIGHT;

//=======================================================
// [CGameManager] コンストラクタ
//=======================================================
CGameManager::CGameManager() : m_IsGameOver(false), m_bStarted(false)
{
	 // プレイヤーを作成
	m_pPlayer = new CPlayer();

	// フィールドを作成
	m_pField = new CField();

	// スコアを作成
	m_pScore = new CScore();
}

//=======================================================
// [CGameManager] デストラクタ
//=======================================================
CGameManager::~CGameManager()
{
	// プレイヤーを破棄
	if (m_pPlayer != NULL)
	{
		delete m_pPlayer;
		m_pPlayer = NULL;
	}

	// フィールドを破棄
	if (m_pField != NULL)
	{
		delete m_pField;
		m_pField = NULL;
	}

	// スコアを破棄
	if (m_pScore != NULL)
	{
		delete m_pScore;
		m_pScore = NULL;
	}
}

//=======================================================
// [CGameManager] ゲーム開始
//=======================================================
void CGameManager::StartGame()
{
	while (1)
	{
		// 生成する
		m_pField->Generate(m_pScore->GetScore());

		// 描画
        Draw();

		// 積み重ねの操作処理
		m_pPlayer->InputStacks();

		// 進む
		Step();

		// ゲームオーバー判定
		if (m_IsGameOver)
		{
			GameOver();
			break;
		}
	}

}

//=======================================================
// [CGameManager] 描画
//=======================================================
void CGameManager::Draw()
{
	// プレイヤー領域をクリア
	for (int nCntBlock = 0; nCntBlock < CPlayer::MAX_STACK * CField::TILE_HEIGHT; nCntBlock++)
	{
		printf("\033[%d;%dH\033[2K", CPlayer::PLAYER_HEIGHT - nCntBlock, CPlayer::PLAYER_WIDTH);
	}

	// プレイヤーの描画
	m_pPlayer->Draw(CPlayer::PLAYER_HEIGHT);

	// フィールドの描画
	m_pField->Draw(CField::FIELD_HEIGHT);

	// スコアの描画
	m_pScore->Draw(CScore::SCORE_HEIGHT);

	// ルール説明の描画（進む前のみ）
	if (!m_bStarted)
	{
		DescriptionDraw();
	}
}

//=======================================================
// [CGameManager] 説明の描画
//=======================================================
void CGameManager::DescriptionDraw()
{
	printf("\033[%d;%dH", DESCRIPTION_HEIGHT, DESCRIPTION_WIDTH);
	printf("地面に届くようにブロックを積み上げるゲーム");
	printf("\033[%d;%dH", DESCRIPTION_HEIGHT + 3, DESCRIPTION_WIDTH + 8);
	printf("SPACEキー長押しで積み上げる");
}

//=======================================================
// [CGameManager] 進む
//=======================================================
void CGameManager::Step()
{
	int nStacks = m_pPlayer->GetStacks();
	int nPos = m_pPlayer->GetPos();
	m_bStarted = true;

	// 何ステップするか決める
	int nStepNum = nStacks + 1;

	for (int i = 0; i < nStepNum; i++)
	{
		// 移動
		nPos++;

		// ブロックを設置する
		if (nStacks > 0)
		{ // ブロックがあるのなら

			if (m_pField->Get(nPos) == CField::BLOCK_NONE)
			{ // ブロックがないとき
				m_pField->Set(nPos, CField::BLOCK_PLAYER);		// ブロック設置
			}
				
			nStacks--;		// ブロックを減らす
		}

		// プレイヤーの情報を更新する
		m_pPlayer->SetPos(nPos);
		m_pPlayer->SetStacks(nStacks);

		// 描画
		Draw();

		// 待つ
		Sleep(CPlayer::STEP_INTERVAL);
	}

	// 最終到着地点のブロックはなにか
	CField::BLOCK currentBlock = m_pField->Get(nPos);
	if (currentBlock == CField::BLOCK_NONE)
	{	// ブロックがないとき（ゲームオーバー）
		m_IsGameOver = true;
		return;
	}

	// スコアを加算する
	m_pScore->AddScore(nPos);

	// 現在地点の位置を0まで移動させる
	ResetPos();
}

//=======================================================
// [CGameManager] ゲームオーバー
//=======================================================
void CGameManager::GameOver()
{
	// ランキングに登録する
	int nRank = m_pScore->RegisterScore();

	for (int i = 0; i < STAGEFALL_COUNT; i++)
	{
		// プレイヤー領域をクリア
		for (int nCntBlock = 0; nCntBlock < CPlayer::MAX_STACK * CField::TILE_HEIGHT + CField::TILE_HEIGHT; nCntBlock++)
		{
			printf("\033[%d;%dH\033[2K", CPlayer::PLAYER_HEIGHT + CField::TILE_HEIGHT - nCntBlock, CPlayer::PLAYER_WIDTH);
		}

		// プレイヤーの描画
		m_pPlayer->Draw(CPlayer::PLAYER_HEIGHT - i / PLAYER_FALLSPEED);

		// フィールドの描画
		if (CField::FIELD_HEIGHT - i > 0)
			m_pField->Draw(CField::FIELD_HEIGHT - i);

		// 最後スコアの描画
		if (i >= STAGEFALL_COUNT - RESULT_SCORE_HEIGHT)
			m_pScore->Draw(i - (STAGEFALL_COUNT - RESULT_SCORE_HEIGHT));

		// ランキングの描画
		if (nRank != -1)
		{ // ランクインしているとき
			if (i >= STAGEFALL_COUNT - RESULT_RANKING_HEIGHT)
				printf("\033[%d;%dH%d位にランクインしました", i - (STAGEFALL_COUNT - RESULT_RANKING_HEIGHT), RESULT_RANKING_WIDTH, nRank);
		}

		// 待つ
		Sleep(STAGEFALL_INTERVAL);
	}
}

//=======================================================
// [CGameManager] 位置をリセット
//=======================================================
void CGameManager::ResetPos()
{
	// プレイヤーの位置を取得
	int nPos = m_pPlayer->GetPos();

	for (int i = 1; i <= nPos; i++)
	{
		// プレイヤーの位置を減らしていく
		m_pPlayer->SetPos(nPos - i);

		// ブロックをずらす
		for (int nCntBlock = 0; nCntBlock < CField::FIELD_LENGTH - 1; nCntBlock++)
		{
			m_pField->Set(nCntBlock, m_pField->Get(nCntBlock + 1));
		}
		m_pField->Set(CField::FIELD_LENGTH - 1, CField::BLOCK_NONE);

		// 描画する
		Draw();

		// 移動間隔
		Sleep(STAGEMOVE_INTERVAL);
	}
}