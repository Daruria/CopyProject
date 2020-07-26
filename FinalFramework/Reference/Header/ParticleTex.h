#ifndef ParticleTex_h__
#define ParticleTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CParticleTex : public CVIBuffer
{

private:
	explicit CParticleTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CParticleTex(const CParticleTex& rhs);
	virtual ~CParticleTex();

public:
	HRESULT Ready_Buffer(const _ulong& dwVtxCnt);
	void	Render_Buffer(const _float& fTimeDelta) ;

	void	Reset_Buffer();

	void	Set_Speed(const _float& fSpeed) { m_fSpeed = fSpeed; }

private:
	LPDIRECT3DVERTEXBUFFER9			m_pVBTransform;	
	LPDIRECT3DVERTEXDECLARATION9	m_pDeclaration;	
	
	_float	m_fSpeed = 10.f;
	_float	m_fGravity = 0.f;
	
public:
	static CParticleTex* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwVtxCnt);

	virtual CResources* Clone() override;

private:
	virtual void Free();
};

END

#endif // ParticleTex_h__
