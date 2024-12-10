//=============================================================
//
// シャード [shard.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "shard.h"
#include "scene/game.h"
#include "component/other/sound.h"

// 静的メンバ変数の初期化
const int CShard::SHARD_SCORE = 80;

//=============================================================
// [CShard] 更新
//=============================================================
void CShard::Update()
{
	gameObject->FindNameChildren("model")->transform->Rotate(0.0f, 0.01f, 0.0f);
}

//=============================================================
// [CShard] トリガーに侵入したとき
//=============================================================
void CShard::OnTriggerEnter(GameObject* other)
{
	if (other->GetName() == "Player")
	{
		// オブジェクトを破棄する
		gameObject->Destroy();

		// スコアを追加する
		CGameScene* pGameScene = (CGameScene*)CSceneManager::GetInstance()->GetScene("game")->pScene;
		pGameScene->AddScore(SHARD_SCORE);

		// 音
		GameObject* pSound = new GameObject;
		pSound->AddComponent<CSound>();
		pSound->GetComponent<CSound>()->LoadWav("data\\SOUND\\SE\\pickup_shard.wav");
		pSound->GetComponent<CSound>()->Play();
		pSound->GetComponent<CSound>()->IsStoppedDestroy();
	}
}