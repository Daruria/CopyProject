#pragma once
class CGameObject
{
protected:
	CGameObject();

public:
	virtual ~CGameObject();

public:
	virtual int Update()		PURE;
	virtual void LateUpdate()	PURE;
	virtual void Render()		PURE;

protected:
	virtual HRESULT	Initialize()PURE;
	virtual void Release()		PURE;

public:
	virtual float GetLayerStd()	PURE;

public:
	void SetInfo(const INFO& tInfo)			{ m_tInfo = tInfo; }
	void SetPos(float fx, float fy)			{ m_tInfo.vPos.x = fx; m_tInfo.vPos.y = fy; }
	void SetPos(const D3DXVECTOR3& vPos)	{ m_tInfo.vPos = vPos; }
	void SetPosX(float fx)					{ m_tInfo.vPos.x = fx; }
	void SetPosY(float fy)					{ m_tInfo.vPos.y = fy; }
	void SetDir(const D3DXVECTOR3& vDir)	{ m_tInfo.vDir = vDir; }
	void SetLook(const D3DXVECTOR3& vLook)	{ m_tInfo.vLook = vLook; }
	void AddPos(float fx, float fy)			{ m_tInfo.vPos.x += fx; m_tInfo.vPos.y += fy; }
	void AddPosX(float fx)					{ m_tInfo.vPos.x = fx; }
	void AddPosY(float fy)					{ m_tInfo.vPos.y = fy; }
	void SetScale(float fScale)				{ m_tInfo.vScale = {fScale, fScale, 0.f}; }
	void SetScale(float fScaleX, float fScaleY) { m_tInfo.vScale = { fScaleX, fScaleY, 0.f }; }
	void SetRenderType(RENDER_TYPE eType)	{ m_eRenderType = eType; }

public:
	const INFO&			GetInfo()		const { return m_tInfo; }
	const RENDER_TYPE	GetRenderType() const { return m_eRenderType; }

protected:
	RENDER_TYPE	m_eRenderType;
	INFO		m_tInfo;

protected:
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;
};

