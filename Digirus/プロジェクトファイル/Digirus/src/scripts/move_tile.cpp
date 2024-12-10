//=============================================================
//
// ˆÚ“®° [move_tile.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "move_tile.h"

//=============================================================
// [CMoveTile] ‰Šú‰»
//=============================================================
void CMoveTile::Init()
{
	m_move = { 0.0f, 0.0f, 0.0f };
	m_nMoveCount = 0;
	m_nMoveCounter = 0;
}

//=============================================================
// [CMoveTile] XV
//=============================================================
void CMoveTile::Update()
{
	// ˆÚ“®
	transform->Translate(m_move);

	// ‹t
	m_nMoveCounter++;
	if (m_nMoveCounter >= m_nMoveCount)
	{
		m_move *= -1;
		m_nMoveCounter = 0;
	}
}