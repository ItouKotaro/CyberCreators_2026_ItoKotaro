//=============================================================
//
// 破壊エフェクト [break_effect.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _BREAK_EFFECT_H_
#define _BREAK_EFFECT_H_

#include "component.h"

// マクロ定義
#define DEF_BREAKEFFECT_LIFE		(120)
#define DEF_BREAKEFFECT_SCALE		(1.0f)

// 破壊エフェクトコンポーネント
class CBreakEffect : public Component
{
public:
	CBreakEffect(const int& nLife = DEF_BREAKEFFECT_LIFE, const float& fScale = DEF_BREAKEFFECT_SCALE);
	void Init() override;
	void Update() override;

	static const std::string MESH_PATH;
private:
	int m_nLife;								// 寿命
	float m_fScale;							// スケール
	GameObject* m_pPickupObj;		// 拾う範囲
};

#endif // !_BREAK_EFFECT_H_
