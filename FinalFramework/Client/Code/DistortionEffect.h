#ifndef DistortionEffect_h__
#define DistortionEffect_h__

#include "Effect.h"

BEGIN(Engine)

class CRcTex;
class CTexture;

END

class CDistortionEffect : public CEffect
{
private:
	explicit CDistortionEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDistortionEffect();

public:
	HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

	void Set_Speed(const _float& fSpeed) { m_fSpeed = fSpeed; }
	void Set_UV(const _vec2* pStartUV, const _vec2* pUVSize) { m_vStartUV = *pStartUV, m_vUVSize = *pUVSize; }
	void Set_DeltaAlpha(const _float& fDeltaAlpha) { m_fDeltaAlpha = fDeltaAlpha; }

protected:
	HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag);
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	Engine::CRcTex*		m_pBufferCom = nullptr;
	

	_float	m_fSpeed = 1.f;
	_float	m_fAlpha = 1.f;
	_float	m_fDeltaAlpha = 0.f;

	_vec2 m_vStartUV = {};
	_vec2 m_vUVSize = { 1.f, 1.f };

public:
	static CDistortionEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag);

private:
	virtual void Free();
};


#endif // DistortionEffect_h__
