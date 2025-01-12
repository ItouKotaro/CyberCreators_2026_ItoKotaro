//===================================================================================
//
// プレイヤー [player.h]
// Author: Ito Kotaro
//
//===================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

// マクロ定義
#define STACK_CHAR1	"┏━━━┓"
#define STACK_CHAR2	"┃   ┃"
#define STACK_CHAR3	"┗━━━┛"
#define STACK_LINK		"┣━━━┫"
#define PLAYER_CHAR1	"┏━━━┓"
#define PLAYER_CHAR2	"┫   ┣"
#define PLAYER_CHAR3	"┗┳━┳┛"

// プレイヤークラスの定義
class CPlayer
{
public:
	CPlayer();			// コンストラクタ
	~CPlayer();		// デストラクタ

	void InputStacks();					// 積み重ね処理
	void Draw(int nHeight);			// プレイヤーの描画

	int GetPos() { return m_nPos; }
	int GetStacks() { return m_nStacks; }

	void SetPos(int nPos) { m_nPos = nPos; }
	void SetStacks(int nStacks) { m_nStacks = nStacks; }

	// 定数定義
	static const int PLAYER_WIDTH = 35;				// 横のスタート位置
	static const int PLAYER_HEIGHT = 45;				// 高さのスタート位置

	static const int MAX_STACK = 20;					// スタックの最大数
	static const int STACK_INTERVAL = 300;			// スタックの間隔
	static const int STEP_INTERVAL = 250;			// 進むときの間隔
private:
	int m_nStacks;	// 積み重ねている数
	int m_nPos;		// 位置
};

#endif // !_PLAYER_H_
