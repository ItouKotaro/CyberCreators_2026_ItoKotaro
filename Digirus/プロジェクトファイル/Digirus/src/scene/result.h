//=============================================================
//
// リザルトシーン [result.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "scene.h"
#include "gameobject.h"
#include "component.h"

// リザルトシーン
class CResultScene : public CScene
{
public:
	void Init() override;			// 初期化
	void Uninit() override;			// 終了
	void Update() override;		// 更新
	void Draw() override;			// 描画
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
