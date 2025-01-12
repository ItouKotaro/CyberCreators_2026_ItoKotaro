//=============================================================
//
// �V���[�h [shard.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "shard.h"
#include "scene/game.h"
#include "component/other/sound.h"

// �ÓI�����o�ϐ��̏�����
const int CShard::SHARD_SCORE = 80;

//=============================================================
// [CShard] �X�V
//=============================================================
void CShard::Update()
{
	gameObject->FindNameChildren("model")->transform->Rotate(0.0f, 0.01f, 0.0f);
}

//=============================================================
// [CShard] �g���K�[�ɐN�������Ƃ�
//=============================================================
void CShard::OnTriggerEnter(GameObject* other)
{
	if (other->GetName() == "Player")
	{
		// �I�u�W�F�N�g��j������
		gameObject->Destroy();

		// �X�R�A��ǉ�����
		CGameScene* pGameScene = (CGameScene*)CSceneManager::GetInstance()->GetScene("game")->pScene;
		pGameScene->AddScore(SHARD_SCORE);

		// ��
		GameObject* pSound = new GameObject;
		pSound->AddComponent<CSound>();
		pSound->GetComponent<CSound>()->LoadWav("data\\SOUND\\SE\\pickup_shard.wav");
		pSound->GetComponent<CSound>()->Play();
		pSound->GetComponent<CSound>()->IsStoppedDestroy();
	}
}