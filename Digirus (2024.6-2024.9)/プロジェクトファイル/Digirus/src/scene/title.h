//=============================================================
//
// �^�C�g�� [title.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "scene.h"
#include "gameobject.h"

// �^�C�g���V�[��
class CTitleScene : public CScene
{
public:
	void Init() override;						// ������
	void Uninit() override;						// �I��
	void Update() override;					// �X�V
	void Draw() override;						// �`��

	// �I����
	enum CHOICE
	{
		START,			// �J�n����
		SETTING,		// �ݒ�
		END				// �I������
	};

	static const std::string IMG_BG;
	static const std::string IMG_START;
	static const std::string IMG_SETTING;
	static const std::string IMG_END;
	static const D3DXVECTOR2 CHOICE_POS;
	static const float CHOICE_WIDTH;
	static const float CHOICE_HEIGHT;
	static const float CHOICE_SPACE;
	static const float CHOICED_ALPHA;

	static const D3DXVECTOR2 BAR_SIZE;
	static const float BAR_WIDTHPOS;
private:
	void InitChoices();							// �I�����̏�����
	void UpdateChoices();						// �I����Ԃ̍X�V

	GameObject* m_pBG;					// �w�i
	GameObject* m_pStartChoice;		// �I����: �J�n����
	GameObject* m_pSettingChoice;		// �I����: �ݒ�
	GameObject* m_pEndChoice;			// �I����: �I������
	GameObject* m_pSelectBar;			// �I���o�[

	CHOICE m_choice;							// �I�����
};

#endif // !_TITLE_H_
