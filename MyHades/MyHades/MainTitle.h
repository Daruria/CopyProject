#pragma once
#include "Scene.h"

class CAnimatedObject;
class CNonAnimatedObject;
class CMyButton;
class CMainTitle :
	public CScene
{
public:
	CMainTitle();
	virtual ~CMainTitle();

public:// Inherited via CScene
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

	MACRO_CREATE(CMainTitle)

private:
	bool	m_bLogoEnd;

	CAnimatedObject*	m_pMainBG;
	CNonAnimatedObject* m_pTitle;

	CMyButton*	m_pPlayButton;
	CMyButton*	m_pEndButton;

};

