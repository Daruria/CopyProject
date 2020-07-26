#pragma once
#include "Obj.h"
class CBGObject :
	public CObj
{
public:
	CBGObject();
	virtual ~CBGObject();

private:
	string  m_strKey;
	int		m_iInterval;

public:
	void SetImage(const string& strKey, const wchar_t* pPath);
	void SetInterval(const int iInterval) { m_iInterval = iInterval; }

public:
	const int GetInterval() { return m_iInterval; }

private:
	const POSITION GetScreenFromScroll();

public:
	virtual void Initialize();
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();
};

