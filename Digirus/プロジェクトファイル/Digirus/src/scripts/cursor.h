//=============================================================
//
// カーソル [cursor.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _CURSOR_H_
#define _CURSOR_H_

#include "gameobject.h"

class CCursor
{
public:
	// カーソルデバイス
	enum CursorDevice
	{
		MOUSE,
		CONTROLLER,
	};

	void Init();
	void Uninit();
	void Update();
	void SetVisible(const bool& bVisible);
	void SetPriority(const int& nPriority);
	POINT GetPos() { return m_cursorPoint; }
	CursorDevice GetCursorDevice() { return m_cursorDevice; }

	static const int CURSOR_SIZE;			// カーソルサイズ
private:
	CursorDevice m_cursorDevice;
	POINT m_cursorPoint;
	GameObject* m_pCursorObj;
};

#endif // !_CURSOR_H_
