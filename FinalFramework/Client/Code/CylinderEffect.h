#ifndef CylinderEffect_h__
#define CylinderEffect_h__

#include "Effect.h"

BEGIN(Engine)

class CCylinderTex;
class CTexture;

END

class CCylinderEffect : public CEffect
{
private:
	explicit CCylinderEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCylinderEffect();

public:
	virtual HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag,
		const _ulong& dwSlices, const _ulong& dwStacks, const _float& fUpperRadius, const _float& fLowerRadius, const _float& fHeight, 
		const _ushort& wRenderID = Engine::RENDER_ALPHA, const _uint& iRenderPass = 0);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

public:
	void	Set_Speed(const _float& fSpeed) { m_fSpeed = fSpeed; }
	void	Set_DirUV(const _vec2* pDirUV) { memcpy(&m_vDirUV, pDirUV, sizeof(_vec2)); }
	void	Set_DeltaAlpha(const _float& fDeltaAlpha) { m_fDeltaAlpha = fDeltaAlpha; }
	
protected:
	HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag,
		const _ulong& dwSlices, const _ulong& dwStacks, const _float& fUpperRadius, const _float& fLowerRadius, const _float& fHeight);
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	Engine::CCylinderTex*		m_pBufferCom = nullptr;
	Engine::CTexture*			m_pColorTextureCom = nullptr;

	_float	m_fSpeed = 1.f;
	_float	m_fAlpha = 1.f;
	_float	m_fDeltaAlpha = 0.f;
	_vec2	m_vDirUV = { 0.f, 0.f };
	_ushort m_wRenderID = Engine::RENDER_ALPHA;
	_uint	m_iRenderPass = 0;

public:
	static CCylinderEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag,
		const _ulong& dwSlices, const _ulong& dwStacks, const _float& fUpperRadius, const _float& fLowerRadius, const _float& fHeight);

	static CCylinderEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag,
		const _ulong& dwSlices, const _ulong& dwStacks, const _float& fUpperRadius, const _float& fLowerRadius, const _float& fHeight, 
		const _ushort& wRenderID, const _uint& iRenderPass);

private:
	virtual void Free();
};


#endif // CylinderEffect_h__
