//=============================================================
//
// 移動床 [move_tile.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "move_tile.h"

//=============================================================
// [CMoveTile] 初期化
//=============================================================
void CMoveTile::Init()
{
	m_move = { 0.0f, 0.0f, 0.0f };
	m_nMoveCount = 0;
	m_nMoveCounter = 0;
}

//=============================================================
// [CMoveTile] 更新
//=============================================================
void CMoveTile::Update()
{
	// 移動
	transform->Translate(m_move);

	// 逆
	m_nMoveCounter++;
	if (m_nMoveCounter >= m_nMoveCount)
	{
		m_move *= -1;
		m_nMoveCounter = 0;
	}
}