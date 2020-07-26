#ifndef CircleTex_h__
#define CircleTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCircleTex : public CVIBuffer
{
private:
	explicit CCircleTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCircleTex(const CCircleTex& rhs);
	virtual ~CCircleTex();

public:
	HRESULT Ready_Buffer(const _ulong& dwEdge);
	void	Move_UV_Up(const _float& fTimeDelta, const _float& fSpeed);
	virtual void Render_Buffer() override;

private:
	_float m_fTop = 0.f;
	_float m_fBottom = 1.f;

public:
	static CCircleTex* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwEdge);

	virtual CResources* Clone() override;

private:
	virtual void Free();
};

END

#endif // CircleTex_h__
