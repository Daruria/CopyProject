#ifndef SparkEffect_h__
#define SparkEffect_h__

#include "Effect.h"

BEGIN(Engine)

class CTexture;
class CRcTex;

END

class CSparkEffect : public CEffect
{
private:
	explicit CSparkEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSparkEffect();

public:
	HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor, const _vec2* pStartUV, const _vec2* pUVSize);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

	virtual void Set_Scale(const _float& fScale);
	void Set_Speed(const _float& fSpeed) { m_fSpeed = fSpeed; }
	void Set_Alpha(const _float& fAlpha) { m_fAlpha = fAlpha; }

protected:
	HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor);
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	Engine::CRcTex*		m_pBufferCom = nullptr;
	Engine::CTexture*	m_pColorTextureCom = nullptr;

	_float	m_fSpeed = 1.f;
	_float	m_fAlpha = 1.f;

	_vec2	m_vStartUV	= {};
	_vec2	m_vUVSize	= {};

	_vec3	m_vScale = {};
	_float	m_fScale = 0.f;
	_bool	m_bXScale = false;

public:
	static CSparkEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, 
		const D3DXCOLOR& dwColor, const _vec2* pStartUV, const _vec2* pUVSize);

private:
	virtual void Free();
};


#endif // SparkEffect_h__
