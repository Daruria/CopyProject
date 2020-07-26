#pragma once
#include "UIWindow.h"
class CMapRoom;
class CMapWindow :
	public CUIWindow
{
public:
	enum ROOM_POS { UP, DOWN, LEFT, RIGHT, END };

public:
	CMapWindow();
	virtual ~CMapWindow();

public:
	virtual void Initialize();
	virtual void Initialize(const string& strKey, const wchar_t* pPath);
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

public:
	void AddRoom(CMapRoom* m_pRoom, ROOM_POS ePos);
	void SetCurStage(CMapRoom* pRoom) { m_pCurRoom = pRoom; }
	void SetPrevFastMove(bool bPrevFastMove) { m_bPrevFastMove = bPrevFastMove; }
	void SetbFastMove(bool bFastMove) { m_bFastMove = bFastMove; }

public:
	const SCENE_CHANGE GetCurStage() const;
	const CMapRoom*		FindRoom(SCENE_CHANGE eStage) const;
	const float GetScrollX()		const { return m_fScrollX; }
	const float GetScrollY()		const { return m_fScrollY; }
	const bool GetPrevFastMove()	const { return m_bPrevFastMove; }
	const POSITION& GetFastMoveDest() const { return m_tFastMoveDest; }

private:
	void Scrolling();

private:
	bool			m_bIsOnIcon;
	INFO			m_tOnInfo;

	bool			m_bIsScrolling;
	float			m_fScrollX;
	float			m_fScrollY;
	POSITION		m_tClickedPos;
	POSITION		m_tScrollWhenClick;

	CMapRoom*		m_pCurRoom;
	list<CMapRoom*> m_RoomList;


	bool			m_bFastMove;
	bool			m_bPrevFastMove;
	POSITION		m_tFastMoveDest;

	CMyButton*		m_pExitButton;
};

