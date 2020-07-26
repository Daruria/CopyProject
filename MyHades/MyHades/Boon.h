#pragma once
#include "StaticObject.h"
class CBoonButton;
class CBoon :
	public CStaticObject
{
private:
	enum STATE {INTRO, SELECT, OUTRO, END};

private:
	CBoon();
public:
	virtual ~CBoon();

public:// Inherited via CStaticObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CBoon* Create(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vScale, float fSort);

public:
	virtual void Die() override { };

public:
	virtual float GetLayerStd() override { return m_fSort; }

private:
	void StateUpdate();
	void IntroUpdate();
	void SelectUpdate();
	void OutroUpdate();
	void Animation();

private:
	CBoon::STATE m_eCurState;

	bool	m_bIsDead;

	bool	m_bOnceAnimFinish;
	FRAME	m_tFrame;
	wstring m_wstrStateKey;

	float	m_fSort;

	bool	m_bSelected;
	float	m_fSelectEffectWaiting;

	CBoonButton* m_pBoonButton1;
	CBoonButton* m_pBoonButton2;
	CBoonButton* m_pBoonButton3;
};

