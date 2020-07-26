#include "stdafx.h"
#include "Navigation.h"

CNavigation::CNavigation(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CNavigation::~CNavigation()
{
}

HRESULT CNavigation::Ready_Navigation()
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwIdxSize = sizeof(INDEX32_LINE);

	m_IdxFmt = D3DFMT_INDEX32;
	m_dwVtxFVF = FVF_COL;

	m_dwVtxCnt = 0;
	m_dwLineCnt = 0;

	m_WorkHistory.push_back(make_pair(m_dwVtxCnt, m_dwLineCnt));
	m_iterWorkHistroy = m_WorkHistory.end();

	FAILED_CHECK_RETURN(Reserve_Buffer(1000, 1000), E_FAIL);


	return S_OK;
}

HRESULT CNavigation::Ready_Buffer(const _ulong& dwVtxMax, const _ulong& dwLineMax, LPDIRECT3DVERTEXBUFFER9* ppVB, LPDIRECT3DINDEXBUFFER9* ppIB)
{
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * dwVtxMax, 0, m_dwVtxFVF, D3DPOOL_MANAGED, ppVB, NULL), E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * dwLineMax, 0, m_IdxFmt, D3DPOOL_MANAGED, ppIB, NULL), E_FAIL);

	return S_OK;
}

HRESULT CNavigation::Reserve_Buffer(const _ulong & dwVtxMax, const _ulong & dwLineMax)
{
	if (!m_pVB && !m_pIB)
	{
		m_dwVtxMax = dwVtxMax;
		m_dwLineMax = dwLineMax;

		Ready_Buffer(dwVtxMax, dwLineMax, &m_pVB, &m_pIB);

		return S_OK;
	}

	LPDIRECT3DVERTEXBUFFER9 pNewVB;
	LPDIRECT3DINDEXBUFFER9	pNewIB;

	Ready_Buffer(dwVtxMax, dwLineMax, &pNewVB, &pNewIB);

	VTXCOL* pOldVertex = nullptr, * pNewVertex = nullptr;
	
	m_pVB->Lock(0, 0, (void**)&pOldVertex, 0);
	pNewVB->Lock(0, 0, (void**)&pNewVertex, 0);

	memcpy(pNewVertex, pOldVertex, sizeof(VTXCOL) * m_dwVtxCnt);
	m_pVB->Unlock();
	pNewVB->Unlock();

	INDEX32_LINE*	pOldIndex = nullptr, *pNewIndex = nullptr;
	
	m_pIB->Lock(0, 0, (void**)&pOldIndex, 0);
	pNewIB->Lock(0, 0, (void**)&pNewIndex, 0);

	memcpy(pNewIndex, pOldIndex, sizeof(INDEX32_LINE) * m_dwLineCnt);

	m_pIB->Unlock();
	pNewIB->Unlock();

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	m_pVB = pNewVB;
	m_pIB = pNewIB;

	return S_OK;
}

