//=============================================================
//
// デバックシーン [debug.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _DEBUG_SCENE_H_
#define _DEBUG_SCENE_H_

#include "scene.h"
#include "world_loader.h"

// デバックシーン
class CDebugScene : public CScene
{
public:
	void Init() override;			// 初期化
	void Uninit() override;			// 終了
	void Update() override;		// 更新
	void Draw() override;			// 描画
private:
	WorldLoader* m_worldLoader;
	GameObject* m_pCamera;	// カメラ
};

#endif // !_DEBUG_SCENE_H_
