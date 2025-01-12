//===================================================================================
//
// �Q�[���}�l�[�W���[ [game.h]
// Author: Ito Kotaro
//
//===================================================================================
#ifndef _GAME_H_
#define _GAME_H_

// �C���N���[�h
#include "player.h"
#include "field.h"
#include "score.h"

// �Q�[���}�l�[�W���[�N���X���`
class CGameManager
{
public:
	CGameManager();												// �R���X�g���N�^
	~CGameManager();											// �f�X�g���N�^
	void StartGame();												// �Q�[�����n�߂�
	void Draw();														// �`��
	void HideDescription() { m_bStarted = true; }		// ����������
	CField* GetField() { return m_pField; }				// �t�B�[���h�擾

	// ��`
	static const int STAGEMOVE_INTERVAL = 50;			// �X�e�[�W�ړ��̊Ԋu
	static const int STAGEFALL_INTERVAL = 5;				// �X�e�[�W���痎������Ԋu
	static const int STAGEFALL_COUNT = 80;					// �ǂꂮ�炢�������邩
	static const int PLAYER_FALLSPEED = 15;				// �v���C���[�̗������x
	static const int RESULT_SCORE_HEIGHT = 27;			// �X�R�A���ʂ̍���
	static const int RESULT_RANKING_WIDTH = 83;		// �����L���O�̉�
	static const int RESULT_RANKING_HEIGHT = 36;		// �����L���O�̍���
	static const int DESCRIPTION_WIDTH = 74;				// �����̉�
	static const int DESCRIPTION_HEIGHT = 20;			// �����̍���
private:
	void DescriptionDraw();	// �����̕`��
	void Step();					// �i�ޏ���
	void GameOver();			// �Q�[���I�[�o�[
	void ResetPos();				// �ʒu�����Z�b�g

	CPlayer* m_pPlayer;		// �v���C���[
	CField* m_pField;			// �t�B�[���h
	CScore* m_pScore;		// �X�R�A

	bool m_bStarted;			// �J�n�ς݂�
	bool m_IsGameOver;		// �Q�[���I�[�o�[��Ԃ�
};

#endif // !_GAME_H_
