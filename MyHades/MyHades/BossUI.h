#pragma once
#include "UI.h"

class CBossObserver;
class CBossUI :
	public CUI
{
	enum SHOW_STATE { COME_IN, SHOW, GO_OUT, NO_SHOW, SHOW_END };

public:
	CBossUI();
	virtual ~CBossUI();

public:
	// Inherited via CGameObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CBossUI* Create();

public:
	virtual float GetLayerStd() override;
	virtual void Die() override;

public:
	void UIComeIn();

private:
	bool ComeInHpBar();

private:
	bool	m_bIsDead;
	bool	m_bRender;
	int		m_iBossHp;
	int		m_iBossMaxHp;

	CBossUI::SHOW_STATE m_eShowState;
	float	m_fSpeed;
	D3DXVECTOR3 m_vHpBarPos;

	wstring m_wstrBossHp;

	CSubjectMgr* m_pSubjectMgr;
	CBossObserver* m_pObserver;
};