//=============================================================
//
// スタートシーン [start_scene.cpp]
// Author: Ito Kotaro
// 
// アプリケーション開始後、最初に読み込まれるシーンです
// シーンの登録を行うことをお勧めします
//
//=============================================================
#include "start_scene.h"
#include "manager.h"

// シーンのインクルード
#include "title.h"
#include "operation_system.h"
#include "game.h"
#include "result.h"
#include "debug.h"
#include "dead.h"

//=============================================================
// [CStartScene] 初期化
//=============================================================
void CStartScene::Init()
{
	// シーンマネージャーの取得
	CSceneManager* pSceneManager = CSceneManager::GetInstance();

	// シーンの登録
	pSceneManager->RegistScene<CTitleScene>("title");
	pSceneManager->RegistScene<COperationSystem>("os");
	pSceneManager->RegistScene<CGameScene>("game");
	pSceneManager->RegistScene<CResultScene>("result");
	pSceneManager->RegistScene<CDeadScene>("dead");

	// シーンの再生
	pSceneManager->SetScene("os");
}

//=============================================================
// [CStartScene] 終了
//=============================================================
void CStartScene::Uninit()
{

}

//=============================================================
// [CStartScene] 更新
//=============================================================
void CStartScene::Update()
{

}

//=============================================================
// [CStartScene] 描画
//=============================================================
void CStartScene::Draw()
{

}