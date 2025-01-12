//=============================================================
//
// ジャンプパッド [jumppad.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _JUMPPAD_H_
#define _JUMPPAD_H_

#include "component.h"

// ジャンプパッド
class CJumppad : public Component
{
public:
	void OnTriggerStay(GameObject* other) override;
	void SetPower(const float& fPower) { m_fPower = fPower; }
private:
	float m_fPower;
};

#endif // !_JUMPPAD_H_
