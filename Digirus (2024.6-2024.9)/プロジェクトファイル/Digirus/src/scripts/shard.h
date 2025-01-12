//=============================================================
//
// シャード [shard.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _SHARD_H_
#define _SHARD_H_

#include "component.h"

// シャード
class CShard : public Component
{
public:
	void Update() override;
	void OnTriggerEnter(GameObject* other) override;

	static const int SHARD_SCORE;
private:

};

#endif // !_SHARD_H_
