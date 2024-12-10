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
	// ファイルの種類
	enum FILETYPE
	{
		FOLDER,	// フォルダ
		WORLD,	// ワールド
		TEXT,		// テキスト
		BACK			// 戻る
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

	static const int FILELIST_WIDTH;		// ワールドリストの横幅
	static const int FILELIST_HEIGHT;		// ワールドリストの高さ
	static const int FILELIST_EDGE;			// ワールドリストの縁
	static const int FILELIST_ICONSIZE;	// アイコンサイズ
	static const int DOUBLECLICK_TIME;	// ダブルクリックの時間
private:
	GameObject* m_pFileBG;
	GameObject* m_pFileEdge;
	GameObject* m_pFileTitle;
	GameObject* m_pFileIcon;
	std::string m_sPath;
	std::string m_sName;
	FILETYPE m_fileType;
	int m_nDoubleCounter;						// ダブルクリックのカウンター
	D3DXVECTOR2 m_defaultPos;
};


// ホームシーン
class COperationSystem : public CScene
{
public:
	void Init() override;			// 初期化
	void Uninit() override;			// 終了
	void Update() override;		// 更新
	void Draw() override;			// 描画

	POINT GetCursorPoint();
	static CFileListContent* GetSelectedFileContent() { return m_pSelectedFileContent; }
	static void SetWorld(const std::string& sPath);

	// カーソルデバイス
	enum CursorDevice
	{
		MOUSE,
		CONTROLLER,
	};

	// 選択タブ
	enum SELECT_TAB
	{
		HOME,
		PLAY,
		MAX
	};

	// 読み込んだファイルデータ
	struct FileContent
	{
		std::string sPath;		// パス
		std::string sName;		// 名前
		CFileListContent::FILETYPE fileType;		// 種類
	};

	static const int TABBAR_WIDTH;			// タブバーの幅
	static const int TABBAR_HEIGHT;		// タブバーの高さ
	static const int LOGO_SIZE;				// ロゴのサイズ
	static const int PLAYBAR_WIDTH;		// プレイタブのバーの幅
	static const int PLAYBAR_HEIGHT;		// プレイタブのバーの高さ
	static const int POWER_SIZE;				// 電源アイコンのサイズ
	static const char* LOAD_PATH;			// 読み込むパス
	static const char* INFO_FILENAME;		// 情報ファイル名
	static const char* WORLD_FILENAME;	// ワールドファイル名
	static const std::string BACK_NAME;	// 戻るボタンの表示名
	static const int GUIDE_WIDTH;			// ガイドの横幅
	static const int GUIDE_HEIGHT;			// ガイドの高さ
private:
	void UpdateTab();				// タブの更新処理
	void LoadFiles();				// ファイルを読み込む

	void InitHomeTab();			// ホームタブの処理
	void InitPlayTab();				// プレイタブの処理

	void UpdatePlayTab();			// プレイタブの更新処理

	SELECT_TAB m_selectTab;			// 選択中のタブ
	GameObject* m_selectTabObj;	// 選択タブの背景オブジェクト
	GameObject* m_pPowerObj;		// 電源
	GameObject* m_pLogoObj;		// ロゴ
	int m_nLogoType;						// ロゴの切り替え
	int m_nLogoCounter;					// ロゴの切り替えカウンター
	int m_nScroll;							// スクロール値
	CCursor* m_pCursor;					// カーソル
	GameObject* m_pGuide;			// ガイド表示

	// ワールド選択
	char m_sCurrentDirectory[MAX_PATH];					// 現在のディレクトリ
	GameObject* m_thumbnailObj;								// サムネイル
	GameObject* m_limitTimeObj;								// 制限時間
	static CFileListContent* m_pSelectedFileContent;		// 選択済みのファイルコンテンツ
};


// ウィンドウクラス（基底）
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
	static const int CLOSEBUTTON_SIZE;	// 閉じるボタンのサイズ
private:
	void Move();									// ウィンドウを動かす

	D3DXVECTOR2 m_windowSize;		// ウィンドウサイズ
	GameObject* m_pTitleBarObj;		// タイトルバー
	GameObject* m_pWindowObj;		// ウィンドウ領域
	GameObject* m_pCloseObj;			// 閉じるボタン
	GameObject* m_pTitleTextObj;		// タイトルテキスト
	GameObject* m_pCloseButtonObj;	// クローズ
	POINT m_oldPoint;							// 前回のマウス位置
	bool m_bVisible;								// 表示するか

	static CWindowObject* m_pTopWindow;
	static CWindowObject* m_pDragging;
	static std::vector<CWindowObject*> m_pWindowObjects;
};

// 終了ウィンドウ
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

// ワールドウィンドウ
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
