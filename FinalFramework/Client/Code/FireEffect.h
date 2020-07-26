#ifndef FireEffect_h__
#define FireEffect_h__

#include "Effect.h"

BEGIN(Engine)

class CTexture;
class CRcTex;

END

class CFireEffect : public CEffect
{
protected:
	explicit CFireEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFireEffect();

public:
	HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const wstring& wstrNoiseTextureTag, const wstring& wstrAlphaTextureTag);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

	void Set_DeltaAlpha(const _float& fDeltaAlpha) { m_fDeltaAlpha = fDeltaAlpha; }
	void Set_UV(const _vec2* pUVStart, const _vec2* pUVSize) { m_vUVStart = *pUVStart; m_vUVSize = *pUVSize; }


protected:
	HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const wstring& wstrNoiseTextureTag, const wstring& wstrAlphaTextureTag);
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

protected:
	Engine::CTexture*		m_pNoiseTextureCom = nullptr;
	Engine::CTexture*		m_pMaskTextureCom = nullptr;
	Engine::CRcTex*			m_pBufferCom = nullptr;

	_vec3 m_vScrollSpeed = { 0.1f, 0.5f, 1.f };
	_vec3 m_vScales = { 1.f, 2.f, 3.f };
	_float m_fPadding = 0.2f;
	
	_vec2 m_vDistortion1 = { 0.2f, 0.3f };
	_vec2 m_vDistortion2 = { 0.1f, 0.6f };
	_vec2 m_vDistortion3 = { 0.7f, 0.9f };
	_float	m_fDistortionScale = 1.f;
	_float	m_fDistortionBias = 0.1f;

	_float	m_fAlpha = 1.f;
	_float	m_fDeltaAlpha = 0.f;

	_vec2	m_vUVStart;
	_vec2	m_vUVSize;
	

public:
	static CFireEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, 
		const wstring& wstrNoiseTextureTag, const wstring& wstrAlphaTextureTag);

protected:
	virtual void Free();
};


#endif // FireEffect_h__
