//=============================================================
//
// テストコリジョン [test_collision.h]
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
		printf("オブジェクトの接触検知！");
		m_count++;
	}

	void OnTriggerEnter(GameObject* other) override
	{
		printf("入ったようです");
	}

	void OnTriggerExit(GameObject* other) override
	{
		printf("出たようです");
		other->Destroy();
	}

private:
	int m_count;
};

#endif // !_TEST_COLLISION_H_
