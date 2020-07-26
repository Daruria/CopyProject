#pragma once
#include "Obj.h"
class CBGImage :
	public CObj
{
public:
	CBGImage();
	virtual ~CBGImage();

private:
	float	m_fRatio;
	int		m_iFrame;
	int		m_iMaxFrame;
	DWORD	m_dwFrameTime;
	DWORD	m_dwFrameMaxTime;
	string  m_strKey;

public:
	void SetRatio(float fRatio) { m_fRatio = fRatio; }
	void SetMaxFrame(int iMaxFrame) { m_iMaxFrame = iMaxFrame; }
	void SetImage(const string& strKey, const wchar_t* pPath);

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void RenderWithoutScroll(HDC hDC);
};

