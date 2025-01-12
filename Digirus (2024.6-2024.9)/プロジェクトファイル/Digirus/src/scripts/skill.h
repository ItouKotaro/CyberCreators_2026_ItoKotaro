//=============================================================
//
// スキル [skill.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _SKILL_H_
#define _SKILL_H_

#include "component.h"

// スキル基底クラス
class CSkill
{
public:
	CSkill();								// コンストラクタ
	virtual void Use() = 0;		// スキル使用時の処理
	int GetUsePoint() {				// 消費ポイントの取得
		return m_nUsePoint; 
	}
	int GetPushTime() {			// 長押し時間の取得
		return m_nPushTime;
	}
protected:
	GameObject* m_pPlayer;	// プレイヤー
	int m_nUsePoint;				// 消費するポイント数
	int m_nPushTime;				// 使用時間（長押しし続ける時間）
};

// ヒールスキル
class CHealSkill : public CSkill
{
public:
	CHealSkill();
	void Use() override;					// スキル使用時の処理
	static const int HEAL_VALUE;		// 回復値
};


// バグ虫
class CBugAttackSkill : public CSkill
{
public:
	CBugAttackSkill();
	void Use() override;					// スキル使用時の処理
};


// スキル管理
class CSkillManager : public Component
{
public:
	void Init() override;											// 初期化
	void Uninit() override;											// 終了
	void Update() override;										// 更新
	void SetSkill(const int& nSlotID, CSkill* skill);		// スキルの設定
	CSkill* GetSkill(const int& nSlotID);						// スキルの取得

	static const int MAX_SLOT = 1;							// 最大スキルスロット
private:
	CSkill* m_pSkillSlot[MAX_SLOT];							// スキルスロット
	int m_nPushCounter;											// 長押しカウンター
};

#endif // !_SKILL_H_
