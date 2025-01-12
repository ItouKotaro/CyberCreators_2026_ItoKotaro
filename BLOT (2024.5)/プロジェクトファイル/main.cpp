//===================================================================================
//
// Blot [main.cpp]
// Author: Ito Kotaro
//
//===================================================================================
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// ��`
const int CProcess::SELECT_WIDTH;
const int CProcess::SELECT_HEIGHT;
const int CProcess::TITLE_WIDTH;
const int CProcess::TITLE_HEIGHT;
const int CProcess::SCREEN_UPDATE;
const int CProcess::TITLE_TO_GAME;

//=======================================================
// ���C��
//=======================================================
int main(void)
{
	// �t���X�N���[���ɋ����ύX
	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);

	// ��ʂ��N���A
	system("cls");

	// �v���Z�X�N���X���쐬����
	CProcess process;

	// ��ʂ̃X�N���[���`�F�b�N
	process.ScreenCheck();

	// �J�[�\���̔�\��
	process.LookCursor(false);

	// ���[�h�Z���N�g
	process.SelectMode();

	return 0;
}


//=======================================================
// [CProcess] ���[�h�Z���N�g
//=======================================================
void CProcess::SelectMode()
{
	// �ϐ��錾
	MODE selectMode = MODE_START;

	while (1)
	{
		// �^�C�g�����v�����g
		PrintTitle();

		switch (selectMode)
		{
		case MODE_START:
			printf("\033[%d;%dH\033[36m����������������������������������\033[39m     ����������������������������������"	, SELECT_HEIGHT, SELECT_WIDTH);
			printf("\033[%d;%dH\033[36m��     �n�߂�    ��\033[39m     ��      �I��     ��"															, SELECT_HEIGHT + 1, SELECT_WIDTH);
			printf("\033[%d;%dH\033[36m����������������������������������\033[39m     ����������������������������������"	, SELECT_HEIGHT + 2, SELECT_WIDTH);

			if ((GetKeyState(VK_RIGHT) & 0x8000) > 0)
			{
				selectMode = MODE_END;
			}
			break;

		case MODE_END:
			printf("\033[%d;%dH����������������������������������     \033[31m����������������������������������\033[39m"	, SELECT_HEIGHT, SELECT_WIDTH);
			printf("\033[%d;%dH��     �n�߂�    ��     \033[31m��      �I��     ��\033[39m"															, SELECT_HEIGHT + 1, SELECT_WIDTH);
			printf("\033[%d;%dH����������������������������������     \033[31m����������������������������������\033[39m"	, SELECT_HEIGHT + 2, SELECT_WIDTH);

			if ((GetKeyState(VK_LEFT) & 0x8000) > 0)
			{
				selectMode = MODE_START;
			}
			break;
		}

		// ���肷��
		if ((GetKeyState(VK_RETURN) & 0x8000) > 0 ||
			(GetKeyState(VK_SPACE) & 0x8000) > 0)
		{
			switch (selectMode)
			{
			case MODE_START:
				StartGame();		// �Q�[���J�n
				break;

			case MODE_END:
				Uninit();			// �I������
				return;
			}
		}

	}
}

//=======================================================
// [CProcess] �R���X�g���N�^
//=======================================================
CProcess::CProcess()
{

}

//=======================================================
// [CProcess] �f�X�g���N�^
//=======================================================
CProcess::~CProcess()
{
	// �Q�[���}�l�[�W���[��j������
	if (m_pGameManager != NULL)
	{
		delete m_pGameManager;
		m_pGameManager = NULL;
	}
}

//=======================================================
// [CProcess] �Q�[���X�^�[�g
//=======================================================
void CProcess::StartGame()
{
	// �Q�[���}�l�[�W���[�𐶐�����
	m_pGameManager = new CGameManager();

	// �Q�[���}�l�[�W���[�ɊJ�n�̖��߂�����
	m_pGameManager->StartGame();

	// �Q�[���}�l�[�W���[��j������
	if (m_pGameManager != NULL)
	{
		delete m_pGameManager;
		m_pGameManager = NULL;
	}
}

//=======================================================
// [CProcess] �I��
//=======================================================
void CProcess::Uninit()
{

}

