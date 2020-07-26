#ifndef DamageEffect_h__
#define DamageEffect_h__

#include "Effect.h"

BEGIN(Engine)

class CRcTex;
class CTexture;

END

class CDamageEffect : public CEffect
{
	enum class EDamageState { Scaling, ScaleStop };

private:
	explicit CDamageEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDamageEffect();

public:
	HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor, const _uint& iDamage);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;


	void Set_DigitScale(const _float& fDigitScale) { m_fScale = fDigitScale; }
	void SetUp_DigitScaling(const _float& fStartScale, const _float& fEndScale, const _float& fSpeed, const _float& fDeltaSpeed);

	void Set_Speed(const _float& fSpeed) { m_fSpeed = fSpeed; }
	void Set_UV(const _vec2* pStartUV, const _vec2* pUVSize) { m_vStartUV = *pStartUV, m_vUVSize = *pUVSize; }
	void Set_DeltaAlpha(const _float& fDeltaAlpha) { m_fDeltaAlpha = fDeltaAlpha; }

protected:
	HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag, const D3DXCOLOR & dwColor);
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	
	void Render_Digit(LPD3DXEFFECT pEffect,const _matrix* pmatWorld, const _uint& iDigit);

private:
	Engine::CRcTex*		m_pBufferCom = nullptr;
	Engine::CTexture*	m_pColorTextureCom = nullptr;

	_float	m_fSpeed = 1.f;
	_float	m_fAlpha = 1.f;
	_float	m_fDeltaAlpha = 0.f;

	_vec2 m_vStartUV = {};
	_vec2 m_vUVSize = { 1.f, 1.f };

	list<_uint>	m_DigitList;
	_float		m_fScale = 1.f;
	_float		m_fEndScale = 1.f;
	_float		m_fDeltaSpeed = 1.f;

	EDamageState	m_eState = EDamageState::Scaling;

public:
	static CDamageEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag, 
		const D3DXCOLOR& dwColor, const _uint& iDamage);

private:
	virtual void Free();
};


#endif // DamageEffect_h__
