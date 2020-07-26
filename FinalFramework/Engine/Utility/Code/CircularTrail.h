#ifndef CircularTrail_h__
#define CircularTrail_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCircularTrail : public CVIBuffer
{
private:
	explicit CCircularTrail(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCircularTrail(const CCircularTrail& rhs);
	virtual ~CCircularTrail();

public:
	HRESULT Ready_Buffer(const _ulong& dwEdge, const _float& fInnerRadius, const _float& fOuterRadius);
	virtual void Render_Buffer() override;

	void	Set_Angle(const _float& fRadian) { m_fRadian = fRadian; }
	void	Change_UV(const _float& fTimeDelta, const _float& fSpeed);
	void	Reset_Progress() { m_fProgress = 0.f; }

private:
	_float m_fRadian = 0.f;
	_float m_fProgress = 0.f;

public:
	static CCircularTrail* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwEdge);
	static CCircularTrail* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwEdge, const _float& fInnerRadius, const _float& fOuterRadius);

	virtual CResources* Clone() override;

private:
	virtual void Free();
};

END

#endif // CircularTrail_h__
