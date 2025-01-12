//===================================================================================
//
// ゲームマネージャー [game.h]
// Author: Ito Kotaro
//
//===================================================================================
#ifndef _GAME_H_
#define _GAME_H_

// インクルード
#include "player.h"
#include "field.h"
#include "score.h"

// ゲームマネージャークラスを定義
class CGameManager
{
public:
	CGameManager();												// コンストラクタ
	~CGameManager();											// デストラクタ
	void StartGame();												// ゲームを始める
	void Draw();														// 描画
	void HideDescription() { m_bStarted = true; }		// 説明を消す
	CField* GetField() { return m_pField; }				// フィールド取得

	// 定義
	static const int STAGEMOVE_INTERVAL = 50;			// ステージ移動の間隔
	static const int STAGEFALL_INTERVAL = 5;				// ステージから落下する間隔
	static const int STAGEFALL_COUNT = 80;					// どれぐらい落下するか
	static const int PLAYER_FALLSPEED = 15;				// プレイヤーの落下速度
	static const int RESULT_SCORE_HEIGHT = 27;			// スコア結果の高さ
	static const int RESULT_RANKING_WIDTH = 83;		// ランキングの横
	static const int RESULT_RANKING_HEIGHT = 36;		// ランキングの高さ
	static const int DESCRIPTION_WIDTH = 74;				// 説明の横
	static const int DESCRIPTION_HEIGHT = 20;			// 説明の高さ
private:
	void DescriptionDraw();	// 説明の描画
	void Step();					// 進む処理
	void GameOver();			// ゲームオーバー
	void ResetPos();				// 位置をリセット

	CPlayer* m_pPlayer;		// プレイヤー
	CField* m_pField;			// フィールド
	CScore* m_pScore;		// スコア

	bool m_bStarted;			// 開始済みか
	bool m_IsGameOver;		// ゲームオーバー状態か
};

#endif // !_GAME_H_
