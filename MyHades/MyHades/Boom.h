#pragma once
#include "Monster.h"
class CBoom :
	public CMonster
{
private:
	enum STATE { PREVIEW, FIRE, END };

public:
	CBoom();
	virtual ~CBoom();

public:// Inherited via CGameObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CBoom* Create(const D3DXVECTOR3& vPos);

private:
	float	m_fTimeCount;
	float	m_fBoomTime;

	CBoom::STATE m_eCurState;
};

