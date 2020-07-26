#ifndef DamageFont_h__
#define DamageFont_h__

#include "UI.h"

class CDamageFont : public CUI
{
	enum class EFontState { Scaling, ScaleStop };

protected:
	explicit CDamageFont(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDamageFont();

public:
	virtual HRESULT Ready_GameObject(const _uint& iNumber);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;

	void	SetUp_Scaling(const _float& fStartScale, const _float& fMaxScale, const _float& fEndScale, const _float& fSpeed);
	void	SetUp_ScalingDelta(const _float& fStartScale, const _float& fEndScale, const _float& fSpeed, const _float& fDeltaSpeed);
	void	Set_FontScale(const _float& fFontScale) { m_fScale = fFontScale; }
	void	Set_LifeTime(const _float& fLifeTime) { m_fLifeTime = fLifeTime; }

private:
	void Render_Digit(const _matrix* pWorldMatrix, const _uint& iDigit);

private:
	list<_uint>		m_DigitList;
	_uint			m_iDamage = 0;
	
	_float			m_fLifeCount = 0.f;
	_float			m_fLifeTime = 1.f;

	_float			m_fScale = 1.f;
	_float			m_fMaxScale = 2.f;
	_float			m_fEndScale = 1.f;
	_float			m_fSpeed = 1.f;
	_float			m_fDeltaSpeed = 1.f;

	EFontState		m_eState = EFontState::Scaling;

public:
	static CDamageFont* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iNumber);

protected:
	virtual void Free();
};

#endif // DamageFont_h__
