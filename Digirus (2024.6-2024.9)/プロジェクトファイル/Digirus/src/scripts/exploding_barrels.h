//=============================================================
//
// ��������M [exploding_barrels.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _EXPLODING_BARRELS_H_
#define _EXPLODING_BARRELS_H_

#include "component.h"

class CExplodingBarrels : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Explosion();
	static const float EXPLOSION_RADIUS;		// �����͈�
	static const float EXPLOSION_POWER;			// �����З�
private:
	GameObject* m_pExplosionCol;
};

#endif // !_EXPLODING_BARRELS_H_
