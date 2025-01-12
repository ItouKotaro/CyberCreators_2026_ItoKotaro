//===================================================================================
//
// �X�R�A [score.cpp]
// Author: Ito Kotaro
//
//===================================================================================
#include "score.h"
#include <stdio.h>
#include <math.h>
#include <sys/stat.h>  

// ��`
const int CScore::NUM_WIDTH;
const int CScore::NUM_HEIGHT;
const int CScore::NUM_SPACE;
const int CScore::SCORE_HEIGHT;
const int CScore::CENTER;
const int CScore::MAX_RANKING;

//=======================================================
// [CScore] �R���X�g���N�^
//=======================================================
CScore::CScore() : m_nScore(0)
{

}

//=======================================================
// [CScore] �X�R�A��o�^
//=======================================================
int CScore::RegisterScore()
{
	// data�t�H���_�����݂��邩
	struct stat st;
	if (stat("data", &st) != 0)
		return -1;	// ���݂��Ȃ��Ƃ��̓����L���O�ɓo�^���Ȃ�

	// �X�R�A��0�̂Ƃ��͓o�^���Ȃ�
	if (m_nScore == 0)
		return -1;

	//-----------------------------------------------------------------
	// �L�^�t�@�C����ǂݍ���

	int aScore[MAX_RANKING] = {};

	// �ϐ�������������
	for (int i = 0; i < MAX_RANKING; i++)
		aScore[i] = 0;

	// �t�@�C�����J��
	FILE* pFile = NULL;
	pFile = fopen(RANKING_PATH, "rb");

	if (pFile != NULL)
	{ // �t�@�C�������������Ƃ�

		// �t�@�C������f�[�^�����o��
		fread(&aScore, sizeof(int), MAX_RANKING, pFile);

		// �t�@�C�������
		fclose(pFile);
	}

	//-----------------------------------------------------------------
	// �L�^���X�V����ꍇ�͋L�^�t�@�C���ɏ�������
	int nRank = 0;
	if (aScore[MAX_RANKING - 1] <= m_nScore)
	{ // �ō��L�^�̂Ƃ�

		// �ō��L�^�ɑ������
		aScore[MAX_RANKING - 1] = m_nScore;

		// �����L���O�����ւ���
		for (int nCnt = 0; nCnt < MAX_RANKING - 1; nCnt++)
		{
			if (aScore[MAX_RANKING - (nCnt + 1)] > aScore[MAX_RANKING - (nCnt + 2)])
			{
				int nSaveNum = aScore[MAX_RANKING - (nCnt + 1)];
				aScore[MAX_RANKING - (nCnt + 1)] = aScore[MAX_RANKING - (nCnt + 2)];
				aScore[MAX_RANKING - (nCnt + 2)] = nSaveNum;
				nRank++;
			}
		}

		// ���ʂ����v�Z����
		nRank = MAX_RANKING - nRank;

		// �t�@�C���������o����ԂŊJ��
		pFile = fopen(RANKING_PATH, "wb");

		// �L�^����������
		fwrite(&aScore, sizeof(int), MAX_RANKING, pFile);

		// �t�@�C�������
		fclose(pFile);

		return nRank;
	}

	return -1;
}

