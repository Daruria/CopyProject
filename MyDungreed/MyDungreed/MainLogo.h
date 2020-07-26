#pragma once
#include "Scene.h"
class CMyButton;
class CBGObject;
class CLogoObject;
class CMainLogo :
	public CScene
{
public:
	CMainLogo();
	virtual ~CMainLogo();

private:
	CMyButton* m_pPlayButton;
	CMyButton* m_pOptionButton;
	CMyButton* m_pExitButton;

#ifdef _DEBUG
	CMyButton* m_pEditButton;
#endif // _DEBUG

private:
	CBGObject* m_pCloud1;
	CBGObject* m_pCloud2;

private:
	list<CLogoObject*>	m_pBirdList;
	int					m_iBirdCount;
	CLogoObject*		m_pCloud;

private:
	void AddBird(float fx, float fy,float fSpeed);
	void AddVBird(float fx, float fy, float fSpeed);
	void AddCloud(float fx, float fy, float fSpeed);

private:
	void BirdUpdate();

public:
	virtual void Initialize();
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();
};

