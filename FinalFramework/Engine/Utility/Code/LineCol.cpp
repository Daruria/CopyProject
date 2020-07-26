#include "LineCol.h"

USING(Engine)

Engine::CLineCol::CLineCol(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CLineCol::CLineCol(const CLineCol& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CLineCol::~CLineCol()
{

}

HRESULT Engine::CLineCol::Ready_Buffer()
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 2;
	m_dwVtxFVF = FVF_COL;
	m_dwTriCnt = 1;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCOL*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec3(0.f, 0.f, 0.f);
	pVertex[0].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);

	pVertex[1].vPos = _vec3(1.f, 0.f, 0.f);
	pVertex[1].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*	pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CLineCol::Ready_Buffer(const _vec3 * pStartPos, const _vec3 * pEndPos)
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 2;
	m_dwVtxFVF = FVF_COL;
	m_dwTriCnt = 1;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCOL*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = *pStartPos;
	pVertex[0].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);

	pVertex[1].vPos = *pEndPos;
	pVertex[1].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*	pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;

	m_pIB->Unlock();

	return S_OK;
}

void CLineCol::Render_Buffer()
{
	VTXCOL*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].dwColor = m_d3dColor;
	pVertex[1].dwColor = m_d3dColor;

	m_pVB->Unlock();

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->SetIndices(m_pIB);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

Engine::CLineCol* Engine::CLineCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLineCol* pInst = new CLineCol(pGraphicDev);

	if (FAILED(pInst->Ready_Buffer()))
		Safe_Release(pInst);
	return pInst;
}

CLineCol * CLineCol::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pStartPos, const _vec3 * pEndPos)
{
	CLineCol* pInst = new CLineCol(pGraphicDev);

	if (FAILED(pInst->Ready_Buffer(pStartPos, pEndPos)))
		Safe_Release(pInst);

	return pInst;
}

Engine::CResources* Engine::CLineCol::Clone()
{
	return new CLineCol(*this);
}

void Engine::CLineCol::Free()
{
	CVIBuffer::Free();
}

