//===================================================================================
//
// �Q�[���}�l�[�W���[ [game.cpp]
// Author: Ito Kotaro
//
//===================================================================================
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

// �ϐ��̏�����
const int CGameManager::STAGEMOVE_INTERVAL;
const int CGameManager::STAGEFALL_INTERVAL;
const int CGameManager::STAGEFALL_COUNT;
const int CGameManager::PLAYER_FALLSPEED;
const int CGameManager::RESULT_SCORE_HEIGHT;
const int CGameManager::RESULT_RANKING_WIDTH;
const int CGameManager::RESULT_RANKING_HEIGHT;
const int CGameManager::DESCRIPTION_WIDTH;
const int CGameManager::DESCRIPTION_HEIGHT;

//=======================================================
// [CGameManager] �R���X�g���N�^
//=======================================================
CGameManager::CGameManager() : m_IsGameOver(false), m_bStarted(false)
{
	 // �v���C���[���쐬
	m_pPlayer = new CPlayer();

	// �t�B�[���h���쐬
	m_pField = new CField();

	// �X�R�A���쐬
	m_pScore = new CScore();
}

//=======================================================
// [CGameManager] �f�X�g���N�^
//=======================================================
CGameManager::~CGameManager()
{
	// �v���C���[��j��
	if (m_pPlayer != NULL)
	{
		delete m_pPlayer;
		m_pPlayer = NULL;
	}

	// �t�B�[���h��j��
	if (m_pField != NULL)
	{
		delete m_pField;
		m_pField = NULL;
	}

	// �X�R�A��j��
	if (m_pScore != NULL)
	{
		delete m_pScore;
		m_pScore = NULL;
	}
}

//=======================================================
// [CGameManager] �Q�[���J�n
//=======================================================
void CGameManager::StartGame()
{
	while (1)
	{
		// ��������
		m_pField->Generate(m_pScore->GetScore());

		// �`��
        Draw();

		// �ςݏd�˂̑��쏈��
		m_pPlayer->InputStacks();

		// �i��
		Step();

		// �Q�[���I�[�o�[����
		if (m_IsGameOver)
		{
			GameOver();
			break;
		}
	}

}

//=======================================================
// [CGameManager] �`��
//=======================================================
void CGameManager::Draw()
{
	// �v���C���[�̈���N���A
	for (int nCntBlock = 0; nCntBlock < CPlayer::MAX_STACK * CField::TILE_HEIGHT; nCntBlock++)
	{
		printf("\033[%d;%dH\033[2K", CPlayer::PLAYER_HEIGHT - nCntBlock, CPlayer::PLAYER_WIDTH);
	}

	// �v���C���[�̕`��
	m_pPlayer->Draw(CPlayer::PLAYER_HEIGHT);

	// �t�B�[���h�̕`��
	m_pField->Draw(CField::FIELD_HEIGHT);

	// �X�R�A�̕`��
	m_pScore->Draw(CScore::SCORE_HEIGHT);

	// ���[�������̕`��i�i�ޑO�̂݁j
	if (!m_bStarted)
	{
		DescriptionDraw();
	}
}

//=======================================================
// [CGameManager] �����̕`��
//=======================================================
void CGameManager::DescriptionDraw()
{
	printf("\033[%d;%dH", DESCRIPTION_HEIGHT, DESCRIPTION_WIDTH);
	printf("�n�ʂɓ͂��悤�Ƀu���b�N��ςݏグ��Q�[��");
	printf("\033[%d;%dH", DESCRIPTION_HEIGHT + 3, DESCRIPTION_WIDTH + 8);
	printf("SPACE�L�[�������Őςݏグ��");
}

