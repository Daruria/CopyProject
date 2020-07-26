#ifndef SphereEffect_h__
#define SphereEffect_h__

#include "Effect.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CStaticMesh;

END

class CSphereEffect : public CEffect
{
private:
	explicit CSphereEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSphereEffect();

public:
	HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR& dwColor,
		const _ushort& wRenderID = Engine::RENDER_ALPHA, const _uint& iRenderPass = 3) ;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

public:
	void Set_DeltaDifferScale(const _float& fDeltaDifferScale) { m_fDeltaDifferScale = fDeltaDifferScale; }
	void Set_Alpha(const _float& fAlpha) { m_fAlpha = fAlpha; }
	void Set_DeltaAlpha(const _float& fDeltaAlpha) { m_fDeltaAlpha = fDeltaAlpha; };

protected:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR& dwColor) ;
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect) override;

private:
	Engine::CTexture*	m_pColorTextureCom = nullptr;

	//Engine::CTexture*		m_pNoiseTextureCom = nullptr;
	//Engine::CTexture*		m_pMaskTextureCom = nullptr;
	//Engine::CRcTex*		m_pBufferCom = nullptr;
	Engine::CStaticMesh*		m_pBufferCom = nullptr;

	_float				m_fAlpha = 1.f;
	_float				m_fDeltaAlpha = 0.f;
	_float				m_fDeltaDifferScale = 0.f;

	_ushort				m_wRenderID = Engine::RENDER_ALPHA;
	_uint				m_iRenderPass = 3;

public:
	static CSphereEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR& dwColor);
	static CSphereEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR& dwColor, 
		const _ushort& wRenderID, const _uint& iRenderPass);

private:
	virtual void Free() override;
};


#endif // SphereEffect_h__
