#pragma once
#include "MyButton.h"
class CAbilityPerk :
	public CMyButton
{
public:
	enum PERK_LEVEL { FIVE, TEN, TWENTY, END };

public:
	CAbilityPerk();
	virtual ~CAbilityPerk();

public:
	virtual void Initialize(const string& strKey, const wchar_t* pPath, const string& strOnKey, const wchar_t* pOnPath);
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

public:
	void SetPerkLevel(PERK_LEVEL eLevel) { m_eLevel = eLevel; }
	void AddDescription(const string& strText);
	void SetActive(bool bIsActive)		{ m_bIsActive = bIsActive; }

private:
	void DescriptionRender(HDC hDC);

private:
	bool		m_bIsActive;
	PERK_LEVEL	m_eLevel;
	list<string> m_DescList;

};