//=======================================================
// [CProcess] �\���t���O
//=======================================================
void CProcess::LookCursor(bool isLook)
{
	HANDLE hOut;
	CONSOLE_CURSOR_INFO cci;

	//�o�͗p�n���h���̎擾
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	//CONSOLE_CURSOR_INFO�\���̂̌��݂̏�Ԃ��擾����
	GetConsoleCursorInfo(hOut, &cci);

	//�����o�ϐ��ł���bVisible���J�[�\���̕\���E��\����ݒ肷��
	cci.bVisible = isLook;

	//�ύX�����\���̏����R���\�[��Window�ɃZ�b�g����
	SetConsoleCursorInfo(hOut, &cci);
}

//=======================================================
// [CProcess] �^�C�g���o��
//=======================================================
void CProcess::PrintTitle()
{
	printf("\033[%d;%dH _______    __          ______    ________ ", TITLE_HEIGHT + 0, TITLE_WIDTH);
	printf("\033[%d;%dH/       | /   |        /      \\  /        |", TITLE_HEIGHT + 1, TITLE_WIDTH);
	printf("\033[%d;%dH$$$$$$$ | |$$ |       /$$$$$$  | $$$$$$$$/ ", TITLE_HEIGHT + 2, TITLE_WIDTH);
	printf("\033[%d;%dH$$ |__$$| |$$ |       $$ |  $$ |    $$ |   ", TITLE_HEIGHT + 3, TITLE_WIDTH);
	printf("\033[%d;%dH$$    $<  |$$ |       $$ |  $$ |    $$ |   ", TITLE_HEIGHT + 4, TITLE_WIDTH);
	printf("\033[%d;%dH$$$$$$$ | |$$ |       $$ |  $$ |    $$ |   ", TITLE_HEIGHT + 5, TITLE_WIDTH);
	printf("\033[%d;%dH$$ |__$$| |$$ |_____  $$ |__$$ |    $$ |   ", TITLE_HEIGHT + 6, TITLE_WIDTH);
	printf("\033[%d;%dH$$    $$/ |$$       | $$    $$/     $$ |   ", TITLE_HEIGHT + 7, TITLE_WIDTH);
	printf("\033[%d;%dH$$$$$$$/  |$$$$$$$$/   $$$$$$/      $$/    ", TITLE_HEIGHT + 8, TITLE_WIDTH);
}

//=======================================================
// [CProcess] �X�N���[���`�F�b�N���̐����o��
//=======================================================
void CProcess::PrintScreenDescription()
{
	printf("\033[%d;%dH", CGameManager::DESCRIPTION_HEIGHT, CGameManager::DESCRIPTION_WIDTH + 2);
	printf("�\��������Ȃ��T�C�Y�ɕύX���Ă�������");
	printf("\033[%d;%dH", CGameManager::DESCRIPTION_HEIGHT + 1, CGameManager::DESCRIPTION_WIDTH + 14);
	printf("(F11�ōő剻)");
	printf("\033[%d;%dH", CGameManager::DESCRIPTION_HEIGHT + 4, CGameManager::DESCRIPTION_WIDTH + 10);
	printf("SPACE�L�[�ő��s���܂�");
}

//=======================================================
// [CProcess] �X�N���[���`�F�b�N
//=======================================================
void CProcess::ScreenCheck()
{
	// �Q�[���}�l�[�W���[�𐶐�����
	m_pGameManager = new CGameManager();

	// ����������
	m_pGameManager->HideDescription();

	// �u���b�N��~���l�߂�
	CField* pField = m_pGameManager->GetField();
	for (int i = 0; i < CField::FIELD_LENGTH; i++)
	{
		pField->Set(i, CField::BLOCK_PLAYER);
	}

	// �X�V���Ԃ�ݒ肷��
	DWORD updateTime = timeGetTime();

	while (1)
	{
		// �X�V���Ԃ�������`����X�V
		if (timeGetTime() >= updateTime)
		{
			// �`�悷��
			m_pGameManager->Draw();

			// ������`�悷��
			PrintScreenDescription();

			// ���̍X�V���Ԃ�ݒ�
			updateTime = timeGetTime() + SCREEN_UPDATE;
		}
		
		// �v���C���[�̓���
		if ((GetKeyState(VK_SPACE) & 0x8000) > 0 ||
			(GetKeyState(VK_RETURN) & 0x8000) > 0)
		{
			break;
		}
	}

	// �Q�[���}�l�[�W���[��j������
	if (m_pGameManager != NULL)
	{
		delete m_pGameManager;
		m_pGameManager = NULL;
	}

	// ��ʂ��N���A
	system("cls");

	// �ҋ@
	Sleep(TITLE_TO_GAME);
}