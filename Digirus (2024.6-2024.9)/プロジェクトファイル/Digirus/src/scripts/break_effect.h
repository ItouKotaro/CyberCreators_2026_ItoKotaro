//=============================================================
//
// �j��G�t�F�N�g [break_effect.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _BREAK_EFFECT_H_
#define _BREAK_EFFECT_H_

#include "component.h"

// �}�N����`
#define DEF_BREAKEFFECT_LIFE		(120)
#define DEF_BREAKEFFECT_SCALE		(1.0f)

// �j��G�t�F�N�g�R���|�[�l���g
class CBreakEffect : public Component
{
public:
	CBreakEffect(const int& nLife = DEF_BREAKEFFECT_LIFE, const float& fScale = DEF_BREAKEFFECT_SCALE);
	void Init() override;
	void Update() override;

	static const std::string MESH_PATH;
private:
	int m_nLife;								// ����
	float m_fScale;							// �X�P�[��
	GameObject* m_pPickupObj;		// �E���͈�
};

#endif // !_BREAK_EFFECT_H_
