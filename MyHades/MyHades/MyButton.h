#pragma once
#include "StaticObject.h"
class CMyButton :
	public CStaticObject
{
public:
	enum BUTTON_TYPE {IMAGE_ONLY, FONT_ONLY, IMAGE_AND_FONT, BUTTON_END};

public:
	CMyButton();
	virtual ~CMyButton();

public:	// Inherited via CGameObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CMyButton* Create(BUTTON_TYPE eType, const wstring& wstrObjectKey, float fx, float fy, const wstring& wstrFont = L"", float fScale = 1.f);

protected:
	bool IsOn();
	
private:
	wstring GetState();

private:
	BUTTON_TYPE m_eType;

	bool m_bIsOn;

	wstring m_wstrObjectKey;
	wstring m_wstrFont;
	wstring m_wstrStateKey;

	SIZE m_tSize;
};