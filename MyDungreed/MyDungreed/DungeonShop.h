#pragma once
#include "Scene.h"
class CBGImage;
class CMiniMap;
class CWormPassage;
class CDungeonShop :
	public CScene
{
public:
	CDungeonShop();
	virtual ~CDungeonShop();

public:
	// Inherited via CScene
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	CBGImage* m_pDisplay;
	CMiniMap* m_pMiniMap;

	CWormPassage*	m_pWormPassage;

	bool	m_bFastMove;
	int		m_iFastMoveDest;

	DWORD	m_dwFastMoveDelay;
	DWORD	m_dwFastMoveDelayMax;

};

