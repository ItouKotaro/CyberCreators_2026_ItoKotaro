//===================================================================================
//
// �v���C���[ [player.h]
// Author: Ito Kotaro
//
//===================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

// �}�N����`
#define STACK_CHAR1	"����������"
#define STACK_CHAR2	"��   ��"
#define STACK_CHAR3	"����������"
#define STACK_LINK		"����������"
#define PLAYER_CHAR1	"����������"
#define PLAYER_CHAR2	"��   ��"
#define PLAYER_CHAR3	"����������"

// �v���C���[�N���X�̒�`
class CPlayer
{
public:
	CPlayer();			// �R���X�g���N�^
	~CPlayer();		// �f�X�g���N�^

	void InputStacks();					// �ςݏd�ˏ���
	void Draw(int nHeight);			// �v���C���[�̕`��

	int GetPos() { return m_nPos; }
	int GetStacks() { return m_nStacks; }

	void SetPos(int nPos) { m_nPos = nPos; }
	void SetStacks(int nStacks) { m_nStacks = nStacks; }

	// �萔��`
	static const int PLAYER_WIDTH = 35;				// ���̃X�^�[�g�ʒu
	static const int PLAYER_HEIGHT = 45;				// �����̃X�^�[�g�ʒu

	static const int MAX_STACK = 20;					// �X�^�b�N�̍ő吔
	static const int STACK_INTERVAL = 300;			// �X�^�b�N�̊Ԋu
	static const int STEP_INTERVAL = 250;			// �i�ނƂ��̊Ԋu
private:
	int m_nStacks;	// �ςݏd�˂Ă��鐔
	int m_nPos;		// �ʒu
};

#endif // !_PLAYER_H_
