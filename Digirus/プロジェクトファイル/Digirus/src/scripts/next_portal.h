//=============================================================
//
// 次なるポータル [next_portal.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _NEXT_PORTAL_H_
#define _NEXT_PORTAL_H_

#include "component.h"

// 次なるポータル
class CNextPortal : public Component
{
public:
	CNextPortal();
	void Init() override;
	void Uninit() override;
	void Update() override;
	void OnTriggerEnter(GameObject* other) override;
	void SetDisplaySize(const float& x, const float& y);
	void SetDisplayOffset(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);
	void SetNextPos(const D3DXVECTOR3& pos);
	void SetFrontStop(const float& fStopDistance) { m_fFrontStop = fStopDistance; }
	void SetDisplayAdjust(const float& x, const float& y) { m_displayAdjust = { x, y }; }
	void SetIsGoal(const bool& bGoal);
	bool GetIsGoal() { return m_bIsGoal; }
	float GetFrontStop() { return m_fFrontStop; }
	D3DXVECTOR2 GetDisplayAdjust() { return m_displayAdjust; }
	void ResetNextCamera();

	D3DXVECTOR3 GetNextPos() { return m_nextPos; }
	GameObject* GetDisplayObj() { return m_pDisplayObj; }
	GameObject* GetNextCamera() { return m_pNextCamera; }
private:
	GameObject* m_pDisplayObj;		// ディスプレイを表示するためのオブジェクト
	GameObject* m_pNextCamera;	// 行き先のカメラ
	D3DXVECTOR3 m_nextPos;			// 行き先
	float m_fFrontStop;
	D3DXVECTOR2 m_displayAdjust;
	bool m_bIsGoal;							// ゴールポータルか
};

#endif