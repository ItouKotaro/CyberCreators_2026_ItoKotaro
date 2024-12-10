//=============================================================
//
// プレイヤー [player.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "component.h"
#include "internal/physics.h"

// プレイヤークラス
class CPlayer : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DamagePlayer(const int& nDamage);
	void HealPlayer(const int& nHeal);
	int GetLife() { return m_nLife; }
	GameObject* GetHPBar() { return m_pHPBarObj; }
	GameObject* GetStaminaBar() { return m_pStaminaBarObj; }
	void SetSavePoint(const D3DXVECTOR3& pos) { m_savePoint = pos; }

	void OnDeath();											// 死亡時の処理

	static const float MOVE_SPEED;					// 移動速度
	static const float DASH_RATE;						// ダッシュによる上昇率
	static const int REVIVAL;								// 復活回数
	static const int DEF_LIFE;							// 初期体力
	static const int DEF_STAMINA;					// 初期スタミナ
	static const float ROTATION_SPEED;			// 回転速度

	// 消費スタミナ
	static const int DASH_STAMINA;					// ダッシュの消費スタミナ
	static const int ROLLING_STAMINA;				// 回転の消費スタミナ
private:
	enum STATE
	{
		STAY,		// 待機
		WALK,		// 歩く
		ATTACK,	// 攻撃
		ROLL,		// 回転
	};

	void MoveControl();									// 移動操作
	float GetMoveAngle();									// 動く方向の角度を取得する
	void AttackControl();									// 攻撃操作
	void AttackAction();									// 攻撃動作
	void OnDamage();										// ダメージを受けたときの処理
	void UpdateAngle();									// 向きの更新
	void UpdateState();									// 状態の更新
	void FallJudge();											// 落下判定（壁にくっつかないため）
	void RollingAction();									// 回転
	void UpdateStamina();								// スタミナの更新

	GameObject* m_pAttackCollision;				// 攻撃範囲
	GameObject* m_pReachCollision;				// リーチ範囲
	GameObject* m_pWeaponObj;					// 武器
	GameObject* m_pHPBarObj;						// 体力バー
	GameObject* m_pStaminaBarObj;				// スタミナバー
	GameObject* m_pRevivalText;					// 復活回数UI
	int m_nLife;												// 体力
	int m_nStamina;										// スタミナ
	float m_fObjectiveAngle;								// 目標の向き
	float m_fCurrentAngle;								// 現在の向き
	int m_nParticleCounter;								// パーティクルのカウンター
	bool m_bDash;											// ダッシュ
	bool m_bIsFloor;										// 地面か
	int m_nRevival;											// 復活ライフ
	D3DXVECTOR3 m_savePoint;						// セーブポイント

	// モーション
	STATE m_state;											// 状態
	int m_nStateCounter;									// 状態カウンター

	// 攻撃関係
	int m_nChainAttack;									// 連続

};

#endif // !_PLAYER_H_
