//=============================================================
//
// 敵 [enemy.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "component.h"

// 敵（ベース）
class CEnemy : public Component
{
public:
	// 状態
	typedef enum
	{
		SEARCH = 0,			// 捜索（発見範囲に入るまで待機）
		FOUND,					// 発見
		ATTACK,				// 攻撃
		DAMAGE_MOVE		// 攻撃を受けた方向に移動
	}STATE;

	virtual void Init() override;								// 初期化
	virtual void Uninit() override;							// 終了
	virtual void Update() override;							// 更新（状態管理）

	// イベント
	virtual void OnDamage();								// ダメージを受けたときの処理
	virtual void OnDeath();									// 死亡時の処理
	virtual void OnFoundPlayer();							// プレイヤーを見つけたときの処理
	virtual void OnLostPlayer();								// プレイヤーを見失ったときの処理

	// 動作
	virtual void SearchAction();								// 捜索動作
	virtual void FoundAction();								// 発見動作
	virtual void AttackAction();								// 攻撃動作
	virtual void DamagedAction();							// 攻撃を受けたときの動作
	virtual void AttackDecision();							// 攻撃決定
	
	void DamageEnemy(const int& nDamage,							// ダメージを与える
		const	 D3DXVECTOR3& damagePos = {0.0f, 0.0f, 0.0f});		
	GameObject* GetSearchRange();										// 察知範囲オブジェクトの取得
	void AttractPlayer();										// プレイヤーに引き付ける

	// 設定項目
	void SetLife(const int& nLife);											// 体力の設定
	int GetLife();																	// 体力の取得
	void SetSpeed(const float& fSpeed);								// 速さの設定
	float GetSpeed();															// 速さの取得
	void SetAttack(const int& nAttack);									// 攻撃力の設定
	int GetAttack();																// 攻撃力の取得
	void SetAttackRange(const float& fRange);						// 攻撃範囲の設定
	void SetFoundDuration(const int& nDuration);					// 発見の継続時間の設定
	void SetAttackDuration(const int& nDuration);					// 攻撃の継続時間の設定
	void SetDamagedMoveDuration(const int& nDuration);		// 攻撃を受けた方向に移動する時間の設定
	int GetStateCounter() { return m_nStateCounter; }			// 状態カウンターの取得

	static const float KNOCKBACK_POWER;
	static const float ROTATION_SPEED;
private:
	void StateManager();										// 状態管理
	void UpdateAngle();										// 向きの更新

	// 基本情報
	int m_nLife;													// 体力
	float m_fSpeed;												// 速さ
	int m_nAttack;												// 攻撃力
	float m_fObjectiveAngle;									// 目標の向き
	float m_fCurrentAngle;									// 現在の向き

	// 察知関係のパラメータ
	GameObject* m_pSearchRange;						// 察知範囲
	float m_fAttackRange;									// 攻撃範囲
	int m_nFoundDuration;									// 発見状態の継続時間
	float m_damagedAngle;									// ダメージを受けた方向
	int m_nDamagedMoveDuration;						// 攻撃を受けた方向に移動する時間
	int m_nAttackDuration;									// 攻撃状態の継続時間

	// 状態
	STATE m_state;												// 状態
	int m_nStateCounter;										// 状態カウンター
	GameObject* m_pStateUI;								// 状態表示
};

// デバッグ敵
class CDebugEnemy : public CEnemy
{
public:
	void Init() override;				// 初期化
	void FoundAction() override;	// 発見動作
	void AttackAction() override;	// 攻撃動作
private:

};

// スライム
class CSlimeEnemy : public CEnemy
{
public:
	void Init() override;				// 初期化
	void FoundAction() override;	// 発見動作
	void AttackAction() override;	// 攻撃動作
private:
	int m_nJumpCounter;				// ジャンプカウンター
};

#endif // !_ENEMY_H_
