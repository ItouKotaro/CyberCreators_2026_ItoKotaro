//=============================================================
//
// レンダラー [renderer.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "renderer.h"
#include "gameobject.h"
#include "component/3d/camera.h"
#include "component/3d/render_texture.h"
#include "component/other/render_objects_texture.h"

// 変数の初期化
const int CRenderer::SCREEN_WIDTH = 1920;
const int CRenderer::SCREEN_HEIGHT = 1080;
const bool CRenderer::USE_FOG = false;

//=============================================================
// [CRenderer] コンストラクタ
//=============================================================
CRenderer::CRenderer()
{
	// 変数の初期化
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;
	m_pShadow = nullptr;
	m_bFullScreen = true;
	m_fFogStartPos = 200.0f;
	m_fFogEndPos = 1000.0f;
}

//=============================================================
// [CRenderer] デストラクタ
//=============================================================
CRenderer::~CRenderer()
{

}

//=============================================================
// [CRenderer] 初期化
//=============================================================
HRESULT CRenderer::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;						// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;		// プレゼンテーションパラメータ

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));														// パラメータのゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;												// ゲーム画面サイズ（幅）
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;											// ゲーム画面サイズ（高さ）
	d3dpp.BackBufferFormat = d3ddm.Format;												// バックバッファの形式
	d3dpp.BackBufferCount = 1;																	// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;									// ダブルバッファの切り替え（映像信号に同期）
	d3dpp.EnableAutoDepthStencil = TRUE;													// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;									// デプスバッファ
	d3dpp.Windowed = bWindow;																	// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;				// インターバル

	// Direct3Dデバイスの生成（描画処理と頂点処理をハードウェアで行う）
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
	{
		// Direct3Dデバイスの生成（描画処理はハードウェアで頂点処理はCPUで行う）
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
		{
			// Direct3Dデバイスの生成（描画処理と頂点処理をCPUで行う）
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// ハンドルの代入
	m_hwnd = hWnd;

	// 影の生成
	if (CShadow::USE_SHADOW)
	{
		m_pShadow = new CShadow();
		if (!m_pShadow->CheckShadow(m_pD3D, m_pD3DDevice, d3dpp))
		{ // 失敗
			return E_FAIL;
		}
		m_pShadow->Init();
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// フォグ
	if (USE_FOG)
	{
		m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
		m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&m_fFogStartPos));
		m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&m_fFogEndPos));
	}


	return S_OK;
}

//=============================================================
// [CRenderer] 終了
//=============================================================
void CRenderer::Uninit()
{
	// 影の破棄
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		delete m_pShadow;
		m_pShadow = nullptr;
	}

	// Direct3Dデバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

//=============================================================
// [CRenderer] 更新
//=============================================================
void CRenderer::Update()
{
	// すべてのオブジェクトを更新する
	GameObject::UpdateAll();

	// フルスクリーン切り替え
	if (INPUT_INSTANCE->onTrigger("f11"))
	{
		SetFullScreen(!GetFullScreen());
	}
}

//=============================================================
// [CRenderer] 描画
//=============================================================
void CRenderer::Draw()
{
	// 描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{ // 描画開始が成功した場合

		// 画面クリア（バッファクリア＆Zバッファクリア）
		m_pD3DDevice->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);

		// カメラのコンポーネントを取得する
		std::vector<CCamera*> cameraComponents = Component::GetComponents<CCamera>(false, true);

		for (int nCntCamera = 0; nCntCamera < cameraComponents.size(); nCntCamera++)
		{
			// カメラを取得
			CCamera* pCamera = cameraComponents.at(nCntCamera);

			if (!pCamera->GetVisible())
			{
				continue;	// 表示しないときは処理をスキップ
			}

			// 画面クリア（バッファクリア＆Zバッファクリア）
			m_pD3DDevice->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), pCamera->GetColor(), 1.0f, 0);

			// カメラをセット
			pCamera->SetCamera();
			pCamera->GetSkybox()->Draw();

			// オブジェクト描画前処理
			GameObject::BeforeDrawAll();

			// 3Dオブジェクトの描画（影使用時）
			if (CShadow::USE_SHADOW)
			{
				m_pShadow->Draw(pCamera);
			}

			// ゲームオブジェクトの描画処理
			GameObject::DrawAll();
		}

		// ゲームオブジェクトのUI描画処理
		GameObject::DrawUIAll();

		// 描画終了
		m_pD3DDevice->EndScene();
	}

	// レンダーテクスチャ --------------------------------------------------------------------------------------------------------------------

	// コンポーネントを取得する
	std::vector<CRenderTexture*> renderComponents = Component::GetComponents<CRenderTexture>(false, true);

	// 前回の設定として保存しておく
	LPDIRECT3DSURFACE9 pBeforeBufferSurface;
	LPDIRECT3DSURFACE9 pBeforeDepthSurface;
	m_pD3DDevice->GetRenderTarget(0, &pBeforeBufferSurface);
	m_pD3DDevice->GetDepthStencilSurface(&pBeforeDepthSurface);

	for (int nCntRender = 0; nCntRender < renderComponents.size(); nCntRender++)
	{
		if (renderComponents[nCntRender]->BeginRender())
		{
			// カメラを取得
			CCamera* pCamera = renderComponents[nCntRender]->GetTargetCamera();

			// 画面クリア（バッファクリア＆Zバッファクリア）
			m_pD3DDevice->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), pCamera->GetColor(), 1.0f, 0);

			// カメラをセット
			pCamera->SetCamera();
			pCamera->GetSkybox()->Draw();

			// オブジェクト描画前処理
			GameObject::BeforeDrawAll();

			// 3Dオブジェクトの描画（影使用時）
			if (CShadow::USE_SHADOW)
			{
				m_pShadow->Draw(pCamera);
			}

			// ゲームオブジェクトの描画処理
			GameObject::DrawAll();

			renderComponents[nCntRender]->EndRender();
		}
	}

	// 元の設定に戻す
	m_pD3DDevice->SetDepthStencilSurface(pBeforeDepthSurface);

	// レンダーオブジェクトテクスチャ ----------------------------------------------------------------------------------------------------------

	// コンポーネントを取得する
	std::vector<CRenderObjectsTexture*> renderObjectsComponents = Component::GetComponents<CRenderObjectsTexture>(false, true);

	// 描画
	for (int nCntRender = 0; nCntRender < renderObjectsComponents.size(); nCntRender++)
	{
		renderObjectsComponents[nCntRender]->DrawTexture();
	}

	// 元のレンダーターゲットを設定する
	m_pD3DDevice->SetRenderTarget(0, pBeforeBufferSurface);

	// ------------------------------------------------------------------------------------------------------------------------------------

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

//=============================================================
// [CRenderer] フルスクリーン
//=============================================================
void CRenderer::SetFullScreen(const bool& bFullScreen)
{
	// 変数を上書きする
	m_bFullScreen = bFullScreen;

	// 現在のウィンドウスタイルを取得
	DWORD dwStyle = GetWindowLong(m_hwnd, GWL_STYLE);
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }; // ウィンドウの座標を格納

	// フルスクリーンに切り替える
	if (m_bFullScreen)
	{
		// ウィンドウモードに切り替え
		SetWindowLong(m_hwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(m_hwnd, HWND_TOP, rect.left, rect.top,
			rect.right - rect.left, rect.bottom - rect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(m_hwnd, SW_NORMAL);
	}
	else
	{
		// フルスクリーンモードに切り替え
		GetWindowRect(m_hwnd, &rect);
		SetWindowLong(m_hwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(m_hwnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(m_hwnd, SW_MAXIMIZE);
	}
}