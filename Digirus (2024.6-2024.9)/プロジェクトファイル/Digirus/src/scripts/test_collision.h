//=============================================================
//
// �e�X�g�R���W���� [test_collision.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _TEST_COLLISION_H_
#define _TEST_COLLISION_H_

#include "component.h"

class CTestCollision : public Component
{
public:
	void Init()
	{
		m_count = 0;
	}

	void OnTriggerStay(GameObject* other) override
	{
		printf("�I�u�W�F�N�g�̐ڐG���m�I");
		m_count++;
	}

	void OnTriggerEnter(GameObject* other) override
	{
		printf("�������悤�ł�");
	}

	void OnTriggerExit(GameObject* other) override
	{
		printf("�o���悤�ł�");
		other->Destroy();
	}

private:
	int m_count;
};

#endif // !_TEST_COLLISION_H_
