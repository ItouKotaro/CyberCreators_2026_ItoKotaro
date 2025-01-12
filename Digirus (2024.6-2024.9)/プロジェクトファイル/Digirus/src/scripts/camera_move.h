//=============================================================
//
// カメラの動き [camera_move.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _CAMERA_MOVE_H_
#define _CAMERA_MOVE_H_

#include "component.h"
#include "scripts/next_portal.h"

// カメラの動きクラス
class CCameraMove : public Component
{
public:
	// フォーカスの種類
	typedef enum
	{
		PLAYER = 0,	// プレイヤー
		PORTAL			// ポータル
	}FOCUS;

	// コンピュータフォーカスの状態
	typedef enum
	{
		INIT_POS = 0,			// 初期位置に移動
		GO_MOVE,					// 向かう
		NEXT_STAGE,			//	次のステージへ
		END,							// 終了
		GOAL,						// ゴール
	} PORTAL_PROGRESS;

	void Init() override;																// 初期化
	void Update() override;															// 更新
	void SetFocus(FOCUS focus);													// フォーカス対象の設定
	void SetNextPortal(CNextPortal* pNextPortal);							// ポータルの設定
	void SetShake(const int& nPower, const int& nTime);				// 振動
	void SetCameraPlayerOffset(const D3DXVECTOR3& offset) {		// プレイヤーからのオフセット
		m_playerCameraOffset = offset;
		m_nChangedCounter = CHANGING_TIME;
	}
	int GetChangedCounter() { return m_nChangedCounter; }
	
	FOCUS GetFocus() { return m_focus; }
	PORTAL_PROGRESS GetPortalProgress() { return m_portalProgress; }
	CNextPortal* GetNextPortal() { return m_portal; }

	static const int CHANGING_TIME;
	static const int CHANGE_TIMING;
private:
	void PlayerFocus();										// プレイヤーフォーカス
	void PortalFocus();										// ポータルフォーカス

	FOCUS m_focus;										// フォーカス対象
	PORTAL_PROGRESS m_portalProgress;		// ポータルの進行度
	CNextPortal* m_portal;								// ポータル
	D3DXVECTOR3 m_playerCameraOffset;		// プレイヤーフォーカス時のカメラのオフセット
	int m_nChangedCounter;							// チェンジカウンター
	int m_nShakePower;									// 振動の強さ
	int m_nShakeTime;									// 振動の時間
};

// カメラの位置を変えるためのコンポーネント
class CChangePlayerFocus : public Component
{
public:
	void OnTriggerStay(GameObject* other) override;
	void SetOffset(const D3DXVECTOR3& offset) { m_changedOffset = offset; }
private:
	D3DXVECTOR3 m_changedOffset;
};

#endif // !_CAMERA_MOVE_H_
