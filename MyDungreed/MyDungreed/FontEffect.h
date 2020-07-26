#pragma once
#include "Effect.h"
class CFontEffect :
	public CEffect
{
public:
	CFontEffect();
	virtual ~CFontEffect();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	float		m_fRatio;
	float		m_fDist;
	float		m_fSpeed;
	string		m_strKey;
	string		m_strSentence;
	POSITION	m_tStartPos;
	POSITION	m_tEndPos;
	POSITION	m_tDir;
	FONT_TYPE	m_eType;

public:
	void SetRatio(const float fRatio)		{ m_fRatio = fRatio; }
	void SetSpeed(const float fSpeed)		{ m_fSpeed = fSpeed; }
	void SetImage(const string& strKey, const wchar_t* pPath);
	void SetSentence(const string& strSentence) { m_strSentence = strSentence; }
	void SetStartPos(const POSITION& tPos)	{ m_tStartPos = tPos; }
	void SetStartPos(const INFO& tInfo)		{ m_tStartPos.fx = tInfo.fx; m_tStartPos.fy = tInfo.fy; }
	void SetEndPos(const POSITION& tPos)	{	m_tEndPos = tPos; }
	void SetEndPos(const float fx, const float fy) { m_tEndPos.fx = fx; m_tEndPos.fy = fy; }
	void SetFontType(FONT_TYPE eType)		{ m_eType = eType; }

private:
	void NumberRender(HDC hDC, const POSITION& tPos, const SIZE& tSize, int iNumber);

public:
	void MoveDirSetting(const POSITION& tStartPos, const POSITION& tEndPos);

};

