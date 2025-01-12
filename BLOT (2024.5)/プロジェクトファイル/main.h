//===================================================================================
//
// Blot [main.h]
// Author: Ito Kotaro
//
//===================================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

// �C���N���[�h
#include "game.h"

// �v���Z�X�N���X�̒�`
class CProcess
{
public:
	CProcess();		// �R���X�g���N�^
	~CProcess();		// �f�X�g���N�^

	// ���[�h
	typedef enum
	{
		MODE_START = 0,		// �X�^�[�g
		MODE_END,				// �I��
		MODE_MAX
	} MODE;

	void SelectMode();						// ���[�h�Z���N�g
	void LookCursor(bool isLook);		// �J�[�\���̕\���E��\��
	void PrintTitle();							// �^�C�g���̕`��
	void ScreenCheck();					// �X�N���[���`�F�b�N

	// ��`
	static const int SELECT_WIDTH = 75;				// �I�����̉�
	static const int SELECT_HEIGHT = 40;				// �I�����̍���
	static const int TITLE_WIDTH = 74;					// �^�C�g���̉�
	static const int TITLE_HEIGHT = 10;				// �^�C�g���̍���
	static const int SCREEN_UPDATE = 500;			// �X�N���[���`�F�b�N�̕`��X�V�Ԋu
	static const int TITLE_TO_GAME = 500;
private:
	void StartGame();						// �Q�[�����n�߂�
	void Uninit();								// �I��
	void PrintScreenDescription();		// �X�N���[���`�F�b�N���̐���

	CGameManager* m_pGameManager;		// �Q�[���}�l�[�W���[
};

#endif // !_MAIN_H_
