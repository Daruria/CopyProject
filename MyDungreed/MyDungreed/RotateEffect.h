#pragma once

#include "Effect.h"
class CRotateEffect : public CEffect
{
public:
	CRotateEffect();
	virtual ~CRotateEffect();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	int				m_iFrame;
	int				m_iMaxFrame;
	DWORD			m_dwFrameTime;
	float			m_fAngle;
	string			m_strKey;
	vector<string>	m_vecFrameKey;

public:
	void SetMaxFrame(int iMaxFrame)		{ m_iMaxFrame = iMaxFrame; }
	void SetKey(const string& strKey)	{ m_strKey = strKey; }
	void AddFrameKey(const string& strKey) { m_vecFrameKey.push_back(strKey); }
	void SetAngle(const float fAngle)	{ m_fAngle = fAngle; }

public:
	const int	GetFrame()		const { return m_iFrame; }
	const int	GetMaxFrame()	const { return m_iMaxFrame; }
	const string GetKey()		const { return m_strKey; }


};

