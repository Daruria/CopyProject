#pragma once
class CUI
{
public:
	CUI();
	~CUI();

protected:
	POINT	m_tPoint;
	SIZE	m_tSize;
	string	m_strKey;

public:
	void SetPoint(LONG lx, LONG ly)		{ m_tPoint.x = lx; m_tPoint.y = ly; }
	void SetPoint(const POINT& pt)		{ m_tPoint = pt; }
	void SetSize(LONG lcx, LONG lcy)	{ m_tSize.cx = lcx; m_tSize.cy = lcy; }
	void SetSize(const SIZE& tSize)		{ m_tSize = tSize; }
	void SetKey(const string& strKey)	{	m_strKey = strKey; }

public:
	const POINT&	GetPoint()	const { return m_tPoint; }
	const SIZE&		GetSize()	const { return m_tSize; }
	const string&	GetKey()	const { return m_strKey; }

public:
	void AddPoint(const LONG ldx, const LONG ldy) { m_tPoint.x += ldx; m_tPoint.y += ldy; }

public:
	virtual void Initialize(const string& strKey, const wchar_t* pPath);
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();
};

