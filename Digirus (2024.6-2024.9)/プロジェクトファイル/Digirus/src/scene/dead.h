//=============================================================
//
// ���S��� [dead.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _DEAD_H_
#define _DEAD_H_

#include "scene.h"
#include "gameobject.h"
#include "scripts/cursor.h"

// ���S�V�[��
class CDeadScene : public CScene
{
public:
	void Init() override;
	void Update() override;
	void Uninit() override;
private:
	GameObject* m_pUpBG;		// ��
	GameObject* m_pDownBG;	// ��
	GameObject* m_pLeftBG;		// ��
	GameObject* m_pRightBG;		// �E
	GameObject* m_pRetry;			// ���g���C
	GameObject* m_pRetryIcon;	// ���g���C�A�C�R��
	GameObject* m_pHome;		// �z�[��
	GameObject* m_pHomeIcon;	// �z�[���A�C�R��

	float m_fRollingRetryIcon;		// ���g���C�A�C�R���̉�]

	int m_nTextCounter;				// �e�L�X�g�J�E���^�[
	float m_fAlpha;						// �s�����x
	CCursor* m_pCursor;				// �J�[�\��
};

#endif // !_DEAD_H_
