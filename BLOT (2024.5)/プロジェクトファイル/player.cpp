//===================================================================================
//
// �v���C���[ [player.cpp]
// Author: Ito Kotaro
//
//===================================================================================
#include "player.h"
#include "field.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#pragma comment(lib, "winmm.lib")	// �V�X�e�������擾�ɕK�v

// ��`
const int CPlayer::PLAYER_WIDTH;
const int CPlayer::PLAYER_HEIGHT;
const int CPlayer::MAX_STACK;
const int CPlayer::STACK_INTERVAL;
const int CPlayer::STEP_INTERVAL;

//=======================================================
// [CPlayer] �R���X�g���N�^
//=======================================================
CPlayer::CPlayer() : m_nStacks(0), m_nPos(0)
{

}

//=======================================================
// [CPlayer] �f�X�g���N�^
//=======================================================
CPlayer::~CPlayer()
{

}

//=======================================================
// [CPlayer] �ςݏd�ˏ���
//=======================================================
void CPlayer::InputStacks()
{
	// �ϐ��錾
	DWORD nextStack = timeGetTime() + STACK_INTERVAL;

	//�@�L�[���͂��󂯕t����
	bool bFirst = true;
	while (1)
	{
		if ((GetKeyState(VK_SPACE) & 0x8000) > 0)
		{	// �X�y�[�X�������Ă���Ƃ�
			if (timeGetTime() >= nextStack)
			{
				m_nStacks++;	// �u���b�N��ςݏグ��
				bFirst = false;

				// ���̃X�^�b�N���Ԃ�ݒ�
				nextStack = timeGetTime() + STACK_INTERVAL;
			}

			// �X�^�b�N�̍ő吔���}�����Ƃ�
			if (m_nStacks >= MAX_STACK)
				break;	// �I��
		}
		else if (!bFirst)
		{	// �������Ƃ�
			Sleep(STACK_INTERVAL);	// �i�ݎn�߂�܂ł̒x��
			break;
		}

		// �`����X�V����
		Draw(PLAYER_HEIGHT);
	}
}

//=======================================================
// [CPlayer] �`��
//=======================================================
void CPlayer::Draw(int nHeight)
{
	// �X�^�b�N�u���b�N�̕`��
	for (int nCntBlock = 1; nCntBlock < m_nStacks + 1; nCntBlock++)
	{
		for (int i = 0; i < CField::TILE_HEIGHT; i++)
		{
			printf("\033[%d;%dH", nHeight - nCntBlock * CField::TILE_HEIGHT + i + (nCntBlock - 1), PLAYER_WIDTH + m_nPos * CField::TILE_WIDTH);
			if (i == 0 && m_nStacks == nCntBlock)
				printf(STACK_CHAR1);			// 1�s�ځi�X�^�b�N�̏�̂ݕ`��j
			else if (i == 1)
				printf(STACK_CHAR2);			// 2�s��
			else if (i == 2 && nCntBlock == 1)
				printf(STACK_CHAR3);			// 3�s��
			else if (i == 2)
				printf(STACK_LINK);				// 3�s�ځi�Ȃ��j
		}
	}

	// �v���C���[��`��
	for (int i = 0; i < CField::TILE_HEIGHT; i++)
	{
		int nNoStacks = bool(m_nStacks == 0);
		printf("\033[%d;%dH", 
			nHeight - CField::TILE_HEIGHT * (m_nStacks + 1) + (m_nStacks - 1) + i + nNoStacks,
			PLAYER_WIDTH + CField::TILE_WIDTH * m_nPos);		// �����ʒu�Ɉړ�

		if (i == 0)
			printf(PLAYER_CHAR1);			// 1�s��
		else if (i == 1)
			printf(PLAYER_CHAR2);			// 2�s��
		else if (i == 2)
			printf(PLAYER_CHAR3);			// 3�s��
	}
}