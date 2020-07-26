#include "RcAlphaTex.h"

USING(Engine)

Engine::CRcAlphaTex::CRcAlphaTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CRcAlphaTex::CRcAlphaTex(const CRcAlphaTex& rhs)
	: CVIBuffer(rhs)
{
	
}

Engine::CRcAlphaTex::~CRcAlphaTex()
{

}

HRESULT Engine::CRcAlphaTex::Ready_Buffer(const D3DXCOLOR& d3dColor)
{
	m_d3dColor = d3dColor;

	m_dwVtxSize = sizeof(VTXCOLTEX);
	m_dwVtxCnt = 4;
	m_dwVtxFVF = FVF_COLTEX;
	m_dwTriCnt = 2;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCOLTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[0].dwColor = d3dColor;
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec3(0.5f, 0.5f, 0.f);
	pVertex[1].dwColor = d3dColor;
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec3(0.5f, -0.5f, 0.f);
	pVertex[2].dwColor = d3dColor;
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[3].dwColor = d3dColor;
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	//	0
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	//	1
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CRcAlphaTex::Ready_Buffer(const D3DXCOLOR & d3dColor, RC_TEX_POS ePos)
{
	m_d3dColor = d3dColor;

	m_dwVtxSize = sizeof(VTXCOLTEX);
	m_dwVtxCnt = 4;
	m_dwVtxFVF = FVF_COLTEX;
	m_dwTriCnt = 2;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCOLTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_vec2 vStartPoint;

	switch (ePos)
	{
	case Engine::CRcAlphaTex::RTS_LEFT_TOP:			vStartPoint = { 0.f,	0.f };		break;
	case Engine::CRcAlphaTex::RTS_CENTER_TOP:		vStartPoint = { -0.5f,	0.f };		break;
	case Engine::CRcAlphaTex::RTS_RIGHT_TOP:		vStartPoint = { -1.f,	0.f };		break;
	case Engine::CRcAlphaTex::RTS_LEFT_CENTER:		vStartPoint = { 0.f,	0.5f };		break;
	case Engine::CRcAlphaTex::RTS_CENTER_CENTER:	vStartPoint = { -0.5f,	0.5f };		break;
	case Engine::CRcAlphaTex::RTS_RIGHT_CENTER:		vStartPoint = { -1.f,	0.5f };		break;
	case Engine::CRcAlphaTex::RTS_LEFT_BOTTOM:		vStartPoint = { 0.f,	1.f };		break;
	case Engine::CRcAlphaTex::RTS_CENTER_BOTTOM:	vStartPoint = { -0.5f,	1.f };		break;
	case Engine::CRcAlphaTex::RTS_RIGHT_BOTTOM:		vStartPoint = { -1.f,	1.f };		break;
	}


	pVertex[0].vPos = _vec3(vStartPoint.x, vStartPoint.y, 0.f);
	pVertex[0].dwColor = d3dColor;
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec3(vStartPoint.x + 1.f, vStartPoint.y, 0.f);
	pVertex[1].dwColor = d3dColor;
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec3(vStartPoint.x + 1.f, vStartPoint.y - 1.f, 0.f);
	pVertex[2].dwColor = d3dColor;
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec3(vStartPoint.x, vStartPoint.y - 1.f, 0.f);
	pVertex[3].dwColor = d3dColor;
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();



	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	//	0
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	//	1
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
	return S_OK;
}

void CRcAlphaTex::Set_Color(const D3DXCOLOR & d3dColor)
{
	VTXCOLTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].dwColor = d3dColor;
	pVertex[1].dwColor = d3dColor;
	pVertex[2].dwColor = d3dColor;
	pVertex[3].dwColor = d3dColor;

	m_pVB->Unlock();
}

void CRcAlphaTex::Set_LocalPos(const _vec3 * pLeftTop, const _vec3 * pRightTop, const _vec3 * pRightBottom, const _vec3 * pLeftBottom)
{
	VTXCOLTEX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = *pLeftTop;
	pVertex[1].vPos = *pRightTop;
	pVertex[2].vPos = *pRightBottom;
	pVertex[3].vPos = *pLeftBottom;

	m_pVB->Unlock();
}

