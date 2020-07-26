#pragma once
#include "Scene.h"

class CBGObject;
class CBGImage;
class CMiniMap;
class CMyMiniMap;
class CAnimatedBGObject;
class CTown :
	public CScene
{
public:
	CTown();
	virtual ~CTown();

private:
	CBGObject*	m_pBGSky;
	CBGObject*	m_pBGMountain;
	CBGObject*	m_pBGTrees;

	CBGImage*	m_pSecondFloor1;
	CBGImage*	m_pSecondFloor2;
	CBGImage*	m_pSecondFloor3;

	CBGImage*	m_pBrokenHouse;
	CBGImage*	m_pBrokenTemple;

	CBGImage*	m_pSkulSignL;
	CBGImage*	m_pSkulSignR;

	CBGImage*	m_pMiddleFloor1;
	CBGImage*	m_pMiddleFloor2;
	CBGImage*	m_pMiddleFloor3;
	CBGImage*	m_pMiddleFloor4;
	CBGImage*	m_pMiddleFloor5;
	CBGImage*	m_pMiddleFloor6;

	CMiniMap*	m_pMiniMap;

	CAnimatedBGObject* m_pEatDungeon;

	DWORD	m_dwReturnToTown;

private:
	bool		m_bDunEatSound;
	POSITION	m_tCenterPos;

private:
	void EatDungeonInit();

public:
	virtual void Initialize() override;
	virtual int Update()override;
	virtual void LateUpdate()override;
	virtual void Render(HDC hDC)override;
	virtual void Release()override;

public:
	void ImageInitialize();
	void PlayerSetting();
};