#pragma once
#include "UI.h"
class CPlayerObserver;
class CPlayerUI :
	public CUI
{
private:
	enum WRATH_STATE { NO_PIP, FULL_START, FULL_PIP, WRATH_END };
	enum SHOW_STATE { COME_IN, SHOW, GO_OUT, NO_SHOW, SHOW_END };

protected:
	CPlayerUI();
public:
	virtual ~CPlayerUI();

public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CPlayerUI* Create();

public:
	void UIComeIn();
	void UIGoOut();

private:
	bool ComeInHpBar();
	bool ComeInWrathBar();
	bool ComeInBoolSlotBar();

private:
	bool GoOutHpBar();
	bool GoOutWrathBar();
	bool GoOutBoolSlotBar();

private:
	void Animation();

private:
	int		m_iPlayerHp;
	int		m_iPlayerMaxHp;
	int		m_iWrathPoint;
	int		m_iWrathMaxPoint;

	CPlayerUI::WRATH_STATE m_eWrathState;
	FRAME	m_tWrathFrame;
	bool	m_bOnceAnimFinish;

	CPlayerUI::SHOW_STATE m_eShowState;
	float	m_fSpeed;

	bool	m_bCanReflectSkill;
	bool	m_bCanDashSkill;
	bool	m_bCanCall;

	D3DXVECTOR3 m_vHpBarPos;
	D3DXVECTOR3 m_vWrathBarPos;
	D3DXVECTOR3 m_vBoonSlotPos;

	wstring m_wstrPlayerHp;
	
	CSubjectMgr* m_pSubjectMgr;
	CPlayerObserver* m_pObserver;
	CFontMgr*	m_pFontMgr;
};

