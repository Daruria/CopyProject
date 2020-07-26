#pragma once
#include "StaticObject.h"
class CBoonButton :
	public CStaticObject
{
private:
	enum STATE { ON_START, ON, OFF, END};

private:
	CBoonButton();
public:
	virtual ~CBoonButton();

public:// Inherited via CStaticObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CBoonButton* Create(const wstring& wstrStateKey, const D3DXVECTOR3& vPos, const D3DXVECTOR3& vScale, const D3DXVECTOR3& vSize);

private:
	void IsOnMouse();

	void OnUpdate();
	void ChangeState();

private:
	wstring m_wstrStateKey;
	D3DXVECTOR3 m_vSize;
	bool m_bIsOn;
	FRAME m_tOnFrame;

	CBoonButton::STATE m_ePreState;
	CBoonButton::STATE m_eCurState;

	CKeyMgr* m_pKeyMgr;
};

