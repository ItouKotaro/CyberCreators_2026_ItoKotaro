//=============================================================
//
// タイトル [title.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "scene.h"
#include "gameobject.h"

// タイトルシーン
class CTitleScene : public CScene
{
public:
	void Init() override;						// 初期化
	void Uninit() override;						// 終了
	void Update() override;					// 更新
	void Draw() override;						// 描画

	// 選択肢
	enum CHOICE
	{
		START,			// 開始する
		SETTING,		// 設定
		END				// 終了する
	};

	static const std::string IMG_BG;
	static const std::string IMG_START;
	static const std::string IMG_SETTING;
	static const std::string IMG_END;
	static const D3DXVECTOR2 CHOICE_POS;
	static const float CHOICE_WIDTH;
	static const float CHOICE_HEIGHT;
	static const float CHOICE_SPACE;
	static const float CHOICED_ALPHA;

	static const D3DXVECTOR2 BAR_SIZE;
	static const float BAR_WIDTHPOS;
private:
	void InitChoices();							// 選択肢の初期化
	void UpdateChoices();						// 選択状態の更新

	GameObject* m_pBG;					// 背景
	GameObject* m_pStartChoice;		// 選択肢: 開始する
	GameObject* m_pSettingChoice;		// 選択肢: 設定
	GameObject* m_pEndChoice;			// 選択肢: 終了する
	GameObject* m_pSelectBar;			// 選択バー

	CHOICE m_choice;							// 選択状態
};

#endif // !_TITLE_H_
