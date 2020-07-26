#pragma once
#include "UIWindow.h"
class CBGImage;
class CMyButton;
class CAbilityPerk;
class CAbilityWindow :
	public CUIWindow
{
public:
	CAbilityWindow();
	virtual ~CAbilityWindow();

public:
	virtual void Initialize();
	virtual void Initialize(const string& strKey, const wchar_t* pPath);
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

public:
	virtual void WindowOn();
	virtual void WindowOff();

private:
	void ImageListSetting();
	void ButtonSetting();
	void PerkSetting();
	void AddBGImage(const string& strKey, const wchar_t* pPath, float fx, float fy, int icx, int icy, float fRatio);
	void ButtonsUpdate();
	void PerkUpdate();

private:
	void PerkActiveUpdate(int iPerk, int iStartPerk);
	void AbilityTextRender(HDC hDC);
	void RemainPointRender(HDC hDC);

private:
	list<CBGImage*>		m_BGList;
	CMyButton*			m_pWrathButton;
	CMyButton*			m_pSwiftnessButton;
	CMyButton*			m_pPatienceButton;
	CMyButton*			m_pMysticButton;
	CMyButton*			m_pGreedButton;
	vector<CAbilityPerk*> m_vecPerk;

private:
	int		m_iAbilityPoint;

};