void CRcAlphaTex::Set_LocalPos(RC_TEX_POS ePos)
{
	_vec2 vStartPoint;

	switch (ePos)
	{
	case Engine::CRcAlphaTex::RTS_LEFT_TOP:			vStartPoint = { 0.f,	0.f };		break;
	case Engine::CRcAlphaTex::RTS_CENTER_TOP:		vStartPoint = { -0.5f,	0.f };		break;
	case Engine::CRcAlphaTex::RTS_RIGHT_TOP:		vStartPoint = { -1.f,	0.f };		break;
	case Engine::CRcAlphaTex::RTS_LEFT_CENTER:		vStartPoint = { 0.f,	0.5f };		break;
	case Engine::CRcAlphaTex::RTS_CENTER_CENTER:	vStartPoint = { -0.5f,	0.5f };		break;
	case Engine::CRcAlphaTex::RTS_RIGHT_CENTER:		vStartPoint = { -1.f,	0.5f };		break;
	case Engine::CRcAlphaTex::RTS_LEFT_BOTTOM:		vStartPoint = { 0.f,	1.f };		break;
	case Engine::CRcAlphaTex::RTS_CENTER_BOTTOM:	vStartPoint = { -0.5f,	1.f };		break;
	case Engine::CRcAlphaTex::RTS_RIGHT_BOTTOM:		vStartPoint = { -1.f,	1.f };		break;
	}

	VTXCOLTEX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec3(vStartPoint.x, vStartPoint.y, 0.f);
	pVertex[1].vPos = _vec3(vStartPoint.x + 1.f, vStartPoint.y, 0.f);
	pVertex[2].vPos = _vec3(vStartPoint.x + 1.f, vStartPoint.y - 1.f, 0.f);
	pVertex[3].vPos = _vec3(vStartPoint.x, vStartPoint.y - 1.f, 0.f);

	m_pVB->Unlock();
}

void CRcAlphaTex::Set_UV(const _vec2 * pLeftTop, const _vec2 * pRightTop, const _vec2 * pRightBottom, const _vec2 * pLeftBottom)
{
	VTXCOLTEX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vTexUV = *pLeftTop;
	pVertex[1].vTexUV = *pRightTop;
	pVertex[2].vTexUV = *pRightBottom;
	pVertex[3].vTexUV = *pLeftBottom;

	m_pVB->Unlock();
}

void CRcAlphaTex::Set_UV(const _vec2 * pLeftTop, const _float & fU, const _float & fV)
{
	VTXCOLTEX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vTexUV = *pLeftTop;
	pVertex[1].vTexUV = _vec2(pLeftTop->x + fU, pLeftTop->y);
	pVertex[2].vTexUV = _vec2(pLeftTop->x + fU, pLeftTop->y + fV);
	pVertex[3].vTexUV = _vec2(pLeftTop->x, pLeftTop->y + fV);

	m_pVB->Unlock();
}

