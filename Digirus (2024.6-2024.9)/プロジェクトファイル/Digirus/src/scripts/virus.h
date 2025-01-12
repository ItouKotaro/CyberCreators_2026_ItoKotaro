//=============================================================
//
// ウイルス [virus.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _VIRUS_H_
#define _VIRUS_H_

#include "component.h"

class CVirus : public Component
{
public:
	CVirus(const int& nPoint = 1);										// コンストラクタ
	void Init() override;													// 初期化
	void Update() override;												// 更新
	void OnTriggerEnter(GameObject* other) override;		// 当たり判定
	void SetPoint(const int& nPoint);									// ポイントの設定
	static const float ATTRACT_DISTANCE;						// 引き付ける距離
	static const float ATTRACT_POWER;								// 引き付ける力
	static const float MIN_SIZE;										// 最小サイズ
	static const float MAX_SIZE;										//	最大サイズ
private:
	int m_nPoint;							// ポイント
	D3DXVECTOR3 m_move;		// 移動量
};

#endif // !_VIRUS_H_