//=======================================================
// [CGameManager] �i��
//=======================================================
void CGameManager::Step()
{
	int nStacks = m_pPlayer->GetStacks();
	int nPos = m_pPlayer->GetPos();
	m_bStarted = true;

	// ���X�e�b�v���邩���߂�
	int nStepNum = nStacks + 1;

	for (int i = 0; i < nStepNum; i++)
	{
		// �ړ�
		nPos++;

		// �u���b�N��ݒu����
		if (nStacks > 0)
		{ // �u���b�N������̂Ȃ�

			if (m_pField->Get(nPos) == CField::BLOCK_NONE)
			{ // �u���b�N���Ȃ��Ƃ�
				m_pField->Set(nPos, CField::BLOCK_PLAYER);		// �u���b�N�ݒu
			}
				
			nStacks--;		// �u���b�N�����炷
		}

		// �v���C���[�̏����X�V����
		m_pPlayer->SetPos(nPos);
		m_pPlayer->SetStacks(nStacks);

		// �`��
		Draw();

		// �҂�
		Sleep(CPlayer::STEP_INTERVAL);
	}

	// �ŏI�����n�_�̃u���b�N�͂Ȃɂ�
	CField::BLOCK currentBlock = m_pField->Get(nPos);
	if (currentBlock == CField::BLOCK_NONE)
	{	// �u���b�N���Ȃ��Ƃ��i�Q�[���I�[�o�[�j
		m_IsGameOver = true;
		return;
	}

	// �X�R�A�����Z����
	m_pScore->AddScore(nPos);

	// ���ݒn�_�̈ʒu��0�܂ňړ�������
	ResetPos();
}

//=======================================================
// [CGameManager] �Q�[���I�[�o�[
//=======================================================
void CGameManager::GameOver()
{
	// �����L���O�ɓo�^����
	int nRank = m_pScore->RegisterScore();

	for (int i = 0; i < STAGEFALL_COUNT; i++)
	{
		// �v���C���[�̈���N���A
		for (int nCntBlock = 0; nCntBlock < CPlayer::MAX_STACK * CField::TILE_HEIGHT + CField::TILE_HEIGHT; nCntBlock++)
		{
			printf("\033[%d;%dH\033[2K", CPlayer::PLAYER_HEIGHT + CField::TILE_HEIGHT - nCntBlock, CPlayer::PLAYER_WIDTH);
		}

		// �v���C���[�̕`��
		m_pPlayer->Draw(CPlayer::PLAYER_HEIGHT - i / PLAYER_FALLSPEED);

		// �t�B�[���h�̕`��
		if (CField::FIELD_HEIGHT - i > 0)
			m_pField->Draw(CField::FIELD_HEIGHT - i);

		// �Ō�X�R�A�̕`��
		if (i >= STAGEFALL_COUNT - RESULT_SCORE_HEIGHT)
			m_pScore->Draw(i - (STAGEFALL_COUNT - RESULT_SCORE_HEIGHT));

		// �����L���O�̕`��
		if (nRank != -1)
		{ // �����N�C�����Ă���Ƃ�
			if (i >= STAGEFALL_COUNT - RESULT_RANKING_HEIGHT)
				printf("\033[%d;%dH%d�ʂɃ����N�C�����܂���", i - (STAGEFALL_COUNT - RESULT_RANKING_HEIGHT), RESULT_RANKING_WIDTH, nRank);
		}

		// �҂�
		Sleep(STAGEFALL_INTERVAL);
	}
}

//=======================================================
// [CGameManager] �ʒu�����Z�b�g
//=======================================================
void CGameManager::ResetPos()
{
	// �v���C���[�̈ʒu���擾
	int nPos = m_pPlayer->GetPos();

	for (int i = 1; i <= nPos; i++)
	{
		// �v���C���[�̈ʒu�����炵�Ă���
		m_pPlayer->SetPos(nPos - i);

		// �u���b�N�����炷
		for (int nCntBlock = 0; nCntBlock < CField::FIELD_LENGTH - 1; nCntBlock++)
		{
			m_pField->Set(nCntBlock, m_pField->Get(nCntBlock + 1));
		}
		m_pField->Set(CField::FIELD_LENGTH - 1, CField::BLOCK_NONE);

		// �`�悷��
		Draw();

		// �ړ��Ԋu
		Sleep(STAGEMOVE_INTERVAL);
	}
}