//=======================================================
// [CScore] �`��
//=======================================================
void CScore::Draw(int nHeight)
{
	// �ϐ��錾
	int nDigit = 1;			// ����
	int nStartWidth = 0;	// �o�͂̎n�܂�̈ʒu

	// �X�R�A���������邩���ׂ�
	if (m_nScore != 0)
		nDigit = log10(m_nScore) + 1;

	// �����ɕ\�����邽�߂ɏ����n�߂̈ʒu���v�Z����
	nStartWidth = CENTER - (NUM_WIDTH * nDigit + NUM_SPACE * (nDigit - 1)) / 2;

	// �`�悷��
	for (int nCntDigit = 1; nCntDigit <= nDigit; nCntDigit++)
	{
		// �ʂ̐������擾����
		int nNum = 0;
		int nPow = (int)pow(10, (double)nDigit - (double)nCntDigit);
		nNum = m_nScore % (nPow *10) / nPow;

		// �`��ʒu���v�Z����
		int nWriteWidth = nStartWidth + NUM_WIDTH * (nCntDigit - 1) + NUM_SPACE *(nCntDigit - 1);

		// �����̕`��
		switch (nNum)
		{
		case 0:
			printf("\033[%d;%dH������������", nHeight + 0, nWriteWidth);
			printf("\033[%d;%dH�� ���� ��", nHeight + 1, nWriteWidth);
			printf("\033[%d;%dH�� ���� ��", nHeight + 2, nWriteWidth);
			printf("\033[%d;%dH�� ���� ��", nHeight + 3, nWriteWidth);
			printf("\033[%d;%dH�� ���� ��", nHeight + 4, nWriteWidth);
			printf("\033[%d;%dH������������", nHeight + 5, nWriteWidth);
			break;

		case 1:
			printf("\033[%d;%dH  ������  ", nHeight + 0, nWriteWidth);
			printf("\033[%d;%dH ���� �� ", nHeight + 1, nWriteWidth);
			printf("\033[%d;%dH ���� �� ", nHeight + 2, nWriteWidth);
			printf("\033[%d;%dH  �� ��  ", nHeight + 3, nWriteWidth);
			printf("\033[%d;%dH ���� ����", nHeight + 4, nWriteWidth);
			printf("\033[%d;%dH ����������", nHeight + 5, nWriteWidth);
			break;

		case 2:
			printf("\033[%d;%dH������������", nHeight + 0, nWriteWidth);
			printf("\033[%d;%dH�������� ��", nHeight + 1, nWriteWidth);
			printf("\033[%d;%dH�������� ��", nHeight + 2, nWriteWidth);
			printf("\033[%d;%dH�� ��������", nHeight + 3, nWriteWidth);
			printf("\033[%d;%dH�� ��������", nHeight + 4, nWriteWidth);
			printf("\033[%d;%dH������������", nHeight + 5, nWriteWidth);
			break;

		case 3:
			printf("\033[%d;%dH������������", nHeight + 0, nWriteWidth);
			printf("\033[%d;%dH�������� ��", nHeight + 1, nWriteWidth);
			printf("\033[%d;%dH�������� ��", nHeight + 2, nWriteWidth);
			printf("\033[%d;%dH�������� ��", nHeight + 3, nWriteWidth);
			printf("\033[%d;%dH�������� ��", nHeight + 4, nWriteWidth);
			printf("\033[%d;%dH������������", nHeight + 5, nWriteWidth);
			break;

		case 4:
			printf("\033[%d;%dH������������", nHeight + 0, nWriteWidth);
			printf("\033[%d;%dH�� ���� ��", nHeight + 1, nWriteWidth);
			printf("\033[%d;%dH�� ���� ��", nHeight + 2, nWriteWidth);
			printf("\033[%d;%dH�������� ��", nHeight + 3, nWriteWidth);
			printf("\033[%d;%dH   �� ��", nHeight + 4, nWriteWidth);
			printf("\033[%d;%dH   ������", nHeight + 5, nWriteWidth);
			break;

		case 5:
			printf("\033[%d;%dH������������", nHeight + 0, nWriteWidth);
			printf("\033[%d;%dH�� ��������", nHeight + 1, nWriteWidth);
			printf("\033[%d;%dH�� ��������", nHeight + 2, nWriteWidth);
			printf("\033[%d;%dH�������� ��", nHeight + 3, nWriteWidth);
			printf("\033[%d;%dH�������� ��", nHeight + 4, nWriteWidth);
			printf("\033[%d;%dH������������", nHeight + 5, nWriteWidth);
			break;

		case 6:
			printf("\033[%d;%dH������������", nHeight + 0, nWriteWidth);
			printf("\033[%d;%dH�� ��������", nHeight + 1, nWriteWidth);
			printf("\033[%d;%dH�� ��������", nHeight + 2, nWriteWidth);
			printf("\033[%d;%dH�� ���� ��", nHeight + 3, nWriteWidth);
			printf("\033[%d;%dH�� ���� ��", nHeight + 4, nWriteWidth);
			printf("\033[%d;%dH������������", nHeight + 5, nWriteWidth);
			break;

		case 7:
			printf("\033[%d;%dH������������", nHeight + 0, nWriteWidth);
			printf("\033[%d;%dH�������� ��", nHeight + 1, nWriteWidth);
			printf("\033[%d;%dH   �� ��", nHeight + 2, nWriteWidth);
			printf("\033[%d;%dH   �� ��", nHeight + 3, nWriteWidth);
			printf("\033[%d;%dH   �� ��", nHeight + 4, nWriteWidth);
			printf("\033[%d;%dH   ������", nHeight + 5, nWriteWidth);
			break;

		case 8:
			printf("\033[%d;%dH������������", nHeight + 0, nWriteWidth);
			printf("\033[%d;%dH�� ���� ��", nHeight + 1, nWriteWidth);
			printf("\033[%d;%dH�� ���� ��", nHeight + 2, nWriteWidth);
			printf("\033[%d;%dH�� ���� ��", nHeight + 3, nWriteWidth);
			printf("\033[%d;%dH�� ���� ��", nHeight + 4, nWriteWidth);
			printf("\033[%d;%dH������������", nHeight + 5, nWriteWidth);
			break;

		case 9:
			printf("\033[%d;%dH������������", nHeight + 0, nWriteWidth);
			printf("\033[%d;%dH�� ���� ��", nHeight + 1, nWriteWidth);
			printf("\033[%d;%dH�� ���� ��", nHeight + 2, nWriteWidth);
			printf("\033[%d;%dH�������� ��", nHeight + 3, nWriteWidth);
			printf("\033[%d;%dH�������� ��", nHeight + 4, nWriteWidth);
			printf("\033[%d;%dH������������", nHeight + 5, nWriteWidth);
			break;
		}
	}
}