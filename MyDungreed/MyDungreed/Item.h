#pragma once
#include "Obj.h"

class CItem :
	public CObj
{
public:
	CItem();
	virtual ~CItem();

protected:
	int			m_iPrice;
	float		m_fRatio;
	string		m_strIconKey;
	string		m_strName;
	string		m_strDisc;
	ITEM_TYPE	m_eType;

public:
	void SetRatio(float fRatio)				{ m_fRatio = fRatio; }
	void SetIconKey(const string& strKey)	{ m_strIconKey = strKey; }
	void SetName(const string& strName)		{ m_strName = strName; }
	void SetType(const ITEM_TYPE eType)		{ m_eType = eType; }
	void SetIconImage(const string& strKey, const wchar_t* pPath);
	void SetPrice(const int iPrice)			{ m_iPrice = iPrice; }

public:
	const float GetRatio() const	{ return m_fRatio; }
	const string GetIconKey() const	{ return m_strIconKey; }
	const string GetName()	const	{ return m_strName; }
	const ITEM_TYPE GetType() const	{ return m_eType; }
	const int GetPrice() const		{ return m_iPrice; }

public:
	virtual void Initialize() = 0;
	virtual int Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Render(HDC hDC) = 0;
	virtual void Release() = 0;

public:
	void IconRender(HDC hDC, float fx, float fy);
	virtual void DiscRender(HDC hDC, float fx, float fy);
};

