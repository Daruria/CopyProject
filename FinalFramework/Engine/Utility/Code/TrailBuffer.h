#ifndef TrailBuffer_h__
#define TrailBuffer_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTrailBuffer : public CVIBuffer
{
private:
	explicit CTrailBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTrailBuffer(const CTrailBuffer& rhs);
	virtual ~CTrailBuffer();

public:
	HRESULT Ready_Buffer(const _ulong& dwVtxMax = 400);
	virtual void	Render_Buffer() override;
	void	Render_Buffer(const list<pair<_vec3, _vec3> >* pTrailList, const _uint& iInterpoleCnt = 3);

public:
	HRESULT Add_Vertex(const _vec3* pTopPos, const _vec3* pBottomPos);
	HRESULT	Clear_Vertex();
	HRESULT Add_Vertex_CatmullRom(const list<pair<_vec3, _vec3> >* pTrailList, const _uint& iInterpoleCnt);
	

private:
	_ulong	m_dwUsingVtxCnt = 0;
	_ulong	m_dwUsingTriCnt = 0;

public:
	static CTrailBuffer* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwVtxMax = 400);

	virtual CResources* Clone() override;

private:
	virtual void Free();
};

END

#endif // TrailBuffer_h__
