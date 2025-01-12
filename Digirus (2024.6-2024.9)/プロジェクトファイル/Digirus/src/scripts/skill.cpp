//=============================================================
//
// スキル [skill.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "skill.h"
#include "manager.h"
#include "scripts/player.h"
#include "scripts/skillpt.h"
#include "scripts/bug_attack.h"

// 静的メンバ変数の初期化
const int CHealSkill::HEAL_VALUE = 5;

//=============================================================
// [CSkillManager] 初期化
//=============================================================
void CSkillManager::Init()
{
	// 変数の初期化
	for (int i = 0; i < MAX_SLOT; i++)
	{
		m_pSkillSlot[i] = nullptr;
	}


}

//=============================================================
// [CSkillManager] 終了
//=============================================================
void CSkillManager::Uninit()
{
	// セットされているスキルを破棄する
	for (int i = 0; i < MAX_SLOT; i++)
	{
		delete m_pSkillSlot[i];
		m_pSkillSlot[i] = nullptr;
	}
}

//=============================================================
// [CSkillManager] 更新
//=============================================================
void CSkillManager::Update()
{
	CSkillPoint* pSkillPt = gameObject->GetComponent<CSkillPoint>();

	bool bPushSkillButton = false;
	for (int i = 0; i < MAX_SLOT; i++)
	{
		// スキルスロットにセットされているか
		if (m_pSkillSlot[i] == nullptr)
		{
			continue;
		}

		if (CInputSystem::GetInstance()->onInput("skill_" + std::to_string(i)))
		{ // スキル使用ボタンが入力されたとき
			// スキルポイントが足りるか
			if (m_pSkillSlot[i]->GetUsePoint() > pSkillPt->GetPoint())
			{ // 不足
				continue;
			}

			if (m_nPushCounter >= m_pSkillSlot[i]->GetPushTime())
			{ // 長押し時間に達したとき
				// ポイント消費
				pSkillPt->AddPoint(-m_pSkillSlot[i]->GetUsePoint());

				// スキル実行
				m_pSkillSlot[i]->Use();
				m_nPushCounter = 0;	// カウンターリセット
			}
			else
			{
				m_nPushCounter++;
			}

			bPushSkillButton = true;		// スキルボタンを押している
			break;
		}
	}

	// スキルボタンを押していなかったとき
	if (!bPushSkillButton)
	{
		m_nPushCounter = 0;	// カウンターリセット
	}

}

//=============================================================
// [CSkillManager] スキル設定
//=============================================================
void CSkillManager::SetSkill(const int& nSlotID, CSkill* skill)
{
	if (0 <= nSlotID && nSlotID < MAX_SLOT)
	{
		m_pSkillSlot[nSlotID] = skill;
	}
}

//=============================================================
// [CSkillManager] スキル取得
//=============================================================
CSkill* CSkillManager::GetSkill(const int& nSlotID)
{
	if (0 <= nSlotID && nSlotID < MAX_SLOT)
	{
		return m_pSkillSlot[nSlotID];
	}
	return nullptr;
}



//=============================================================
// [CSkill] コンストラクタ
//=============================================================
CSkill::CSkill() : m_nUsePoint(0), m_nPushTime(0)
{
	// プレイヤーを探す
	m_pPlayer = GameObject::Find("Player");
}

//=============================================================
// [CHealSkill] コンストラクタ
//=============================================================
CHealSkill::CHealSkill()
{
	m_nUsePoint = 20;
	m_nPushTime = 20;
}

//=============================================================
// [CHealSkill] 使用時
//=============================================================
void CHealSkill::Use()
{
	m_pPlayer->GetComponent<CPlayer>()->HealPlayer(HEAL_VALUE);
}

//=============================================================
// [CBugAttackSkill] コンストラクタ
//=============================================================
CBugAttackSkill::CBugAttackSkill()
{
	m_nUsePoint = 20;
	m_nPushTime = 10;
}

//=============================================================
// [CBugAttackSkill] 使用時
//=============================================================
void CBugAttackSkill::Use()
{
	D3DXVECTOR3 spawnPos = { 0.0f, 0.0f, 10.0f };
	D3DXVec3TransformCoord(&spawnPos, &spawnPos, &m_pPlayer->transform->GetMatrix());

	GameObject* pBugAttack = GameObject::LoadPrefab("data\\PREFAB\\mushi.pref",
		Transform(spawnPos,
			D3DXVECTOR3(0.0f, m_pPlayer->transform->GetWRotY(), 0.0f))
	);
	CBugAttack* pBug = pBugAttack->AddComponent<CBugAttack>();
}