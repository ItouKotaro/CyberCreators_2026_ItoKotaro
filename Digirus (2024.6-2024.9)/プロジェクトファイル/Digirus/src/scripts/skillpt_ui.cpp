//=============================================================
//
// スキルポイントUI [skillpt_ui.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "skillpt_ui.h"
#include "manager.h"
#include "skillpt.h"

// 静的メンバ変数の初期化
const float CSkillPointUI::UI_SIZE = 250.0f;
const float CSkillPointUI::UI_OUTLINE = 0.06f;
const float CSkillPointUI::UI_EVERYPERCENT = CSkillPointUI::UI_SIZE / CSkillPoint::MAX_POINT;
const int CSkillPointUI::MAX_PARTICLE;
const float CSkillPointUI::MAX_UPSPEED_PARTICLE = 0.8f;
const float CSkillPointUI::MIN_UPSPEED_PARTICLE = 0.4f;
const float CSkillPointUI::MAX_SIZE_PARTICLE = 22.0f;
const float CSkillPointUI::MIN_SIZE_PARTICLE = 15.0f;

//=============================================================
// [CSkillPointUI] 初期化
//=============================================================
void CSkillPointUI::Init()
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	// デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pMaskVtxBuff, nullptr);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pPointVtxBuff, nullptr);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pOutlineVtxBuff, nullptr);
	VERTEX_2D* pVtx; // 頂点情報へのポインタ

	for (int i = 0; i < 3; i++)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		if (i == 0) m_pMaskVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		else if (i == 1) m_pPointVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		else m_pOutlineVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-UI_SIZE * 0.5f, -UI_SIZE * 0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(UI_SIZE * 0.5f, -UI_SIZE * 0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-UI_SIZE * 0.5f, UI_SIZE * 0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(UI_SIZE * 0.5f, UI_SIZE * 0.5f, 0.0f);

		if (i == 2)
		{ // アウトライン
			pVtx[0].pos *= 1.0f + UI_OUTLINE;
			pVtx[1].pos *= 1.0f + UI_OUTLINE;
			pVtx[2].pos *= 1.0f + UI_OUTLINE;
			pVtx[3].pos *= 1.0f + UI_OUTLINE;
		}

		// 位置
		pVtx[0].pos += transform->GetWPos();
		pVtx[1].pos += transform->GetWPos();
		pVtx[2].pos += transform->GetWPos();
		pVtx[3].pos += transform->GetWPos();

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点バッファをアンロックする
		if (i == 0) m_pMaskVtxBuff->Unlock();
		else if (i == 1) m_pPointVtxBuff->Unlock();
		else m_pOutlineVtxBuff->Unlock();
	}

	// パーティクルの初期化
	InitParticle();

	// テクスチャの読み込み
	m_pMaskTexture = CManager::GetDataManager()->RefTexture("data\\TEXTURE\\SKILL\\mask.png")->GetTexture();
	m_pPointTexture = CManager::GetDataManager()->RefTexture("data\\TEXTURE\\SKILL\\skillpoint.png")->GetTexture();
	m_pBubbleTexture = CManager::GetDataManager()->RefTexture("data\\TEXTURE\\SKILL\\bubble.png")->GetTexture();

	// 変数の初期化
	m_fPosGageY = UI_SIZE;
	m_fAlpha = 1.0f;
}

//=============================================================
// [CSkillPointUI] 終了
//=============================================================
void CSkillPointUI::Uninit()
{
	// 頂点バッファの破棄
	if (m_pMaskVtxBuff != nullptr)
	{
		m_pMaskVtxBuff->Release();
		m_pMaskVtxBuff = nullptr;
	}
	if (m_pPointVtxBuff != nullptr)
	{
		m_pPointVtxBuff->Release();
		m_pPointVtxBuff = nullptr;
	}
	if (m_pOutlineVtxBuff != nullptr)
	{
		m_pOutlineVtxBuff->Release();
		m_pOutlineVtxBuff = nullptr;
	}
	if (m_pParticleVtxBuff != nullptr)
	{
		m_pParticleVtxBuff->Release();
		m_pParticleVtxBuff = nullptr;
	}
}

//=============================================================
// [CSkillPointUI] 更新
//=============================================================
void CSkillPointUI::Update()
{
	// パーティクルの出現
	m_nNextParticleCounter--;
	if (m_nNextParticleCounter <= 0)
	{
		AddParticle();

		if (rand() % 5 <= 1)
		{
			m_nNextParticleCounter = rand() % 30 + 20;
		}
		else
		{
			m_nNextParticleCounter = rand() % 20 + 70;
		}
	}

	// ゲージの更新
	UpdateMoveGage();

	// パーティクルの更新
	UpdateParticle();
}

