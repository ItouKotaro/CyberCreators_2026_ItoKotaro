//=============================================================
//
// ダメージボール [damage_ball.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _DAMAGE_BALL_H_
#define _DAMAGE_BALL_H_

#include "component.h"

// ダメージボールの出現
class CDamageBallEmitter : public Component
{
public:
	void Init() override;
	void Update() override;
private:
	int m_nLength;				// 長さ
	int m_nBetweenRand;		// ランダム差
	int m_nInterval;					// 間隔
	int m_nIntervalCounter;		// 間隔カウンター
	int m_nLife;						// 寿命
};

// ダメージボール
class CDamageBall : public Component
{
public:
	void Init() override;
	void Update() override;
	void OnTriggerEnter(GameObject* other) override;
	void SetLife(const int& nLife) { m_nLife = nLife; }
private:
	int m_nLife;		// 寿命
};

#endif // !_DAMAGE_BALL_H_
