#pragma once
#include "StaticObject.h"

class CAnimatedObject;
class CBoonOrb :
	public CStaticObject
{
private:
	CBoonOrb();
public:
	virtual ~CBoonOrb();

public:// Inherited via CStaticObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CBoonOrb* Create(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vScale);

private:
	CAnimatedObject* m_pBoonFlare;
	CAnimatedObject* m_pOrbBack;
	CAnimatedObject* m_pOrbFront;

	CKeyMgr* m_pKeyMgr;
};

