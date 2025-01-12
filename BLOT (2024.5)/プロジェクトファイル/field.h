//===================================================================================
//
// �t�B�[���h [field.h]
// Author: Ito Kotaro
//
//===================================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

// �}�N����`
#define BLOCK_FIELD_CHAR1	"����������"	
#define BLOCK_FIELD_CHAR2	"����������"
#define BLOCK_FIELD_CHAR3	"     "
#define BLOCK_PLAYER_CHAR1	"����������"
#define BLOCK_PLAYER_CHAR2	"��   ��"
#define BLOCK_PLAYER_CHAR3	"����������"

// �t�B�[���h�N���X�̒�`
class CField
{
public:
	CField();	// �R���X�g���N�^

	// �u���b�N�̎��
	typedef enum
	{
		BLOCK_NONE = 0,		// �Ȃ�
		BLOCK_FIELD,			// �t�B�[���h
		BLOCK_PLAYER,			// �v���C���[
		BLOCK_MAX
	}BLOCK;

	void Generate(int nScore);
	void Set(int nPos, BLOCK type);
	BLOCK Get(int nPos);
	void Draw(int nHeight);

	static const int FIELD_LENGTH = 25;				// �t�B�[���h�̒���
	static const int TILE_WIDTH = 5;					// 1�}�X�̉���
	static const int TILE_HEIGHT = 3;					// 1�}�X�̍���
	static const int FIELD_WIDTH = 35;					// ���̃X�^�[�g�ʒu
	static const int FIELD_HEIGHT = 45;				// �����̃X�^�[�g�ʒu
	static const int MIN_DISTANCE = 2;					// �ŏ��̗���Ă��鋗��
	static const int MAX_SPACE = 4;						// �ő�̃X�y�[�X
	static const int SUPPORT_DISTANCE = 50;		// �T�|�[�g���鋗���i��Փx���j
	static const int CHANCE_DISTANCE = 10;		// �X�y�[�X��������`�����X�̋���
private:
	BLOCK m_aBlock[FIELD_LENGTH];					// �t�B�[���h��̃u���b�N�f�[�^
};

#endif // !_FIELD_H_
