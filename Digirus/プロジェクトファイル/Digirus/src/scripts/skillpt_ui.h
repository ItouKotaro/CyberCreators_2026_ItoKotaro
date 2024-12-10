//=============================================================
//
// スキルポイントUI [skillpt_ui.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _SKILLPT_UI_H_
#define _SKILLPT_UI_H_

#include "component.h"

// スキルポイントUI
class CSkillPointUI : public Component
{
public:
	void Init() override;			// 初期化
	void Uninit() override;			// 終了
	void Update() override;		// 更新
	void DrawUI() override;		// 描画
	void SetPoint(const int& nPercent);
	void SetAlpha(const float& fAlpha);
	static const float UI_SIZE;
	static const float UI_OUTLINE;
	static const float UI_EVERYPERCENT;
	static const int MAX_PARTICLE = 15;
	static const float MAX_UPSPEED_PARTICLE;
	static const float MIN_UPSPEED_PARTICLE;
	static const float MAX_SIZE_PARTICLE;
	static const float MIN_SIZE_PARTICLE;
private:
	LPDIRECT3DVERTEXBUFFER9 m_pMaskVtxBuff, m_pPointVtxBuff, m_pOutlineVtxBuff;
	LPDIRECT3DTEXTURE9 m_pMaskTexture, m_pPointTexture;

	// 泡パーティクル
	struct BubbleParticle
	{
		D3DXVECTOR3 pos, move;
		float fScale;
		float fDeepColor;
		bool bUse;
	};
	void InitParticle();			// 泡の初期化
	void UpdateParticle();		// 泡の更新
	void DrawParticle();		// 泡の描画
	void AddParticle();			// 泡の追加
	void UpdateMoveGage();	// ゲージの移動更新
	LPDIRECT3DVERTEXBUFFER9 m_pParticleVtxBuff;
	LPDIRECT3DTEXTURE9 m_pBubbleTexture;
	BubbleParticle m_bubbleParticle[MAX_PARTICLE];
	int m_nNextParticleCounter;

	float m_fPosGageY;
	int m_nSkillPt;
	float m_fAlpha;
};

#endif // !_SKILLPT_UI_H_
