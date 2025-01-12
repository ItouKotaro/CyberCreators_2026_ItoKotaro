//=============================================================
//
// �|�[�Y [pause.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "gameobject.h"
#include "component.h"
#include "scripts/cursor.h"

// �|�[�Y�Ǘ��N���X
class CPause : public Component
{
public:
	void Init();
	void Uninit();
	void Update();

	void SetPause(const bool& bPause);
	bool GetIsPause() { return m_bIsPause; }

	static const int BUTTON_WIDTH;		// �{�^���̉���
	static const int BUTTON_HEIGHT;	// �{�^���̍���
	static const int BUTTON_SPACE;		// �]��
private:
	void SetShow(const bool& bShow);
	bool m_bIsPause;			// �|�[�Y���Ă��邩
	CCursor* m_pCursor;		// �J�[�\��

	GameObject* m_pBG;					// �w�i
	GameObject* m_pPauseText;			// �e�L�X�g
	GameObject* m_pPauseTextBG;		// �|�[�Y�e�L�X�g�̔w�i
	GameObject* m_pContinue;			// ������{�^��
	GameObject* m_pRetry;					// ���g���C�{�^��
	GameObject* m_pBackHome;			// �z�[���֖߂�{�^��
};

#endif // !_PAUSE_H_
