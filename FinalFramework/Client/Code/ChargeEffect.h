#ifndef ChargeEffect_h__
#define ChargeEffect_h__

#include "Effect.h"

class CChargeEffect : public CEffect
{
private:
	explicit CChargeEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CChargeEffect();

public:
	HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

	void Set_Speed(const _float& fSpeed) { m_fSpeed = fSpeed; }

protected:
	HRESULT Add_Component();

private:
	_ushort		m_wContainerIdx = 0;
	wstring		m_wstrTextureTag = L"";
	D3DXCOLOR m_dwColor = { 1.f,1.f,1.f,1.f };

	_float	m_fSpeed = 1.f;
	_float	m_fAlpha = 1.f;

	_float	m_fItvlTime = 0.f;

public:
	static CChargeEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, 
		const D3DXCOLOR& dwColor);

private:
	virtual void Free();
};


#endif // ChargeEffect_h__
