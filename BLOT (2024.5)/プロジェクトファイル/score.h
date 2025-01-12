//===================================================================================
//
// スコア [score.h]
// Author: Ito Kotaro
//
//===================================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

// マクロ定義
#define RANKING_PATH		"./data/ranking.bin"

// スコアクラスの定義
class CScore
{
public:
	CScore();							// コンストラクタ
	void Draw(int nHeight);		// 描画

	void AddScore(int nAdd) { m_nScore += nAdd; }		// スコアの加算
	void SetScore(int nScore) { m_nScore = nScore; }	// スコアの設定
	int GetScore() { return m_nScore; }						// スコアの取得
	int RegisterScore();												// スコアを登録

	// 定義
	static const int NUM_WIDTH = 6;				// 数字の横
	static const int NUM_HEIGHT = 6;				// 数字の高さ
	static const int NUM_SPACE = 1;					// 数字の間隔
	static const int SCORE_HEIGHT = 8;			// スコアの高さ
	static const int CENTER = 94;						// 中央
	static const int MAX_RANKING = 10;			// ランキングの最大数
	
private:
	int m_nScore;			// スコア
};

#endif // !_SCORE_H_
