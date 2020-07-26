#pragma once
#include "Obj.h"

enum RENDER_TYPE {RT_WORLD, RT_SCREEN, RT_END};
class CAnimatedBGObject :
	public CObj
{
public:
	CAnimatedBGObject();
	virtual ~CAnimatedBGObject();

public:
	// Inherited via CObj
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

protected:
	float			m_fRatio;
	int				m_iFrame;
	int				m_iStartFrame;
	int				m_iMaxFrame;
	int				m_iTotalFrame;
	DWORD			m_dwFrameTime;
	DWORD			m_dwTimePerFrame;
	string			m_strKey;
	ANIMATION_TYPE	m_eAnimationType;
	RENDER_TYPE		m_eRenderType;

public:
	const int GetCurFrame() const { return m_iFrame; }
	
public:
	void SetImage(const string& strKey, const wchar_t* pPath);
	void SetFrame(int iFrame)				{ m_iFrame = iFrame; }
	void SetStartFrame(int iStartFrame)		{ m_iStartFrame = iStartFrame; }
	void SetMaxFrame(const int iMaxFrame)	{ m_iMaxFrame = iMaxFrame; }
	void SetTotalFrame(const int iTotalFrame) { m_iTotalFrame = iTotalFrame; }
	void SetRatio(const float fRatio)		{ m_fRatio = fRatio; }
	void SetTimePerFrame(const DWORD dwTimePerFrame)			{ m_dwTimePerFrame = dwTimePerFrame; }
	void SetAnimationType(const ANIMATION_TYPE eAnimationType)	{ m_eAnimationType = eAnimationType; }
	void SetRenderType(const RENDER_TYPE eRenderType)			{ m_eRenderType = eRenderType; }
};

