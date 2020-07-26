#ifndef CircularEffect_h__
#define CircularEffect_h__

#include "Effect.h"

BEGIN(Engine)

class CCircularTrail;
class CTexture;

END

class CCircularEffect : public CEffect
{
private:
	explicit CCircularEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCircularEffect();

public:
	HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

	void Set_Angle(const _float& fRadian);
	void Set_Speed(const _float& fSpeed) { m_fSpeed = fSpeed; }

protected:
	HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor);
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	Engine::CCircularTrail*		m_pBufferCom = nullptr;
	Engine::CTexture*			m_pColorTextureCom = nullptr;

	_float m_fSpeed = 1.f;
	_float	m_fAlpha = 1.f;

public:
	static CCircularEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, 
		const D3DXCOLOR& dwColor);

private:
	virtual void Free();
};


#endif // CircularEffect_h__
