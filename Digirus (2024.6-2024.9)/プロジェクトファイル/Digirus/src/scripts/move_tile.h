//=============================================================
//
// à⁄ìÆè∞ [move_tile.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _MOVE_TILE_H_
#define _MOVE_TILE_H_

#include "component.h"

// à⁄ìÆè∞
class CMoveTile : public Component
{
public:
	void Init() override;
	void Update() override;
	void SetMoveCount(const int& nCount) { m_nMoveCount = nCount; }
	void SetMove(const D3DXVECTOR3& move) { m_move = move; }
private:
	D3DXVECTOR3 m_move;
	int m_nMoveCount;
	int m_nMoveCounter;
};

#endif // !_MOVE_TILE_H_
