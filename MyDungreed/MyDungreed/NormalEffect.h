#pragma once
#include "Effect.h"
class CNormalEffect :
	public CEffect
{
public:
	CNormalEffect();
	virtual ~CNormalEffect();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	float		m_fAngle;
	float		m_fSpeed;
	int			m_iFrame;
	int			m_iMaxFrame;
	DWORD		m_dwFrameTime;
	DWORD		m_dwFrameMaxTime;
	string		m_strKey;

public:
	void SetAngle(float fAngle) { m_fAngle = fAngle; }
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }
	void SetMaxFrame(int iMaxFrame) { m_iMaxFrame = iMaxFrame; }
	void SetKey(const string& strKey) { m_strKey = strKey; }
	void SetFrameMaxTime(const DWORD dwFrameMaxTime) { m_dwFrameMaxTime = dwFrameMaxTime; }

public:
	const int	GetFrame()		const { return m_iFrame; }
	const int	GetMaxFrame()	const { return m_iMaxFrame; }
	const string GetKey()		const { return m_strKey; }

};

