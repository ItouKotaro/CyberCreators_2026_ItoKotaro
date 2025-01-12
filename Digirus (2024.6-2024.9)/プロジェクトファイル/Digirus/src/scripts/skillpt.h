//=============================================================
//
// �X�L���|�C���g [skillpt.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _SKILLPT_H_
#define _SKILLPT_H_

#include "component.h"
#include "skillpt_ui.h"

class CSkillPoint : public Component
{
public:
	void Init() override;							// ������
	void Update() override;						// �X�V
	void AddPoint(const int& nAddPoint);	// �|�C���g��ǉ�
	int GetPoint() { return m_nPoint; }		// �|�C���g�̎擾
	static const int MAX_POINT;				// �ő�|�C���g
private:
	void SetPoint(const int& nPoint);			// �|�C���g�̐ݒ�
	int m_nPoint;										// �|�C���g
	GameObject* m_pUI;							// UI
};

#endif // !_SKILLPT_H_
