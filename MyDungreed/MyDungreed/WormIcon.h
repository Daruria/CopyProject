#pragma once
#include "MyButton.h"
class CWormIcon :
	public CMyButton
{

public:
	CWormIcon();
	virtual ~CWormIcon();

public:
	virtual void Initialize(const string& strKey, const wchar_t* pPath, const string& strOnKey, const wchar_t* pOnPath);
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render(HDC hDC);
	virtual void Release();

public:
	void Render(HDC hDC, float fx, float fy);

public:
	void SetCurStage(SCENE_CHANGE eStage) { m_eCurStage = eStage; }

public:
	const SCENE_CHANGE GetCurStage() const { return m_eCurStage; }

public:
	const BOOL IsOnMouseScroll();

private:
	SCENE_CHANGE m_eCurStage;


};

