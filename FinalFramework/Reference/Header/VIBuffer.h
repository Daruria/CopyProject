#ifndef VIBuffer_h__
#define VIBuffer_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer : public CResources
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGrahicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual	~CVIBuffer();

public:
	virtual HRESULT		Ready_Buffer();
	virtual void		Render_Buffer();

protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

	_ulong	m_dwVtxSize;
	_ulong	m_dwVtxCnt;
	_ulong	m_dwVtxFVF;
	_ulong	m_dwTriCnt;

	_ulong	m_dwIdxSize;
	D3DFORMAT	m_IdxFmt;

public:
	virtual void Free();
};

END

#endif // VIBuffer_h__