void CRcAlphaTex::Set_UV(RC_TEX_POS ePos, const _float & fU, const _float & fV)
{
	VTXCOLTEX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	switch (ePos)
	{
	case Engine::CRcAlphaTex::RTS_LEFT_TOP:
		pVertex[0].vTexUV = _vec2(0.f, 0.f);
		pVertex[1].vTexUV = _vec2(fU, 0.f);
		pVertex[2].vTexUV = _vec2(fU, fV);
		pVertex[3].vTexUV = _vec2(0.f, fV);
		break;
	case Engine::CRcAlphaTex::RTS_CENTER_TOP:
		pVertex[0].vTexUV = _vec2(0.5f - 0.5f * fU, 0.f);
		pVertex[1].vTexUV = _vec2(0.5f + 0.5f * fU, 0.f);
		pVertex[2].vTexUV = _vec2(0.5f + 0.5f * fU, fV);
		pVertex[3].vTexUV = _vec2(0.5f - 0.5f * fU, fV);
		break;
	case Engine::CRcAlphaTex::RTS_RIGHT_TOP:
		pVertex[0].vTexUV = _vec2(1.f - fU, 0.f);
		pVertex[1].vTexUV = _vec2(1.f, 0.f);
		pVertex[2].vTexUV = _vec2(1.f, fV);
		pVertex[3].vTexUV = _vec2(1.f - fU, fV);
		break;
	case Engine::CRcAlphaTex::RTS_LEFT_CENTER:
		pVertex[0].vTexUV = _vec2(0.f, 0.5f - 0.5f * fV);
		pVertex[1].vTexUV = _vec2(fU, 0.5f - 0.5f * fV);
		pVertex[2].vTexUV = _vec2(fU, 0.5f + 0.5f * fV);
		pVertex[3].vTexUV = _vec2(0.f, 0.5f + 0.5f * fV);
		break;
	case Engine::CRcAlphaTex::RTS_CENTER_CENTER:
		pVertex[0].vTexUV = _vec2(0.5f - 0.5f * fU, 0.5f - 0.5f * fV);
		pVertex[1].vTexUV = _vec2(0.5f + 0.5f * fU, 0.5f - 0.5f * fV);
		pVertex[2].vTexUV = _vec2(0.5f + 0.5f * fU, 0.5f + 0.5f * fV);
		pVertex[3].vTexUV = _vec2(0.5f - 0.5f * fU, 0.5f + 0.5f * fV);
		break;
	case Engine::CRcAlphaTex::RTS_RIGHT_CENTER:
		pVertex[0].vTexUV = _vec2(1.f - fU, 0.5f - 0.5f * fV);
		pVertex[1].vTexUV = _vec2(1.f, 0.5f - 0.5f * fV);
		pVertex[2].vTexUV = _vec2(1.f, 0.5f + 0.5f * fV);
		pVertex[3].vTexUV = _vec2(1.f - fU, 0.5f + 0.5f * fV);
		break;
	case Engine::CRcAlphaTex::RTS_LEFT_BOTTOM:
		pVertex[0].vTexUV = _vec2(0.f, 1.f - fV);
		pVertex[1].vTexUV = _vec2(fU, 1.f - fV);
		pVertex[2].vTexUV = _vec2(fU, 1.f);
		pVertex[3].vTexUV = _vec2(0.f, 1.f);
		break;
	case Engine::CRcAlphaTex::RTS_CENTER_BOTTOM:
		pVertex[0].vTexUV = _vec2(0.5f - 0.5f * fU, 1.f - fV);
		pVertex[1].vTexUV = _vec2(0.5f + 0.5f * fU, 1.f - fV);
		pVertex[2].vTexUV = _vec2(0.5f + 0.5f * fU, 1.f);
		pVertex[3].vTexUV = _vec2(0.5f - 0.5f * fU, 1.f);
		break;
	case Engine::CRcAlphaTex::RTS_RIGHT_BOTTOM:
		pVertex[0].vTexUV = _vec2(1.f - fU, 1.f - fV);
		pVertex[1].vTexUV = _vec2(1.f, 1.f - fV);
		pVertex[2].vTexUV = _vec2(1.f, 1.f);
		pVertex[3].vTexUV = _vec2(1.f - fU, 1.f);
		break;
	}

	m_pVB->Unlock();
}

void CRcAlphaTex::Set_UV(RC_TEX_POS ePos, const _vec2 * pUV)
{
	Set_UV(ePos, pUV->x, pUV->y);
}

Engine::CRcAlphaTex* Engine::CRcAlphaTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DXCOLOR& d3dColor)
{
	CRcAlphaTex* pInst = new CRcAlphaTex(pGraphicDev);

	if (FAILED(pInst->Ready_Buffer(d3dColor)))
		Safe_Release(pInst);

	return pInst;
}

CRcAlphaTex * CRcAlphaTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DXCOLOR & d3dColor, RC_TEX_POS ePos)
{
	CRcAlphaTex* pInst = new CRcAlphaTex(pGraphicDev);

	if (FAILED(pInst->Ready_Buffer(d3dColor, ePos)))
		Safe_Release(pInst);

	return pInst;
}

Engine::CResources* Engine::CRcAlphaTex::Clone()
{
	return new CRcAlphaTex(*this);
}

void Engine::CRcAlphaTex::Free()
{
	CVIBuffer::Free();
}

