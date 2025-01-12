//=============================================================
//
// レーザーブロック [laser_block.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _LASER_BLOCK_H_
#define _LASER_BLOCK_H_

#include "component.h"

// レーザーブロック
class CLaserBlock : public Component
{
public:
	void Init() override;
	void Update() override;
	void SetLength(const float& fLength);
	void SetDamage(const int& nDamage) { m_nDamage = nDamage; }
	void SetMove(const D3DXVECTOR3& move) { m_move = move; }
	void SetMoveCounter(const int& nCount) { m_nMoveCount = nCount; }
private:
	GameObject* m_pLaserCollision;
	float m_fLength;							// レーザーの長さ
	int m_nDamage;							// ダメージ
	D3DXVECTOR3 m_move;				// 移動量
	int m_nMoveCount;						// 移動量カウント
	int m_nMoveCounter;						// 移動量カウンター
};

#endif // !_LASER_BLOCK_H_
