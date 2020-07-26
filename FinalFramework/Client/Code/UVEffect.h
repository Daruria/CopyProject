#ifndef UVEffect_h__
#define UVEffect_h__

#include "Effect.h"

BEGIN(Engine)

class CRcTex;
class CTexture;

END

class CUVEffect : public CEffect
{
private:
	explicit CUVEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUVEffect();

public:
	HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

	void Set_Speed(const _float& fSpeed) { m_fSpeed = fSpeed; }
	void Set_UV(const _vec2* pStartUV, const _vec2* pUVSize) { m_vStartUV = *pStartUV, m_vUVSize = *pUVSize; }
	void Set_DeltaAlpha(const _float& fDeltaAlpha) { m_fDeltaAlpha = fDeltaAlpha; }
	void On_BillBoard() { m_bBillBoard = true; }
	void Set_RenderID_Pass(const _ushort& wRenderID, const _int& iRenderPass);

protected:
	HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor);
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	Engine::CRcTex*		m_pBufferCom = nullptr;
	Engine::CTexture*	m_pColorTextureCom = nullptr;

	_float	m_fSpeed = 1.f;
	_float	m_fAlpha = 1.f;
	_float	m_fDeltaAlpha = 0.f;

	_vec2 m_vStartUV = {};
	_vec2 m_vUVSize = { 1.f, 1.f };

	_bool	m_bBillBoard = false;

	_ushort		m_wRenderID = Engine::RENDER_ALPHA;
	_int 		m_iRenderPass = 0;

public:
	static CUVEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, 
		const D3DXCOLOR& dwColor);

private:
	virtual void Free();
};


#endif // UVEffect_h__
