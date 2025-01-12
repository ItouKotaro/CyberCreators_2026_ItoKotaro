//===================================================================================
//
// フィールド [field.h]
// Author: Ito Kotaro
//
//===================================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

// マクロ定義
#define BLOCK_FIELD_CHAR1	"━━━━━"	
#define BLOCK_FIELD_CHAR2	"━━━━━"
#define BLOCK_FIELD_CHAR3	"     "
#define BLOCK_PLAYER_CHAR1	"┏━━━┓"
#define BLOCK_PLAYER_CHAR2	"┃   ┃"
#define BLOCK_PLAYER_CHAR3	"┗━━━┛"

// フィールドクラスの定義
class CField
{
public:
	CField();	// コンストラクタ

	// ブロックの種類
	typedef enum
	{
		BLOCK_NONE = 0,		// なし
		BLOCK_FIELD,			// フィールド
		BLOCK_PLAYER,			// プレイヤー
		BLOCK_MAX
	}BLOCK;

	void Generate(int nScore);
	void Set(int nPos, BLOCK type);
	BLOCK Get(int nPos);
	void Draw(int nHeight);

	static const int FIELD_LENGTH = 25;				// フィールドの長さ
	static const int TILE_WIDTH = 5;					// 1マスの横幅
	static const int TILE_HEIGHT = 3;					// 1マスの高さ
	static const int FIELD_WIDTH = 35;					// 横のスタート位置
	static const int FIELD_HEIGHT = 45;				// 高さのスタート位置
	static const int MIN_DISTANCE = 2;					// 最小の離れている距離
	static const int MAX_SPACE = 4;						// 最大のスペース
	static const int SUPPORT_DISTANCE = 50;		// サポートする距離（難易度↓）
	static const int CHANCE_DISTANCE = 10;		// スペースが増えるチャンスの距離
private:
	BLOCK m_aBlock[FIELD_LENGTH];					// フィールド上のブロックデータ
};

#endif // !_FIELD_H_
