//=============================================================
//
// スキルポイント [skillpt.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _SKILLPT_H_
#define _SKILLPT_H_

#include "component.h"
#include "skillpt_ui.h"

class CSkillPoint : public Component
{
public:
	void Init() override;							// 初期化
	void Update() override;						// 更新
	void AddPoint(const int& nAddPoint);	// ポイントを追加
	int GetPoint() { return m_nPoint; }		// ポイントの取得
	static const int MAX_POINT;				// 最大ポイント
private:
	void SetPoint(const int& nPoint);			// ポイントの設定
	int m_nPoint;										// ポイント
	GameObject* m_pUI;							// UI
};

#endif // !_SKILLPT_H_
