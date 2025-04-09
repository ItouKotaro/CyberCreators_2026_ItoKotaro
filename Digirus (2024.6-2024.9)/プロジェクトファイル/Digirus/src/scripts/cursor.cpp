//=============================================================
//
// カーソル [cursor.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "cursor.h"
#include "manager.h"
#include "component/2d/polygon.h"

// 静的メンバ変数の初期化
const int CCursor::CURSOR_SIZE = 50;

//=============================================================
// [CCursor] 初期化
//=============================================================
void CCursor::Init()
{
	m_cursorDevice = CursorDevice::MOUSE;
	m_cursorPoint.x = CRenderer::SCREEN_WIDTH / 2;
	m_cursorPoint.y = CRenderer::SCREEN_HEIGHT / 2;
	m_pCursorObj = new GameObject;
	m_pCursorObj->SetPriority(16);
	m_pCursorObj->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\HOME\\cursor.png");
	m_pCursorObj->transform->SetSize(CURSOR_SIZE, CURSOR_SIZE);
}

//=============================================================
// [CCursor] 終了
//=============================================================
void CCursor::Uninit()
{
	m_pCursorObj->Destroy();
}

//=============================================================
// [CCursor] 更新
//=============================================================
void CCursor::Update()
{
	if (GetActiveWindow() != CManager::GetHWND())
	{
		return;
	}

	// 画面中心の位置を取得する
	POINT centerPoint;
	centerPoint.x = CRenderer::SCREEN_WIDTH / 2;
	centerPoint.y = CRenderer::SCREEN_HEIGHT / 2;
	ClientToScreen(CManager::GetHWND(), &centerPoint);

	// 使用中のカーソルデバイスを判定する
	POINT currentPos;
	GetCursorPos(&currentPos);
	CursorDevice oldCursorDevice = m_cursorDevice;

	// マウス判定
	if (currentPos.x != centerPoint.x ||
		currentPos.y != centerPoint.y)
	{ // 前回とカーソル位置が異なるとき
		m_cursorDevice = CursorDevice::MOUSE;
		CManager::CursorPos cursorPos = GetManager()->GetCursorClientPos();
		m_cursorPoint.x = cursorPos.x;
		m_cursorPoint.y = cursorPos.y;
	}

	// コントローラー判定
	short controlStickLx = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>()->GetState().Gamepad.sThumbLX;
	short controlStickLy = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>()->GetState().Gamepad.sThumbLY;
	if (controlStickLx != 0 || controlStickLy != 0)
	{ // コントローラーで動かしているとき
		m_cursorDevice = CursorDevice::CONTROLLER;
		m_cursorPoint.x += static_cast<LONG>(controlStickLx * 0.0004f);
		m_cursorPoint.y += static_cast<LONG>(controlStickLy * -0.0004f);

		// マウス位置を保存する
		SetCursorPos(centerPoint.x, centerPoint.y);
	}

	// カーソル位置に制限つける
	if (m_cursorPoint.x < 0.0f)
		m_cursorPoint.x = static_cast<LONG>(0.0f);
	else if (CRenderer::SCREEN_WIDTH < m_cursorPoint.x)
		m_cursorPoint.x = CRenderer::SCREEN_WIDTH;
	if (m_cursorPoint.y < 0.0f)
		m_cursorPoint.y = static_cast<LONG>(0.0f);
	else if (CRenderer::SCREEN_HEIGHT < m_cursorPoint.y)
		m_cursorPoint.y = CRenderer::SCREEN_HEIGHT;

	// カーソルの表示位置を変更する
	m_pCursorObj->transform->SetPos(static_cast<float>(m_cursorPoint.x), static_cast<float>(m_cursorPoint.y));
}

//=============================================================
// [CCursor] 表示状態
//=============================================================
void CCursor::SetVisible(const bool& bVisible)
{
	m_pCursorObj->SetVisible(bVisible);
}

//=============================================================
// [CCursor] 優先度
//=============================================================
void CCursor::SetPriority(const int& nPriority)
{
	m_pCursorObj->SetPriority(nPriority);
}