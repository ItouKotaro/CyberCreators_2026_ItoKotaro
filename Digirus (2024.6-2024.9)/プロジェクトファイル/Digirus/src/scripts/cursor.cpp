//=============================================================
//
// �J�[�\�� [cursor.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "cursor.h"
#include "manager.h"
#include "component/2d/polygon.h"

// �ÓI�����o�ϐ��̏�����
const int CCursor::CURSOR_SIZE = 50;

//=============================================================
// [CCursor] ������
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
// [CCursor] �I��
//=============================================================
void CCursor::Uninit()
{
	m_pCursorObj->Destroy();
}

//=============================================================
// [CCursor] �X�V
//=============================================================
void CCursor::Update()
{
	if (GetActiveWindow() != CManager::GetHWND())
	{
		return;
	}

	// ��ʒ��S�̈ʒu���擾����
	POINT centerPoint;
	centerPoint.x = CRenderer::SCREEN_WIDTH / 2;
	centerPoint.y = CRenderer::SCREEN_HEIGHT / 2;
	ClientToScreen(CManager::GetHWND(), &centerPoint);

	// �g�p���̃J�[�\���f�o�C�X�𔻒肷��
	POINT currentPos;
	GetCursorPos(&currentPos);
	CursorDevice oldCursorDevice = m_cursorDevice;

	// �}�E�X����
	if (currentPos.x != centerPoint.x ||
		currentPos.y != centerPoint.y)
	{ // �O��ƃJ�[�\���ʒu���قȂ�Ƃ�
		m_cursorDevice = CursorDevice::MOUSE;
		CManager::CursorPos cursorPos = GetManager()->GetCursorClientPos();
		m_cursorPoint.x = cursorPos.x;
		m_cursorPoint.y = cursorPos.y;
	}

	// �R���g���[���[����
	short controlStickLx = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>()->GetState().Gamepad.sThumbLX;
	short controlStickLy = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>()->GetState().Gamepad.sThumbLY;
	if (controlStickLx != 0 || controlStickLy != 0)
	{ // �R���g���[���[�œ������Ă���Ƃ�
		m_cursorDevice = CursorDevice::CONTROLLER;
		m_cursorPoint.x += static_cast<LONG>(controlStickLx * 0.0004f);
		m_cursorPoint.y += static_cast<LONG>(controlStickLy * -0.0004f);

		// �}�E�X�ʒu��ۑ�����
		SetCursorPos(centerPoint.x, centerPoint.y);
	}

	// �J�[�\���ʒu�ɐ�������
	if (m_cursorPoint.x < 0.0f)
		m_cursorPoint.x = static_cast<LONG>(0.0f);
	else if (CRenderer::SCREEN_WIDTH < m_cursorPoint.x)
		m_cursorPoint.x = CRenderer::SCREEN_WIDTH;
	if (m_cursorPoint.y < 0.0f)
		m_cursorPoint.y = static_cast<LONG>(0.0f);
	else if (CRenderer::SCREEN_HEIGHT < m_cursorPoint.y)
		m_cursorPoint.y = CRenderer::SCREEN_HEIGHT;

	// �J�[�\���̕\���ʒu��ύX����
	m_pCursorObj->transform->SetPos(static_cast<float>(m_cursorPoint.x), static_cast<float>(m_cursorPoint.y));
}

//=============================================================
// [CCursor] �\�����
//=============================================================
void CCursor::SetVisible(const bool& bVisible)
{
	m_pCursorObj->SetVisible(bVisible);
}

//=============================================================
// [CCursor] �D��x
//=============================================================
void CCursor::SetPriority(const int& nPriority)
{
	m_pCursorObj->SetPriority(nPriority);
}