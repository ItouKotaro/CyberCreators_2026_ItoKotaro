//=============================================================
//
// スキルポイント [skillpt.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "skillpt.h"

// 静的メンバ変数の初期化
const int CSkillPoint::MAX_POINT = 100;

//=============================================================
// [CSkillPoint] 初期化
//=============================================================
void CSkillPoint::Init()
{
	// UI用のゲームオブジェクトを作成
	m_pUI = new GameObject();
	m_pUI->transform->SetPos(150.0f, 150.0f, 0.0f);
	m_pUI->SetName("SkillPointUI");
	m_pUI->SetTag("UI");
	m_pUI->AddComponent<CSkillPointUI>();

	// 初期ポイントを設定
	SetPoint(0);
}

//=============================================================
// [CSkillPoint] 更新
//=============================================================
void CSkillPoint::Update()
{
	
}

//=============================================================
// [CSkillPoint] ポイントの設定
//=============================================================
void CSkillPoint::AddPoint(const int& nAddPoint)
{
	int nAddedPoint = m_nPoint + nAddPoint;	// 加算後のポイント

	if (nAddedPoint > MAX_POINT)
	{ // 最大ポイントを超えているとき
		SetPoint(MAX_POINT);
	}
	else if (nAddedPoint < 0)
	{ // 0を下回っているとき
		SetPoint(0);
	}
	else
	{ // 通常加算
		SetPoint(nAddedPoint);
	}
}

//=============================================================
// [CSkillPoint] ポイントの設定
//=============================================================
void CSkillPoint::SetPoint(const int& nPoint)
{
	if (0 <= nPoint && nPoint <= MAX_POINT)
	{
		m_nPoint = nPoint;
	}

	// UIを更新する
	m_pUI->GetComponent<CSkillPointUI>()->SetPoint(m_nPoint);
}