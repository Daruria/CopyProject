#include "Screen.h"

USING(Engine)

Engine::CScreen::CScreen(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CScreen::CScreen(const CScreen& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CScreen::~CScreen()
{

}

HRESULT CScreen::Ready_Buffer(const _float & fX, const _float & fY, const _float & fSizeX, const _float & fSizeY)
{
	m_dwVtxSize = sizeof(VTXSCREEN);
	m_dwVtxCnt = 4;
	m_dwVtxFVF = FVF_SCREEN;
	m_dwTriCnt = 2;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXSCREEN*			pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec4(fX, fY, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec4(fX + fSizeX, fY, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec4(fX + fSizeX, fY + fSizeY, 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec4(fX, fY + fSizeY, 0.f, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();


	INDEX16*		pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// 0
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	// 1
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

Engine::CScreen* Engine::CScreen::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float & fX, const _float & fY, const _float & fSizeX, const _float & fSizeY)
{
	CScreen* pInst = new CScreen(pGraphicDev);

	if (FAILED(pInst->Ready_Buffer(fX, fY, fSizeX, fSizeY)))
		Safe_Release(pInst);

	return pInst;
}

Engine::CResources* Engine::CScreen::Clone()
{
	return new CScreen(*this);
}

void Engine::CScreen::Free()
{
	CVIBuffer::Free();
}