//=============================================================
// [CSkillPointUI] 描画
//=============================================================
void CSkillPointUI::DrawUI()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	DWORD dwCurZTest, dwCurZFunc;

	// ステンシルのクリア
	pDevice->Clear(0, nullptr, D3DCLEAR_STENCIL, D3DCOLOR_ARGB(0, 170, 170, 170), 1.0f, 0);

	// マスク -------------------------------------------------------------------------------------------------------------

	// Zバッファの設定を変更するので現在の状態を保存しておく
	pDevice->GetRenderState(D3DRS_ZENABLE, &dwCurZTest);
	pDevice->GetRenderState(D3DRS_ZFUNC, &dwCurZFunc);

	// Zバッファに書き込まないようにします
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);

	// ステンシルバッファの設定です
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);  // ステンシルバッファ有効
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xfe);
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS); // ステンシルテストは常に行う
	
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
	
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x02);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pMaskVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pMaskTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
		0, //描画する最初の頂点インデックス
		2); //描画するプリミティブ数


	// ゲージ描画 -------------------------------------------------------------------------------------------------------------

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xfe);

	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x03);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pPointVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pPointTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
		0, //描画する最初の頂点インデックス
		2); //描画するプリミティブ数


	// パーティクル -------------------------------------------------------------------------------------------------

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x03);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xfe);

	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

	// パーティクルの描画
	DrawParticle();



	// アウトライン -------------------------------------------------------------------------------------------------

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xfe);

	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pOutlineVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pMaskTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
		0, //描画する最初の頂点インデックス
		2); //描画するプリミティブ数

	// 元に戻す --------------------------------------------------------------------------------------------------
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZENABLE, dwCurZTest);
	pDevice->SetRenderState(D3DRS_ZFUNC, dwCurZFunc);

	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x01);
}

//=============================================================
// [CSkillPointUI] 透明度の設定
//=============================================================
void CSkillPointUI::SetAlpha(const float& fAlpha)
{
	m_fAlpha = fAlpha;

	VERTEX_2D* pVtx; // 頂点情報へのポインタ

	for (int i = 0; i < 3; i++)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		if (i == 0) m_pMaskVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		else if (i == 1) m_pPointVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		else m_pOutlineVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラー
		D3DXCOLOR color = pVtx[0].col;
		color.a = fAlpha;
		pVtx[0].col = color;
		pVtx[1].col = color;
		pVtx[2].col = color;
		pVtx[3].col = color;

		// 頂点バッファをアンロックする
		if (i == 0) m_pMaskVtxBuff->Unlock();
		else if (i == 1) m_pPointVtxBuff->Unlock();
		else m_pOutlineVtxBuff->Unlock();
	}
}

//=============================================================
// [CSkillPointUI] 進行度の設定
//=============================================================
void CSkillPointUI::SetPoint(const int& nPercent)
{
	if (0 <= nPercent && nPercent <= CSkillPoint::MAX_POINT)
	{
		m_nSkillPt = nPercent;
	}
}

//=============================================================
// [CSkillPointUI] パーティクルの初期化
//=============================================================
void CSkillPointUI::InitParticle()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTICLE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pParticleVtxBuff, nullptr);

	VERTEX_2D* pVtx; // 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pParticleVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		pVtx[0].pos += transform->GetWPos();
		pVtx[1].pos += transform->GetWPos();
		pVtx[2].pos += transform->GetWPos();
		pVtx[3].pos += transform->GetWPos();

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 155);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	m_pParticleVtxBuff->Unlock();


	// 変数の初期化
	m_nNextParticleCounter = 0;
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		m_bubbleParticle[i].pos = { 0.0f, 0.0f, 0.0f };
		m_bubbleParticle[i].move = { 0.0f, 0.0f, 0.0f };
		m_bubbleParticle[i].fDeepColor = 0.0f;
		m_bubbleParticle[i].fScale = 0.0f;
		m_bubbleParticle[i].bUse = false;
	}
}

