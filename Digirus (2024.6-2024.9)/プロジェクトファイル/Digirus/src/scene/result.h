//=============================================================
//
// ���U���g�V�[�� [result.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "scene.h"
#include "gameobject.h"
#include "component.h"

// ���U���g�V�[��
class CResultScene : public CScene
{
public:
	void Init() override;			// ������
	void Uninit() override;			// �I��
	void Update() override;		// �X�V
	void Draw() override;			// �`��
private:
	GameObject* m_pBG;
};


class CResult : public Component
{
public:
	void Init() override;
private:
	GameObject* m_pBGObj;
};

#endif // !_RESULT_H_
