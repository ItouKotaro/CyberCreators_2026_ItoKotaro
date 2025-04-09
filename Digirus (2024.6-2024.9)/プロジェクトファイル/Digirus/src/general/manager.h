//=============================================================
//
// �}�l�[�W���[ [manager.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "renderer.h"
#include "scene.h"
#include "internal/physics.h"
#include "internal/data_manager.h"
#include "internal/audio_manager.h"
#include "gameobject.h"

// �}�l�[�W���[�N���X�̒�`
class CManager
{
public:
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);	// ������
	void Uninit();																						// �I��
	void Update();																					// �X�V
	void Draw();																						// �`��

	void SetFPS(const int& nFPS) { m_nFPS = nFPS; }											// FPS�̐ݒ�i���[�U�[�̎g�p�֎~�j
	static const int GetFPS() { return m_nFPS; }													// FPS�̎擾
	void SetDeltaTime(const float& fDeltaTime) { m_fDeltaTime = fDeltaTime; }		// �f���^�^�C���̐ݒ�i���[�U�[�̎g�p�֎~�j
	static const float GetDeltaTime() { return m_fDeltaTime; }								// �f���^�^�C���̎擾
	static void SetPause(const bool& bPause) { m_bPause = bPause; }					// �|�[�Y�̐ݒ�
	static const bool GetPause() { return m_bPause; }											// �|�[�Y�̎擾
	void SetMouseWheel(const int& nMouseWheel) { m_nMouseWheel = nMouseWheel; }	// �}�E�X�z�C�[���̐ݒ�i���[�U�[�̎g�p�֎~�j
	static const int GetMouseWheel() { return m_nMouseWheel; }							// �}�E�X�z�C�[���̎擾
	void SetShowCursor(const bool& show);

	struct CursorPos
	{
		float x;
		float y;
	};

	CursorPos GetCursorClientPos();																		// �J�[�\���ʒu�̎擾
	void SetCursorClientPos(float x, float y);															// �J�[�\���ʒu�̐ݒ�
	D3DXVECTOR2 GetWindowSize();																	// �E�B���h�E�T�C�Y�̎擾

	static HWND GetHWND() { return m_hwnd; }													// �n���h���̎擾
	static CRenderer* GetRenderer() { return m_pRenderer; }								// �����_���[�̎擾
	static CPhysics* GetPhysics() { return m_pPhysics; }										// �����̎擾
	static CDataManager* GetDataManager() { return m_pDataManager; }				// �f�[�^�}�l�[�W���[�̎擾
private:
	static int m_nFPS;												// FPS�l
	static float m_fDeltaTime;									// �f���^�^�C��
	static int m_nMouseWheel;									// �}�E�X�z�C�[���l
	static bool m_bPause;											// �|�[�Y
	static bool m_bShowCursor;								// �J�[�\���̕\��
	static HWND m_hwnd;										// �n���h��
	static CRenderer* m_pRenderer;							// �����_���[
	static CPhysics* m_pPhysics;								// ����
	static CDataManager* m_pDataManager;				// �f�[�^�}�l�[�W���[
	static CAudioManager* m_pAudioManager;			// �I�[�f�B�I�}�l�[�W���[
};

#endif // !_MANAGER_H_
