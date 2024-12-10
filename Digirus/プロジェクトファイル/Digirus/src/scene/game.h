//=============================================================
//
// ゲームシーン [game.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "scene.h"
#include "gameobject.h"
#include "world_loader.h"
#include "scripts/pause.h"

// ステージローダー
class StageLoader : public WorldLoader
{
public:
	void CustomObject(GameObject* gameObject) override;
};

// ゲームシーン
class CGameScene : public CScene
{
public:
	void Init() override;			// 初期化
	void Uninit() override;			// 終了
	void Update() override;		// 更新
	void Draw() override;			// 描画
	void SetWorld(const std::string& path);
	void SetLimitTime(const int& nSec);
	void SetAmbient(const float& fAmbient);
	float GetAmbient() { return m_fAmbient; }
	void SetSkybox(const std::string& sPath);
	std::string GetSkybox() { return m_sSkyboxPath; }
	void SetBGM(const std::string& sPath, const float& fVolume);
	void onInitGoal();
	void FinishGoal();
	void UpdateGoal();
	void AddScore(const int& nScore) { m_nScore += nScore; }
	LPDIRECT3DTEXTURE9 GetResultTexture();
	void OnGameOver();
	bool IsGoal() { return m_bIsGoal; }
	GameObject* GetClockSound() { return m_pClockSound; }

	void SavePlayData();
	int GetBestTime();

	static const int RESULT_BAR_SPACE;
	static const D3DXVECTOR2 TIME_POS;
private:
	void InitCamera();				// カメラの初期化
	void InitLight();					// ライトの初期化
	void InitResult();				// リザルトの初期化
	void UpdateResult();			// リザルトの更新
	void CenterCosor();			// カーソルを中央に移動する処理
	void UpdateTime();				// 時間の更新
	void UpdateStartAnim();		// スタートアニメーション（フェードとステージ名と制限時間）

	// プレイデータ
	struct PlayData
	{
		unsigned int nClearTime;
		unsigned int nBestScore;
	};

	// ゲーム情報
	char m_sWorldPath[MAX_PATH];		// ワールドのパス
	char m_sWorldName[MAX_PATH];	// ワールド名
	char m_sSkyboxPath[MAX_PATH];	// スカイボックスのパス
	char m_sBGMPath[MAX_PATH];		// BGMのパス
	float m_fBGMVolume;						// BGMの音量
	float m_fAmbient;							// 明るさ

	// ゲーム要素
	GameObject* m_pPause;			// ポーズ
	GameObject* m_pClockSound;	// 時間制限SE
	GameObject* m_pFade;				// フェード
	GameObject* m_pBGM;				// BGM
	GameObject* m_pCamera;			// カメラ
	GameObject* m_pSkillGuide;		// スキルのキー表示
	StageLoader* m_pStageLoader;	// ステージローダー

	// タイトル表示
	GameObject* m_pTitleName;		// タイトル名（序盤）
	int m_nTitleCounter;					// タイトル表示時間
	int m_nTitleTimeCounter;			// タイムタイトル表示時間
	float m_fTitleAlpha;					// タイトル表示の透明度

	// 時間関係
	GameObject* m_pTimeUI;			// 時間制限UI
	int m_nCurrentTime;
	int m_nLimitTime;						// 制限時間
	int m_nClearTime;						// クリアタイム（ミリ秒）
	bool m_bIsGoal;							// ゴール
	int m_nScore;							// スコア
	float m_fScoreFade;					// スコアのフェード

	// リザルト
	GameObject* m_pResultRender;
	GameObject* m_pScoreText;			// スコアのテキスト
	GameObject* m_pClearTimeText;	// クリアタイムのテキスト
	GameObject* m_pBestTimeText;		// ベストタイムのテキスト
	float m_fAlphaUI;

	// ゲームオーバー
	bool m_bGameOver;						// ゲームオーバー
};

#endif // !_GAME_H_
