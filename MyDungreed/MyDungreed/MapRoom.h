#pragma once
#include "Obj.h"

const unsigned short Flag_Link_Up = 0x01;		//	0000 0001
const unsigned short Flag_Link_Down = 0x02;		//	0000 0010
const unsigned short Flag_Link_Left = 0x04;		//	0000 0100
const unsigned short Flag_Link_Right = 0x08;	//	0000 1000

class CWormIcon;
class CWormPassage;
class CMapRoom :
	public CObj
{
public:
	CMapRoom();
	virtual ~CMapRoom();

public:
	// Inherited via CObj
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void Render(HDC hDC, float fScrollX, float fScrollY);
	void AddWormIcon();

public:
	void SetRatio(float fRatio)					{ m_fRatio = fRatio; }
	void SetStage(SCENE_CHANGE eStage)			{ m_eStage = eStage; }
	void SetWormIcon(CWormIcon* pWormIcon)		{ m_pWormIcon = pWormIcon; }
	void SetWormPassage(CWormPassage* pPassage) { m_pPassage = pPassage; }
	void SetLink(unsigned short FlagLink)		{ m_usFlagLink = FlagLink; }
	void SetPassagePos(float fx, float fy)		{ m_tPassagePos = { fx, fy }; }

public:
	const SCENE_CHANGE	GetStage()		const { return m_eStage; }
	const float			GetRatio()		const { return m_fRatio; }
	const bool			GetWormIconOn() const { return m_bIsWormIconOn; }
	const CWormIcon*	GetWormIcon()	const { return m_pWormIcon; }
	const CWormPassage* GetWormPassage()const { return m_pPassage; }
	const unsigned short GetLink()		const { return  m_usFlagLink; }
	const POSITION&		GetPassagePos() const { return m_tPassagePos; }

private:
	void LinkRender(HDC hDC);
	void LinkRender(HDC hDC, float fScrollX, float fScrollY);

private:
	float			m_fRatio;

	unsigned short	m_usFlagLink;

	bool			m_bIsCurRoom;
	bool			m_bIsWormIconOn;
	SCENE_CHANGE	m_eStage;
	CWormIcon*		m_pWormIcon;
	CWormPassage*	m_pPassage;

	bool			m_bBlinking;
	DWORD			m_dwBlinkingTime;
	DWORD			m_dwBlinkingInterval;

	POSITION		m_tPassagePos;
};

