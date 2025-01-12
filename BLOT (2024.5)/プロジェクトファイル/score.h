//===================================================================================
//
// �X�R�A [score.h]
// Author: Ito Kotaro
//
//===================================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

// �}�N����`
#define RANKING_PATH		"./data/ranking.bin"

// �X�R�A�N���X�̒�`
class CScore
{
public:
	CScore();							// �R���X�g���N�^
	void Draw(int nHeight);		// �`��

	void AddScore(int nAdd) { m_nScore += nAdd; }		// �X�R�A�̉��Z
	void SetScore(int nScore) { m_nScore = nScore; }	// �X�R�A�̐ݒ�
	int GetScore() { return m_nScore; }						// �X�R�A�̎擾
	int RegisterScore();												// �X�R�A��o�^

	// ��`
	static const int NUM_WIDTH = 6;				// �����̉�
	static const int NUM_HEIGHT = 6;				// �����̍���
	static const int NUM_SPACE = 1;					// �����̊Ԋu
	static const int SCORE_HEIGHT = 8;			// �X�R�A�̍���
	static const int CENTER = 94;						// ����
	static const int MAX_RANKING = 10;			// �����L���O�̍ő吔
	
private:
	int m_nScore;			// �X�R�A
};

#endif // !_SCORE_H_