//=============================================================
// [CSkillPointUI] パーティクルの更新
//=============================================================
void CSkillPointUI::UpdateParticle()
{
	VERTEX_2D* pVtx; // 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pParticleVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		if (m_bubbleParticle[i].bUse)
		{ // パーティクルが使われているとき

			// 移動量を加算する
			m_bubbleParticle[i].pos += m_bubbleParticle[i].move;

			// ゲージ以上の高さまで到達したとき破棄
			if (m_bubbleParticle[i].pos.y < transform->GetWPos().y - UI_SIZE)
			{
				m_bubbleParticle[i].bUse = false;
			}

			// 透明度を上限にする
			if (m_fAlpha < m_bubbleParticle[i].fDeepColor)
			{
				m_bubbleParticle[i].fDeepColor = m_fAlpha;
			}

			// 情報を更新する
			pVtx[0].pos = D3DXVECTOR3(-m_bubbleParticle[i].fScale * 0.5f, -m_bubbleParticle[i].fScale * 0.5f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(m_bubbleParticle[i].fScale * 0.5f, -m_bubbleParticle[i].fScale * 0.5f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-m_bubbleParticle[i].fScale * 0.5f, m_bubbleParticle[i].fScale * 0.5f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(m_bubbleParticle[i].fScale * 0.5f, m_bubbleParticle[i].fScale * 0.5f, 0.0f);

			pVtx[0].pos +=m_bubbleParticle[i].pos;
			pVtx[1].pos +=m_bubbleParticle[i].pos;
			pVtx[2].pos +=m_bubbleParticle[i].pos;
			pVtx[3].pos +=m_bubbleParticle[i].pos;

			D3DXCOLOR col = pVtx[0].col;
			col.a = m_bubbleParticle[i].fDeepColor;
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;
		}
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	m_pParticleVtxBuff->Unlock();
}

//=============================================================
// [CSkillPointUI] パーティクルの描画
//=============================================================
void CSkillPointUI::DrawParticle()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pParticleVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		if (m_bubbleParticle[i].bUse)
		{ // パーティクルが使われているとき
			// テクスチャの設定
			pDevice->SetTexture(0, m_pBubbleTexture);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
		}
	}
}

//=============================================================
// [CSkillPointUI] パーティクルの追加
//=============================================================
void CSkillPointUI::AddParticle()
{
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		if (!m_bubbleParticle[i].bUse)
		{ // パーティクルが使われていないとき
			srand((unsigned int)clock());

			// 出現位置を決める
			m_bubbleParticle[i].pos = transform->GetWPos();
			m_bubbleParticle[i].pos.x += fmod(rand(), UI_SIZE) - UI_SIZE * 0.5f;
			m_bubbleParticle[i].pos.y += UI_SIZE * 0.5f;

			// 移動量を決める
			m_bubbleParticle[i].move.x = 0.0f;
			m_bubbleParticle[i].move.y = fmod(rand(), MAX_UPSPEED_PARTICLE - MIN_UPSPEED_PARTICLE) + MIN_UPSPEED_PARTICLE;
			m_bubbleParticle[i].move.y *= -1;

			// サイズを決める
			m_bubbleParticle[i].fScale = fmod(rand(), MAX_SIZE_PARTICLE - MIN_SIZE_PARTICLE) + MIN_SIZE_PARTICLE;

			// 色の深みを決める
			m_bubbleParticle[i].fDeepColor = rand() % 50 * 0.01f + 0.3f;

			m_bubbleParticle[i].bUse = true;		// 使用状態に変更
			break;
		}
	}
}

//=============================================================
// [CSkillPointUI] ゲージの移動更新
//=============================================================
void CSkillPointUI::UpdateMoveGage()
{
	VERTEX_2D* pVtx; // 頂点情報へのポインタ

	// 到達目標位置を計算する
	float objectivePosY = UI_EVERYPERCENT * (CSkillPoint::MAX_POINT - m_nSkillPt);

	// 移動する
	m_fPosGageY += (objectivePosY - m_fPosGageY) * 0.08f;


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pPointVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-UI_SIZE * 0.5f, -UI_SIZE * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(UI_SIZE * 0.5f, -UI_SIZE * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-UI_SIZE * 0.5f, UI_SIZE * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(UI_SIZE * 0.5f, UI_SIZE * 0.5f, 0.0f);

	// 位置の加算
	pVtx[0].pos += transform->GetWPos();
	pVtx[1].pos += transform->GetWPos();
	pVtx[2].pos += transform->GetWPos();
	pVtx[3].pos += transform->GetWPos();

	// パーセントに対応させる
	
	pVtx[0].pos.y += m_fPosGageY;
	pVtx[1].pos.y += m_fPosGageY;
	pVtx[2].pos.y += m_fPosGageY;
	pVtx[3].pos.y += m_fPosGageY;

	// 頂点バッファをアンロックする
	m_pPointVtxBuff->Unlock();
}