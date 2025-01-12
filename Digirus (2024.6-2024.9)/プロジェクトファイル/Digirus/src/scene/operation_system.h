//=============================================================
//
// OS [operation_system.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _OS_H_
#define _OS_H_

#include "scene.h"
#include "gameobject.h"
#include "component.h"
#include "scripts/cursor.h"

class CFileListContent : public Component
{
public:
	// �t�@�C���̎��
	enum FILETYPE
	{
		FOLDER,	// �t�H���_
		WORLD,	// ���[���h
		TEXT,		// �e�L�X�g
		BACK			// �߂�
	};

	void Init() override;
	void Update() override;
	void SetTitle(const std::string& title);
	void SetType(const FILETYPE& type);
	void SetPath(const std::string& path) { m_sPath = path; }
	void SetDefaultPos(const D3DXVECTOR2& pos) { m_defaultPos = pos; }
	D3DXVECTOR2 GetDefaultPos() { return m_defaultPos; }
	FILETYPE GetType() { return m_fileType; }
	std::string GetPath() { return m_sPath; }
	std::string GetName() { return m_sName; }
	bool GetSelectedArea(const POINT& point);
	void OnClick();
	bool GetDoubleClick();

	static const int FILELIST_WIDTH;		// ���[���h���X�g�̉���
	static const int FILELIST_HEIGHT;		// ���[���h���X�g�̍���
	static const int FILELIST_EDGE;			// ���[���h���X�g�̉�
	static const int FILELIST_ICONSIZE;	// �A�C�R���T�C�Y
	static const int DOUBLECLICK_TIME;	// �_�u���N���b�N�̎���
private:
	GameObject* m_pFileBG;
	GameObject* m_pFileEdge;
	GameObject* m_pFileTitle;
	GameObject* m_pFileIcon;
	std::string m_sPath;
	std::string m_sName;
	FILETYPE m_fileType;
	int m_nDoubleCounter;						// �_�u���N���b�N�̃J�E���^�[
	D3DXVECTOR2 m_defaultPos;
};


// �z�[���V�[��
class COperationSystem : public CScene
{
public:
	void Init() override;			// ������
	void Uninit() override;			// �I��
	void Update() override;		// �X�V
	void Draw() override;			// �`��

	POINT GetCursorPoint();
	static CFileListContent* GetSelectedFileContent() { return m_pSelectedFileContent; }
	static void SetWorld(const std::string& sPath);

	// �J�[�\���f�o�C�X
	enum CursorDevice
	{
		MOUSE,
		CONTROLLER,
	};

	// �I���^�u
	enum SELECT_TAB
	{
		HOME,
		PLAY,
		MAX
	};

	// �ǂݍ��񂾃t�@�C���f�[�^
	struct FileContent
	{
		std::string sPath;		// �p�X
		std::string sName;		// ���O
		CFileListContent::FILETYPE fileType;		// ���
	};

	static const int TABBAR_WIDTH;			// �^�u�o�[�̕�
	static const int TABBAR_HEIGHT;		// �^�u�o�[�̍���
	static const int LOGO_SIZE;				// ���S�̃T�C�Y
	static const int PLAYBAR_WIDTH;		// �v���C�^�u�̃o�[�̕�
	static const int PLAYBAR_HEIGHT;		// �v���C�^�u�̃o�[�̍���
	static const int POWER_SIZE;				// �d���A�C�R���̃T�C�Y
	static const char* LOAD_PATH;			// �ǂݍ��ރp�X
	static const char* INFO_FILENAME;		// ���t�@�C����
	static const char* WORLD_FILENAME;	// ���[���h�t�@�C����
	static const std::string BACK_NAME;	// �߂�{�^���̕\����
	static const int GUIDE_WIDTH;			// �K�C�h�̉���
	static const int GUIDE_HEIGHT;			// �K�C�h�̍���
private:
	void UpdateTab();				// �^�u�̍X�V����
	void LoadFiles();				// �t�@�C����ǂݍ���

	void InitHomeTab();			// �z�[���^�u�̏���
	void InitPlayTab();				// �v���C�^�u�̏���

	void UpdatePlayTab();			// �v���C�^�u�̍X�V����

	SELECT_TAB m_selectTab;			// �I�𒆂̃^�u
	GameObject* m_selectTabObj;	// �I���^�u�̔w�i�I�u�W�F�N�g
	GameObject* m_pPowerObj;		// �d��
	GameObject* m_pLogoObj;		// ���S
	int m_nLogoType;						// ���S�̐؂�ւ�
	int m_nLogoCounter;					// ���S�̐؂�ւ��J�E���^�[
	int m_nScroll;							// �X�N���[���l
	CCursor* m_pCursor;					// �J�[�\��
	GameObject* m_pGuide;			// �K�C�h�\��

	// ���[���h�I��
	char m_sCurrentDirectory[MAX_PATH];					// ���݂̃f�B���N�g��
	GameObject* m_thumbnailObj;								// �T���l�C��
	GameObject* m_limitTimeObj;								// ��������
	static CFileListContent* m_pSelectedFileContent;		// �I���ς݂̃t�@�C���R���e���c
};


// �E�B���h�E�N���X�i���j
class CWindowObject : public Component
{
public:
	virtual void Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	void WindowClose();
	void SetVisible(const bool& bShow);
	void SetPos(const float& x, const float& y);
	void SetSize(const D3DXVECTOR2& size);
	void SetTitle(const std::string& title);
	void SetColorBG(const D3DXCOLOR& color);
	void AddWindowObj(GameObject* pGameObject);
	bool GetSelectedArea(const POINT& point);
	static bool GetSelectedAllArea(const POINT& point);

	static const int TITLEBAR_HEIGHT;
	static const int CLOSEBUTTON_SIZE;	// ����{�^���̃T�C�Y
private:
	void Move();									// �E�B���h�E�𓮂���

	D3DXVECTOR2 m_windowSize;		// �E�B���h�E�T�C�Y
	GameObject* m_pTitleBarObj;		// �^�C�g���o�[
	GameObject* m_pWindowObj;		// �E�B���h�E�̈�
	GameObject* m_pCloseObj;			// ����{�^��
	GameObject* m_pTitleTextObj;		// �^�C�g���e�L�X�g
	GameObject* m_pCloseButtonObj;	// �N���[�Y
	POINT m_oldPoint;							// �O��̃}�E�X�ʒu
	bool m_bVisible;								// �\�����邩

	static CWindowObject* m_pTopWindow;
	static CWindowObject* m_pDragging;
	static std::vector<CWindowObject*> m_pWindowObjects;
};

// �I���E�B���h�E
class CCloseWindow : public CWindowObject
{
public:
	void Init() override;
	void Update() override;

	static const int BUTTON_WIDTH;
	static const int BUTTON_HEIGHT;
private:
	GameObject* m_pConfirmText;
	GameObject* m_pWarningIcon;
	GameObject* m_pYesObj;
	GameObject* m_pNoObj;
};

// ���[���h�E�B���h�E
class CWorldWindow : public CWindowObject
{
public:
	void Init() override;
	void Update() override;
	void SetWorldName(const std::string& sWorldName) { m_sName = sWorldName; }
	void SetWorldPath(const std::string& sWorldPath) { m_sPath = sWorldPath; }

	static const int BUTTON_WIDTH;
	static const int BUTTON_HEIGHT;
private:
	std::string m_sPath;
	std::string m_sName;

	GameObject* m_pYesObj;
	GameObject* m_pNoObj;
	GameObject* m_pStageNameObj;
	GameObject* m_pConfirmObj;
};

#endif // !_OS_H_
