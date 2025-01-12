//=============================================================
//
// �J�����̓��� [camera_move.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _CAMERA_MOVE_H_
#define _CAMERA_MOVE_H_

#include "component.h"
#include "scripts/next_portal.h"

// �J�����̓����N���X
class CCameraMove : public Component
{
public:
	// �t�H�[�J�X�̎��
	typedef enum
	{
		PLAYER = 0,	// �v���C���[
		PORTAL			// �|�[�^��
	}FOCUS;

	// �R���s���[�^�t�H�[�J�X�̏��
	typedef enum
	{
		INIT_POS = 0,			// �����ʒu�Ɉړ�
		GO_MOVE,					// ������
		NEXT_STAGE,			//	���̃X�e�[�W��
		END,							// �I��
		GOAL,						// �S�[��
	} PORTAL_PROGRESS;

	void Init() override;																// ������
	void Update() override;															// �X�V
	void SetFocus(FOCUS focus);													// �t�H�[�J�X�Ώۂ̐ݒ�
	void SetNextPortal(CNextPortal* pNextPortal);							// �|�[�^���̐ݒ�
	void SetShake(const int& nPower, const int& nTime);				// �U��
	void SetCameraPlayerOffset(const D3DXVECTOR3& offset) {		// �v���C���[����̃I�t�Z�b�g
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
	void PlayerFocus();										// �v���C���[�t�H�[�J�X
	void PortalFocus();										// �|�[�^���t�H�[�J�X

	FOCUS m_focus;										// �t�H�[�J�X�Ώ�
	PORTAL_PROGRESS m_portalProgress;		// �|�[�^���̐i�s�x
	CNextPortal* m_portal;								// �|�[�^��
	D3DXVECTOR3 m_playerCameraOffset;		// �v���C���[�t�H�[�J�X���̃J�����̃I�t�Z�b�g
	int m_nChangedCounter;							// �`�F���W�J�E���^�[
	int m_nShakePower;									// �U���̋���
	int m_nShakeTime;									// �U���̎���
};

// �J�����̈ʒu��ς��邽�߂̃R���|�[�l���g
class CChangePlayerFocus : public Component
{
public:
	void OnTriggerStay(GameObject* other) override;
	void SetOffset(const D3DXVECTOR3& offset) { m_changedOffset = offset; }
private:
	D3DXVECTOR3 m_changedOffset;
};

#endif // !_CAMERA_MOVE_H_