void CNavigation::Add_Vertex(const _vec3 * pVtxPos)
{
	if (m_WorkHistory.end() != m_iterWorkHistroy)
	{
		++m_iterWorkHistroy;
		m_WorkHistory.erase(m_iterWorkHistroy, m_WorkHistory.end());

	}

	VTXCOL*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[m_dwVtxCnt].vPos = *pVtxPos;
	pVertex[m_dwVtxCnt].dwColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);


	++m_dwVtxCnt;

	INDEX32_LINE*	pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);
	if (2 == m_dwVtxCnt)
	{
		pIndex[m_dwLineCnt]._0 = 0;
		pIndex[m_dwLineCnt]._1 = 1;
		++m_dwLineCnt;
	}
	else if (m_dwVtxCnt > 2)
	{
		_ulong dwNearestIdx1 = 0;
		_ulong dwNearestIdx2 = 0;

		Find_NearestVertex(pVertex, &dwNearestIdx1, &dwNearestIdx2, pVtxPos);

		_bool bLine1To2 = false;

		for (_uint i = 0; i < m_dwLineCnt; ++i)
		{
			if ((dwNearestIdx1 == pIndex[i]._0 && dwNearestIdx2 == pIndex[i]._1) ||
				(dwNearestIdx1 == pIndex[i]._1 && dwNearestIdx2 == pIndex[i]._0))
			{
				bLine1To2 = true;
				break;
			}
		}

		_vec3 vCross;
		D3DXVec3Cross(&vCross, &(pVertex[dwNearestIdx1].vPos - *pVtxPos), &(pVertex[dwNearestIdx2].vPos - *pVtxPos));
		if (D3DXVec3Dot(&vCross, &AXIS_Y) < 0.f)	//	¹Ý½Ã°è.
		{
			pIndex[m_dwLineCnt]._0 = dwNearestIdx1;
			pIndex[m_dwLineCnt]._1 = m_dwVtxCnt - 1;
			++m_dwLineCnt;
			pIndex[m_dwLineCnt]._0 = m_dwVtxCnt - 1;
			pIndex[m_dwLineCnt]._1 = dwNearestIdx2;
			++m_dwLineCnt;
		}
		else
		{
			pIndex[m_dwLineCnt]._0 = m_dwVtxCnt - 1;
			pIndex[m_dwLineCnt]._1 = dwNearestIdx1;
			++m_dwLineCnt;
			pIndex[m_dwLineCnt]._0 = dwNearestIdx2;
			pIndex[m_dwLineCnt]._1 = m_dwVtxCnt - 1;
			++m_dwLineCnt;
		}

		if (!bLine1To2)
		{
			pIndex[m_dwLineCnt]._0 = dwNearestIdx1;
			pIndex[m_dwLineCnt]._1 = dwNearestIdx2;
			++m_dwLineCnt;
		}
	}
	m_pIB->Unlock();

	m_pVB->Unlock();


	m_WorkHistory.push_back(make_pair(m_dwVtxCnt, m_dwLineCnt));
	m_iterWorkHistroy = m_WorkHistory.end();
}

void CNavigation::Undo_Vertex()
{
	if (m_WorkHistory.begin() == m_iterWorkHistroy)
		return;

	if (m_WorkHistory.end() == m_iterWorkHistroy)
		--m_iterWorkHistroy;
	
	--m_iterWorkHistroy;
	
	m_dwVtxCnt = m_iterWorkHistroy->first;
	m_dwLineCnt = m_iterWorkHistroy->second;


}

void CNavigation::Redo_Vertex()
{
	auto iter = m_WorkHistory.end();
	--iter;
	if (iter == m_iterWorkHistroy || m_WorkHistory.end() == m_iterWorkHistroy)
		return;

	++m_iterWorkHistroy;

	m_dwVtxCnt = m_iterWorkHistroy->first;
	m_dwLineCnt = m_iterWorkHistroy->second;
}

void CNavigation::Render_Navigation()
{
	_matrix matIdentity;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, D3DXMatrixIdentity(&matIdentity));

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->SetIndices(m_pIB);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, m_dwVtxCnt, 0, m_dwLineCnt);
}

void CNavigation::Find_NearestVertex(const VTXCOL * pVertices, _ulong * pOutIdx1, _ulong * pOutIdx2, const _vec3 * pVtxPos)
{
	_float fNearDist1 = D3DXVec3Length(&(pVertices[0].vPos - *pVtxPos));
	_float fNearDist2 = D3DXVec3Length(&(pVertices[1].vPos - *pVtxPos));
	*pOutIdx1 = 0;
	*pOutIdx2 = 1;

	_float fTemp = 0.f;
	for (_uint i = 2; i < m_dwVtxCnt - 1; ++i)
	{
		fTemp = D3DXVec3Length(&(pVertices[i].vPos - *pVtxPos));

		if (fNearDist1 < fTemp && fNearDist2 < fTemp)
			continue;

		if (fTemp < fNearDist1)
		{
			if (fNearDist1 < fNearDist2)	//	fTemp < fNearDist1 < fNearDist2
			{
				fNearDist2 = fTemp;
				*pOutIdx2 = i;
			}
			else //	fTemp < fNearDist1, fNearDest2 < fNearDest 1
			{
				fNearDist1 = fTemp;
				*pOutIdx1 = i;
			}
		}
		else if (fTemp < fNearDist2)	//	fNearDist1 < fTemp < fNearDist2
		{
			fNearDist2 = fTemp;
			*pOutIdx2 = i;
		}
	}
}

CNavigation * CNavigation::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNavigation* pInst = new CNavigation(pGraphicDev);

	if (FAILED(pInst->Ready_Navigation()))
		Safe_Release(pInst);
	
	return pInst;
}

void CNavigation::Free(void)
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pGraphicDev);
}

