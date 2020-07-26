#pragma once
#include "GameObject.h"

class CMoveObject :
	public CGameObject
{
protected:
	CMoveObject();
public:
	virtual ~CMoveObject();

public:// Inherited via CGameObject
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

protected:
	virtual void Animation();
	virtual void KnockBack(const D3DXVECTOR3& vHitterPos);
	void KnockBackUpdate();

public:
	virtual float GetLayerStd() override { return m_vFootPos.y + m_tInfo.vPos.y; };
	virtual void Die() override {};

public:
	virtual void Hit(const D3DXVECTOR3& vPos, int iAttack);

public:
	void SetFootPos(const D3DXVECTOR3& vFootPos) { m_vFootPos = vFootPos; }
	void SetFoot(const D3DXVECTOR3& vPos) { m_tInfo.vPos = vPos - m_vFootPos; }
	void SetColl(const bool bColl) { m_bCanColl = bColl; }

public:
	const D3DXVECTOR3& GetFootPos() const { return m_vFootPos; }
	const D3DXVECTOR3  GetFoot()	const { return m_vFootPos + m_tInfo.vPos; }
	const RECT GetRect() const;
	const bool GetColl() const { return m_bCanColl; }

protected:
	bool		m_bOnceAnimFinish;
	FRAME		m_tFrame;
	HIT_BOX		m_tHitBox;
	D3DXVECTOR3 m_vFootPos;
	bool		m_bCanColl;

	bool	m_bKnockBack;
	float	m_fKnockBackPower;
	D3DXVECTOR3 m_vKnockBackDir;
	
protected:
	CObjectMgr*		m_pObjectMgr;
	CSubjectMgr*	m_pSubjectMgr;
};

