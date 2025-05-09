//=============================================================
//
// �W�F�� [gem.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _GEM_H_
#define _GEM_H_

#include "component.h"
class Gem : public Component
{
public:
	void Init() override;
	void Update() override;
	void OnTriggerEnter(GameObject* other) override;
private:
	GameObject* m_particle;
	GameObject* m_camera;

	static AudioClip m_pickupSE;
	const float RENDER_DISTANCE = 1500.0f;
	const int POINT_VALUE = 2;
};

#endif // !_GEM_H_
