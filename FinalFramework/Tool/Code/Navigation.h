#ifndef Navigation_h__
#define Navigation_h__

#include "Base.h"

using namespace Engine;

class CNavigation : public CBase
{
	

private:
	explicit CNavigation(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNavigation();

private:
	HRESULT		Ready_Navigation();
	HRESULT		Ready_Buffer(const _ulong& dwVtxMax, const _ulong& dwLineMax, LPDIRECT3DVERTEXBUFFER9* ppVB, LPDIRECT3DINDEXBUFFER9* ppIB);
	HRESULT		Reserve_Buffer(const _ulong& dwVtxMax, const _ulong& dwLineMax);

public:
	void	Add_Vertex(const _vec3* pVtxPos);
	void	Undo_Vertex();
	void	Redo_Vertex();

	void	Render_Navigation();

private:
	void	Find_NearestVertex(const VTXCOL* pVertices, _ulong* pOutIdx1, _ulong* pOutIdx2, const _vec3* pVtxPos);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;

	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;

	_ulong	m_dwVtxSize;
	_ulong	m_dwVtxMax;
	_ulong	m_dwVtxCnt = 0;
	_ulong	m_dwVtxFVF;

	_ulong	m_dwLineCnt = 0;
	_ulong	m_dwLineMax;

	_ulong	m_dwIdxSize;
	D3DFORMAT	m_IdxFmt;

	list<pair<_ulong, _ulong> > m_WorkHistory;
	list<pair<_ulong, _ulong> >::iterator m_iterWorkHistroy;


public:
	static CNavigation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual void Free(void) override;
};


#endif // Navigation_h__
