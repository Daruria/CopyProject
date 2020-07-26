#pragma once
#include "UIWindow.h"

class CSelectBox;
class CConversation :
	public CUIWindow
{
public:
	CConversation();
	virtual ~CConversation();

private:
	enum TALK_PHASE { IS_TALKING, WAITING, END };

public:
	virtual void Initialize();
	virtual void Initialize(const string& strKey, const wchar_t* pPath);
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

public:
	void	AddTextLine(const string& strText);
	void	AddSelectBox(CSelectBox* pSelectBox);


public:
	bool		m_bTalkingEnd;
	int			m_iTalkWord;
	int			m_iTextLine;
	DWORD		m_dwTalkingTime;
	DWORD		m_dwTalkingSpeed;

	TALK_PHASE m_ePreState;
	TALK_PHASE m_eCurState;

	INFO				m_tSelectBoxInfo;

	vector<string>		m_vecText;
	list<CSelectBox*>	m_SelectList;


};

