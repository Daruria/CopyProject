#pragma once
#include "UI.h"

enum STAGE_TITLE_PHASE { STP_COME_IN, STP_STOP, STP_GO_OUT, STP_END,};

class CObj;
class CInvenSlot;
class CUISlot;
class CUIWindow;
class CPlayerUI :
	public CUI
{
public:
	CPlayerUI();
	virtual ~CPlayerUI();

	DECLARE_SINGLE(CPlayerUI)

public:
	virtual void Initialize(const string& strKey, const wchar_t* pPath) override;
	virtual int Update()override;
	virtual void LateUpdate()override;
	virtual void Render(HDC hDC)override;
	virtual void Release()override;

public:
	void Initialize();

private:
	int		m_iLevel;
	int		m_iHp;
	int		m_iMaxHp;
	int		m_iDashCount;
	int		m_iMaxDash;

private:
	bool	m_bLifeWaveOnOff;
	int		m_iLifeWaveFrame;
	int		m_iLifeWaveMaxFrame;
	DWORD	m_dwLifeWaveFrameTime;

private:
	bool		m_bSwapAnimation;	//	True : Swap Possible,  False : Swap Impossible
	bool		m_bSwapCue;			//	True : Swap Start, False : Swap End
	bool		m_bWeapon1Front;	//	True : Weapon1 Front, False : Weapon1 Back
	DWORD		m_dwSwapTime;
	DWORD		m_dwSwapStartTime;
	POINT		m_tFrontPos;
	POINT		m_tBackPos;
	CUISlot*	m_pWeapon1;
	CUISlot*	m_pWeapon2;

private:
	bool				m_bStageTitle;
	float				m_fStageTitleSpeed;
	STAGE_TITLE_PHASE	m_ePhase;
	DWORD				m_dwStageTitleStopTime;
	INFO				m_tStageTitleInfo;
	string				m_strStageTitle;

private:
	list<CUIWindow*> m_WindowList;
	
public:
	const bool IsPossibleToSwap()	const	{ return m_bSwapAnimation; };
	const bool IsThereOpenWindow()	const;
	list<CUIWindow*>* GetWindowList()		{ return &m_WindowList; }

public:
	void ValueUpdate(CObj* pObj);
	void StartSwap();
	void AddWindow(CUIWindow* pWindow);
	void StageTitleComeIn(const string& strTitle);
	void WeaponSlotReset();
	CUIWindow* FindUIWindow(const string& strKey);
	void WindowListClear();

private:
	void EquipSlotUIUpdate();
	void LifeWaveUpdate(DWORD dwCurTime);
	void WeaponSwapAnimate(DWORD dwCurTime);
	int WindowListUpdate();
	void StageTitleUpdate();

private:
	void PlayerHpBarRender(HDC hDC);
	void PlayerDashBarRender(HDC hDC);
	void PlayerGoldRender(HDC hDC);
	void EquipSlotUIRender(HDC hDC);
	void SkillSlotUIRender(HDC hDC);
	void StageTitleRender(HDC hDC);
};

