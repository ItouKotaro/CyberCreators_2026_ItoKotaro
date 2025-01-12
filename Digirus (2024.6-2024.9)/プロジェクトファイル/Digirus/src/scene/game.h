//=============================================================
//
// �Q�[���V�[�� [game.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "scene.h"
#include "gameobject.h"
#include "world_loader.h"
#include "scripts/pause.h"

// �X�e�[�W���[�_�[
class StageLoader : public WorldLoader
{
public:
	void CustomObject(GameObject* gameObject) override;
};

// �Q�[���V�[��
class CGameScene : public CScene
{
public:
	void Init() override;			// ������
	void Uninit() override;			// �I��
	void Update() override;		// �X�V
	void Draw() override;			// �`��
	void SetWorld(const std::string& path);
	void SetLimitTime(const int& nSec);
	void SetAmbient(const float& fAmbient);
	float GetAmbient() { return m_fAmbient; }
	void SetSkybox(const std::string& sPath);
	std::string GetSkybox() { return m_sSkyboxPath; }
	void SetBGM(const std::string& sPath, const float& fVolume);
	void onInitGoal();
	void FinishGoal();
	void UpdateGoal();
	void AddScore(const int& nScore) { m_nScore += nScore; }
	LPDIRECT3DTEXTURE9 GetResultTexture();
	void OnGameOver();
	bool IsGoal() { return m_bIsGoal; }
	GameObject* GetClockSound() { return m_pClockSound; }

	void SavePlayData();
	int GetBestTime();

	static const int RESULT_BAR_SPACE;
	static const D3DXVECTOR2 TIME_POS;
private:
	void InitCamera();				// �J�����̏�����
	void InitLight();					// ���C�g�̏�����
	void InitResult();				// ���U���g�̏�����
	void UpdateResult();			// ���U���g�̍X�V
	void CenterCosor();			// �J�[�\���𒆉��Ɉړ����鏈��
	void UpdateTime();				// ���Ԃ̍X�V
	void UpdateStartAnim();		// �X�^�[�g�A�j���[�V�����i�t�F�[�h�ƃX�e�[�W���Ɛ������ԁj

	// �v���C�f�[�^
	struct PlayData
	{
		unsigned int nClearTime;
		unsigned int nBestScore;
	};

	// �Q�[�����
	char m_sWorldPath[MAX_PATH];		// ���[���h�̃p�X
	char m_sWorldName[MAX_PATH];	// ���[���h��
	char m_sSkyboxPath[MAX_PATH];	// �X�J�C�{�b�N�X�̃p�X
	char m_sBGMPath[MAX_PATH];		// BGM�̃p�X
	float m_fBGMVolume;						// BGM�̉���
	float m_fAmbient;							// ���邳

	// �Q�[���v�f
	GameObject* m_pPause;			// �|�[�Y
	GameObject* m_pClockSound;	// ���Ԑ���SE
	GameObject* m_pFade;				// �t�F�[�h
	GameObject* m_pBGM;				// BGM
	GameObject* m_pCamera;			// �J����
	GameObject* m_pSkillGuide;		// �X�L���̃L�[�\��
	StageLoader* m_pStageLoader;	// �X�e�[�W���[�_�[

	// �^�C�g���\��
	GameObject* m_pTitleName;		// �^�C�g�����i���Ձj
	int m_nTitleCounter;					// �^�C�g���\������
	int m_nTitleTimeCounter;			// �^�C���^�C�g���\������
	float m_fTitleAlpha;					// �^�C�g���\���̓����x

	// ���Ԋ֌W
	GameObject* m_pTimeUI;			// ���Ԑ���UI
	int m_nCurrentTime;
	int m_nLimitTime;						// ��������
	int m_nClearTime;						// �N���A�^�C���i�~���b�j
	bool m_bIsGoal;							// �S�[��
	int m_nScore;							// �X�R�A
	float m_fScoreFade;					// �X�R�A�̃t�F�[�h

	// ���U���g
	GameObject* m_pResultRender;
	GameObject* m_pScoreText;			// �X�R�A�̃e�L�X�g
	GameObject* m_pClearTimeText;	// �N���A�^�C���̃e�L�X�g
	GameObject* m_pBestTimeText;		// �x�X�g�^�C���̃e�L�X�g
	float m_fAlphaUI;

	// �Q�[���I�[�o�[
	bool m_bGameOver;						// �Q�[���I�[�o�[
};

#endif // !_GAME_H_
