//=============================================================
//
// �f�o�b�N�V�[�� [debug.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _DEBUG_SCENE_H_
#define _DEBUG_SCENE_H_

#include "scene.h"
#include "world_loader.h"

// �f�o�b�N�V�[��
class CDebugScene : public CScene
{
public:
	void Init() override;			// ������
	void Uninit() override;			// �I��
	void Update() override;		// �X�V
	void Draw() override;			// �`��
private:
	WorldLoader* m_worldLoader;
	GameObject* m_pCamera;	// �J����
};

#endif // !_DEBUG_SCENE_H_
