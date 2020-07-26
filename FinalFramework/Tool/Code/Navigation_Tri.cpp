#include "stdafx.h"
#include "Navigation_Tri.h"
#include "Cell.h"

CNavigation_Tri::CNavigation_Tri(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CNavigation_Tri::~CNavigation_Tri()
{
}

HRESULT CNavigation_Tri::Ready_Navigation_Tri()
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwIdxSize = sizeof(INDEX32);

	m_IdxFmt = D3DFMT_INDEX32;
	m_dwVtxFVF = FVF_COL;

	m_dwVtxCnt = 0;
	m_dwTriCnt = 0;

	m_WorkHistory.push_back(make_pair(m_dwVtxCnt, m_dwTriCnt));
	m_iterWorkHistroy = m_WorkHistory.end();

	FAILED_CHECK_RETURN(Reserve_Buffer(1000, 1000), E_FAIL);
	m_vecCellOption.reserve(1000);

	return S_OK;
}

HRESULT CNavigation_Tri::Ready_Buffer(const _ulong& dwVtxMax, const _ulong& dwTriMax, LPDIRECT3DVERTEXBUFFER9* ppVB, LPDIRECT3DINDEXBUFFER9* ppIB)
{
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * dwVtxMax, 0, m_dwVtxFVF, D3DPOOL_MANAGED, ppVB, NULL), E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * dwTriMax, 0, m_IdxFmt, D3DPOOL_MANAGED, ppIB, NULL), E_FAIL);

	return S_OK;
}

HRESULT CNavigation_Tri::Reserve_Buffer(const _ulong & dwVtxMax, const _ulong & dwTriMax)
{
	if (!m_pVB && !m_pIB)
	{
		m_dwVtxMax = dwVtxMax;
		m_dwTriMax = dwTriMax;

		Ready_Buffer(dwVtxMax, dwTriMax, &m_pVB, &m_pIB);

		return S_OK;
	}

	LPDIRECT3DVERTEXBUFFER9 pNewVB;
	LPDIRECT3DINDEXBUFFER9	pNewIB;

	Ready_Buffer(dwVtxMax, dwTriMax, &pNewVB, &pNewIB);

	VTXCOL* pOldVertex = nullptr, * pNewVertex = nullptr;
	
	m_pVB->Lock(0, 0, (void**)&pOldVertex, 0);
	pNewVB->Lock(0, 0, (void**)&pNewVertex, 0);

	memcpy(pNewVertex, pOldVertex, sizeof(VTXCOL) * m_dwVtxCnt);
	m_pVB->Unlock();
	pNewVB->Unlock();

	INDEX32*	pOldIndex = nullptr, *pNewIndex = nullptr;
	
	m_pIB->Lock(0, 0, (void**)&pOldIndex, 0);
	pNewIB->Lock(0, 0, (void**)&pNewIndex, 0);

	memcpy(pNewIndex, pOldIndex, sizeof(INDEX32) * m_dwTriCnt);

	m_pIB->Unlock();
	pNewIB->Unlock();

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	m_pVB = pNewVB;
	m_pIB = pNewIB;

	return S_OK;
}

_bool CNavigation_Tri::Picking_Vertex(const _vec3 * pPos, _ulong* dwOutIdx, _vec3* pOutPos)
{
	VTXCOL* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_float fMinDist = m_fPickingRadius;
	_float fDist = 0.f;
	_ulong dwPickIdx = m_dwVtxCnt;
	for (_uint i = 0; i < m_dwVtxCnt; ++i)
	{
		fDist = D3DXVec3Length(&(pVertex[i].vPos - *pPos));
		if (fDist < fMinDist)
		{
			fMinDist = fDist;
			if (pOutPos)
				*pOutPos = pVertex[i].vPos;
			dwPickIdx = i;
		}
	}

	*dwOutIdx = dwPickIdx;
	
	m_pVB->Unlock();
	
	return !(dwPickIdx == m_dwVtxCnt);
}

_bool CNavigation_Tri::Picking_Vertex(const _vec2 * pMousePos, const _matrix* pmatView, const _matrix* pmatProj, 
	_ulong * dwOutIdx, _vec3 * pOutPos)
{
	if (m_dwVtxCnt < 1)
		return false;

	_vec3 vMouseProj, vVtxProjPos;
	CMyMath::ViewPortToProj(&vMouseProj, pMousePos, WINCX, WINCY);

	VTXCOL* pVertex;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	
	D3DXVec3TransformCoord(&vVtxProjPos, &pVertex[0].vPos, pmatView);
	D3DXVec3TransformCoord(&vVtxProjPos, &vVtxProjPos, pmatProj);

	_ulong dwVtxIdx = 0;
	_float fMinDist = D3DXVec2Length(&((_vec2)vMouseProj - (_vec2)vVtxProjPos));
	_float fDist;

	for (_uint i = 1; i < m_dwVtxCnt; ++i)
	{
		D3DXVec3TransformCoord(&vVtxProjPos, &pVertex[i].vPos, pmatView);
		D3DXVec3TransformCoord(&vVtxProjPos, &vVtxProjPos, pmatProj);
		fDist = D3DXVec2Length(&((_vec2)vMouseProj - (_vec2)vVtxProjPos));
		
		if (fDist < fMinDist)
		{
			fMinDist = fDist;
			dwVtxIdx = i;
		}
	}

	if (pOutPos)
		*pOutPos = pVertex[dwVtxIdx].vPos;
	
	*dwOutIdx = dwVtxIdx;

	m_pVB->Unlock();

	return true;
}

_bool CNavigation_Tri::Picking_Triangle(const _vec3* pRayPos, const _vec3* pRayDir, _ulong* dwOutIdx)
{
	VTXCOL* pVertex = nullptr;
	INDEX32* pIndex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	_float fU, fV, fDist;
	for (_ulong i = 0; i < m_dwTriCnt; ++i)
	{
		if (D3DXIntersectTri(&pVertex[pIndex[i]._0].vPos, &pVertex[pIndex[i]._1].vPos, &pVertex[pIndex[i]._2].vPos,
			pRayPos, pRayDir, &fU, &fV, &fDist))
		{
			*dwOutIdx = i;
			m_pVB->Unlock();
			m_pIB->Unlock();
			return true;
		}
	}

	m_pVB->Unlock();
	m_pIB->Unlock();

	return false;
}

void CNavigation_Tri::Set_Y(const _ulong & dwVtxIdx, const _float & fy)
{

}

void CNavigation_Tri::Change_VertexColor(const _ulong & dwVtxIdx, D3DXCOLOR d3dColor)
{
	VTXCOL* pVertex;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[dwVtxIdx].dwColor = d3dColor;

	m_pVB->Unlock();

}

void CNavigation_Tri::Change_VertexPosY(const _ulong & dwVtxIdx, const _float & fY)
{
	VTXCOL* pVertex;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[dwVtxIdx].vPos.y = fY;

	m_pVB->Unlock();
}

_ulong CNavigation_Tri::Add_Vertex(const _vec3 * pVtxPos)
{
	if (m_WorkHistory.end() != m_iterWorkHistroy)
	{
		++m_iterWorkHistroy;
		m_WorkHistory.erase(m_iterWorkHistroy, m_WorkHistory.end());

	}

	VTXCOL*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[m_dwVtxCnt].vPos = *pVtxPos;
	pVertex[m_dwVtxCnt].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);


	++m_dwVtxCnt;

	INDEX32*	pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	if (m_dwVtxCnt > 2)
	{
		_ulong dwNearestIdx1 = 0;
		_ulong dwNearestIdx2 = 0;

		Find_NearestVertex(pVertex, &dwNearestIdx1, &dwNearestIdx2, pVtxPos);

		_vec3 vCross;
		D3DXVec3Cross(&vCross, &(pVertex[dwNearestIdx1].vPos - *pVtxPos), &(pVertex[dwNearestIdx2].vPos - *pVtxPos));
		if (D3DXVec3Dot(&vCross, &AXIS_Y) < 0.f)	//	반시계.
		{
			pIndex[m_dwTriCnt]._0 = dwNearestIdx1;
			pIndex[m_dwTriCnt]._1 = m_dwVtxCnt - 1;
			pIndex[m_dwTriCnt]._2 = dwNearestIdx2;
			++m_dwTriCnt;
		}
		else
		{
			pIndex[m_dwTriCnt]._0 = m_dwVtxCnt - 1;
			pIndex[m_dwTriCnt]._1 = dwNearestIdx1;
			pIndex[m_dwTriCnt]._2 = dwNearestIdx2;
			++m_dwTriCnt;
		}

		m_vecCellOption.push_back(0);
	}
	m_pIB->Unlock();

	m_pVB->Unlock();


	m_WorkHistory.push_back(make_pair(m_dwVtxCnt, m_dwTriCnt));
	m_iterWorkHistroy = m_WorkHistory.end();

	return m_dwVtxCnt - 1;
}

_ulong CNavigation_Tri::Add_Vertex(const vector<_vec3>* pvecVertices)
{
	if (m_WorkHistory.end() != m_iterWorkHistroy)
	{
		++m_iterWorkHistroy;
		m_WorkHistory.erase(m_iterWorkHistroy, m_WorkHistory.end());

	}

	VTXCOL*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[m_dwVtxCnt].vPos = (*pvecVertices)[0];
	pVertex[m_dwVtxCnt].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	++m_dwVtxCnt;

	pVertex[m_dwVtxCnt].vPos = (*pvecVertices)[1];
	pVertex[m_dwVtxCnt].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	++m_dwVtxCnt;

	pVertex[m_dwVtxCnt].vPos = (*pvecVertices)[2];
	pVertex[m_dwVtxCnt].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	++m_dwVtxCnt;

	INDEX32*	pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	if (m_dwVtxCnt > 2)
	{
		_vec3 vCross;
		D3DXVec3Cross(&vCross, 
			&(pVertex[m_dwVtxCnt - 3].vPos - pVertex[m_dwVtxCnt - 1].vPos), 
			&(pVertex[m_dwVtxCnt - 2].vPos - pVertex[m_dwVtxCnt - 1].vPos));
		if (D3DXVec3Dot(&vCross, &AXIS_Y) < 0.f)	//	반시계.
		{
			pIndex[m_dwTriCnt]._0 = m_dwVtxCnt - 3;
			pIndex[m_dwTriCnt]._1 = m_dwVtxCnt - 1;
			pIndex[m_dwTriCnt]._2 = m_dwVtxCnt - 2;
			++m_dwTriCnt;
		}
		else
		{
			pIndex[m_dwTriCnt]._0 = m_dwVtxCnt - 1;
			pIndex[m_dwTriCnt]._1 = m_dwVtxCnt - 3;
			pIndex[m_dwTriCnt]._2 = m_dwVtxCnt - 2;
			++m_dwTriCnt;
		}

		m_vecCellOption.push_back(0);
	}
	m_pIB->Unlock();

	m_pVB->Unlock();


	m_WorkHistory.push_back(make_pair(m_dwVtxCnt, m_dwTriCnt));
	m_iterWorkHistroy = m_WorkHistory.end();

	return m_dwVtxCnt - 1;
}

_ulong CNavigation_Tri::Add_Vertex(const _vec3 * pVtxPos, const _ulong & dwPickedIdx1, const _ulong & dwPickedIdx2)
{
	if (m_WorkHistory.end() != m_iterWorkHistroy)
	{
		++m_iterWorkHistroy;
		m_WorkHistory.erase(m_iterWorkHistroy, m_WorkHistory.end());

	}

	VTXCOL*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[m_dwVtxCnt].vPos = *pVtxPos;
	pVertex[m_dwVtxCnt].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	++m_dwVtxCnt;

	INDEX32*	pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	if (m_dwVtxCnt > 2)
	{
		_vec3 vCross;
		D3DXVec3Cross(&vCross, &(pVertex[dwPickedIdx1].vPos - *pVtxPos), &(pVertex[dwPickedIdx2].vPos - *pVtxPos));
		if (D3DXVec3Dot(&vCross, &AXIS_Y) < 0.f)	//	반시계.
		{
			pIndex[m_dwTriCnt]._0 = dwPickedIdx1;
			pIndex[m_dwTriCnt]._1 = m_dwVtxCnt - 1;
			pIndex[m_dwTriCnt]._2 = dwPickedIdx2;
			++m_dwTriCnt;
		}
		else
		{
			pIndex[m_dwTriCnt]._0 = m_dwVtxCnt - 1;
			pIndex[m_dwTriCnt]._1 = dwPickedIdx1;
			pIndex[m_dwTriCnt]._2 = dwPickedIdx2;
			++m_dwTriCnt;
		}

		m_vecCellOption.push_back(0);
	}
	m_pIB->Unlock();

	m_pVB->Unlock();


	m_WorkHistory.push_back(make_pair(m_dwVtxCnt, m_dwTriCnt));
	m_iterWorkHistroy = m_WorkHistory.end();

	return m_dwVtxCnt - 1;
}

_bool CNavigation_Tri::Remove_Vertex(const _ulong & dwIndex)
{
	if (m_dwVtxCnt < 1)
		return false;

	
	if (m_WorkHistory.end() != m_iterWorkHistroy)
	{
		++m_iterWorkHistroy;
		m_WorkHistory.erase(m_iterWorkHistroy, m_WorkHistory.end());

	}

	VTXCOL* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	memcpy(&pVertex[dwIndex], &pVertex[dwIndex + 1], sizeof(VTXCOL) * (m_dwVtxCnt - dwIndex + 1));

	m_pVB->Unlock();

	--m_dwVtxCnt;

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (_int i = m_dwTriCnt - 1; i >= 0; --i)
	{
		if (dwIndex == pIndex[i]._0 || dwIndex == pIndex[i]._1 || dwIndex == pIndex[i]._2)
		{
			if (i != m_dwTriCnt - 1)
				memcpy(&pIndex[i], &pIndex[i + 1], sizeof(INDEX32) * (m_dwTriCnt - i - 1));
			--m_dwTriCnt;
		}
	}

	for (_uint i = 0; i < m_dwTriCnt; ++i)
	{
		if (pIndex[i]._0 > dwIndex)
			--pIndex[i]._0;
		if (pIndex[i]._1 > dwIndex)
			--pIndex[i]._1;
		if (pIndex[i]._2 > dwIndex)
			--pIndex[i]._2;
	}

	m_pIB->Unlock();

	m_vecCellOption.erase(m_vecCellOption.begin() + dwIndex);

	m_WorkHistory.push_back(make_pair(m_dwVtxCnt, m_dwTriCnt));
	m_iterWorkHistroy = m_WorkHistory.end();

	return true;
}

_ulong CNavigation_Tri::Connect_Vertex(const _ulong& dwConnectIdx, const _ulong & dwPickedIdx1, const _ulong & dwPickedIdx2)
{
	if (m_WorkHistory.end() != m_iterWorkHistroy)
	{
		++m_iterWorkHistroy;
		m_WorkHistory.erase(m_iterWorkHistroy, m_WorkHistory.end());

	}

	VTXCOL*		pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	INDEX32*	pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	if (m_dwVtxCnt > 2)
	{
		_vec3 vCross;
		D3DXVec3Cross(&vCross, &(pVertex[dwPickedIdx1].vPos - pVertex[dwConnectIdx].vPos), 
			&(pVertex[dwPickedIdx2].vPos - pVertex[dwConnectIdx].vPos));
		if (D3DXVec3Dot(&vCross, &AXIS_Y) < 0.f)	//	반시계.
		{
			pIndex[m_dwTriCnt]._0 = dwPickedIdx1;
			pIndex[m_dwTriCnt]._1 = dwConnectIdx;
			pIndex[m_dwTriCnt]._2 = dwPickedIdx2;
			++m_dwTriCnt;
		}
		else
		{
			pIndex[m_dwTriCnt]._0 = dwConnectIdx;
			pIndex[m_dwTriCnt]._1 = dwPickedIdx1;
			pIndex[m_dwTriCnt]._2 = dwPickedIdx2;
			++m_dwTriCnt;
		}
		m_vecCellOption.push_back(0);
	}
	m_pIB->Unlock();

	m_pVB->Unlock();

	m_WorkHistory.push_back(make_pair(m_dwVtxCnt, m_dwTriCnt));
	m_iterWorkHistroy = m_WorkHistory.end();

	return dwConnectIdx;
}

_bool CNavigation_Tri::Disunite_Vertex(const _ulong & dwDisuniteIdx, const _ulong & dwPickedIdx1, const _ulong & dwPickedIdx2)
{
	VTXCOL* pVertex = nullptr;
	INDEX32* pIndex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	_bool	bPossible = false;
	_ulong dwTriIdx = 0;

	const _ulong dwPicked[3] = { dwDisuniteIdx, dwPickedIdx1, dwPickedIdx2 };

	for (_uint i = 0; i < m_dwTriCnt; ++i)
	{
		_uint uiCorrectCnt = 0;
		for (_uint j = 0; j < 3; ++j)
		{
			for (_uint k = 0; k < 3; ++k)
			{
				if (dwPicked[j] == *((_ulong*)&pIndex[i] + k))
					++uiCorrectCnt;
			}
		}

		if (uiCorrectCnt == 3)
		{
			bPossible = true;
			dwTriIdx = i;
			break;
		}

		//if ((dwDisuniteIdx == pIndex[i]._0 || dwDisuniteIdx == pIndex[i]._1 || dwDisuniteIdx == pIndex[i]._2) &&
		//	(dwDisuniteIdx == pIndex[i]._0 || dwDisuniteIdx == pIndex[i]._1 || dwDisuniteIdx == pIndex[i]._2) &&
		//	(dwDisuniteIdx == pIndex[i]._0 || dwDisuniteIdx == pIndex[i]._1 || dwDisuniteIdx == pIndex[i]._2))
		//{
		//	bPossible = true;
		//	dwTriIdx = i;
		//	break;
		//}
	}
	if (!bPossible)
	{
		m_pIB->Unlock();
		m_pVB->Unlock();
		return false;
	}

	pVertex[m_dwVtxCnt].vPos = pVertex[dwDisuniteIdx].vPos;
	pVertex[m_dwVtxCnt].vPos.y += 0.5f;
	pVertex[m_dwVtxCnt].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	++m_dwVtxCnt;
	
	for (_uint i = 0; i < 3; ++i)
	{
		if (dwDisuniteIdx == *((_ulong*)&pIndex[dwTriIdx] + i))
		{
			*((_ulong*)&pIndex[dwTriIdx] + i) = m_dwVtxCnt - 1;
		}
	}

	m_pIB->Unlock();
	m_pVB->Unlock();

	return true;
}

void CNavigation_Tri::Undo_Vertex()
{
	if (m_WorkHistory.begin() == m_iterWorkHistroy)
		return;

	if (m_WorkHistory.end() == m_iterWorkHistroy)
		--m_iterWorkHistroy;
	
	--m_iterWorkHistroy;
	
	m_dwVtxCnt = m_iterWorkHistroy->first;
	m_dwTriCnt = m_iterWorkHistroy->second;


}

void CNavigation_Tri::Redo_Vertex()
{
	auto iter = m_WorkHistory.end();
	--iter;
	if (iter == m_iterWorkHistroy || m_WorkHistory.end() == m_iterWorkHistroy)
		return;

	++m_iterWorkHistroy;

	m_dwVtxCnt = m_iterWorkHistroy->first;
	m_dwTriCnt = m_iterWorkHistroy->second;
}

void CNavigation_Tri::Click(const _vec2 * pClickedPos, const _vec3* pVtxPos)
{
	if (!m_bClicked)
	{
		m_bClicked = true;

		_vec3 vProjPos = {};
		CMyMath::ViewPortToProj(&vProjPos, pClickedPos, WINCX, WINCY);

		memcpy(&m_vClickPos, &vProjPos, sizeof(_vec2));
		memcpy(&m_vOldData, pVtxPos, sizeof(_vec3));
	}
}

void CNavigation_Tri::Click_Update(const _vec2 * pMousePos, const _vec3 * pMoveDir, const _ulong& dwVtxIdx, 
	const _matrix* pmatView, const _matrix* pmatProj, _vec3* pChangedPos, const _float& fCameraDist)
{
	if (m_bClicked)
	{
		_vec3 vOrigin = { 0.f, 0.f, 0.f };
		_vec3 vDir;
		D3DXVec3TransformCoord(&vOrigin, &vOrigin, pmatView);
		D3DXVec3TransformCoord(&vOrigin, &vOrigin, pmatProj);

		D3DXVec3TransformCoord(&vDir, pMoveDir, pmatView);
		D3DXVec3TransformCoord(&vDir, &vDir, pmatProj);

		_vec2 vDir2D = { vDir.x - vOrigin.x, vDir.y - vOrigin.y };
		D3DXVec2Normalize(&vDir2D, &vDir2D);

		_vec3 vMouseProj = {};
		CMyMath::ViewPortToProj(&vMouseProj, pMousePos, WINCX, WINCY);

		_vec2 vDelta2D = (_vec2)vMouseProj - m_vClickPos;
		//vDelta2D.y = -vDelta2D.y;

		_float fDot = D3DXVec2Dot(&vDir2D, &vDelta2D);
		
		VTXCOL* pVertex;
		m_pVB->Lock(0, 0, (void**)&pVertex, 0);

		pVertex[dwVtxIdx].vPos = m_vOldData + (*pMoveDir) * fDot * fCameraDist;
		if (pChangedPos)
			*pChangedPos = pVertex[dwVtxIdx].vPos;

		m_pVB->Unlock();
	}
}

void CNavigation_Tri::ClickReset()
{
	m_bClicked = false;
}

_ulong CNavigation_Tri::Get_VertexOption(const _vec3* pRayPos, const _vec3* pRayDir)
{
	_ulong dwFindIdx;

	Picking_Triangle(pRayPos, pRayDir, &dwFindIdx);
	
	return Get_VertexOption(dwFindIdx);
}

_bool CNavigation_Tri::Change_CellOption(const _vec3* pRayPos, const _vec3* pRayDir, const _ulong & dwOption)
{
	_ulong dwFindIdx;

	if (!Picking_Triangle(pRayPos, pRayDir, &dwFindIdx))
		return false;

	Change_CellOption(dwFindIdx, dwOption);

	return true;
}

void CNavigation_Tri::Refresh_VertexColor()
{
	VTXCOL* pVertex = nullptr;
	INDEX32* pIndex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	if (m_dwTriCnt != m_vecCellOption.size())
	{
		wstring wstrMsg = L"TriCnt : " + to_wstring(m_dwTriCnt);
		wstrMsg += L" CallOption Size : " + to_wstring(m_vecCellOption.size());
		MessageBox(NULL, wstrMsg.c_str(), L"Error", MB_OK);
		return;
	}

	D3DXCOLOR dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	for (_ulong i = 0; i < m_dwTriCnt; ++i)
	{
		switch (m_vecCellOption[i])
		{
		case CCell::CELL_OPTION::NO_OPTION: dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
			break;

		case CCell::CELL_OPTION::FALL: dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);
			break;
		}

		pVertex[pIndex[i]._0].dwColor = dwColor;
		pVertex[pIndex[i]._1].dwColor = dwColor;
		pVertex[pIndex[i]._2].dwColor = dwColor;
	}

	m_pVB->Unlock();
	m_pIB->Unlock();
}

void CNavigation_Tri::Render_Navigation_Tri()
{
	DWORD	dwPreFill, dwPreCull;
	m_pGraphicDev->GetRenderState(D3DRS_FILLMODE, &dwPreFill);
	m_pGraphicDev->GetRenderState(D3DRS_CULLMODE, &dwPreCull);

	if (m_bWireFrame)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	if (m_bCulling)
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	else
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	_matrix matIdentity;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, D3DXMatrixIdentity(&matIdentity));

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->SetIndices(m_pIB);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, dwPreFill);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, dwPreCull);
}

HRESULT CNavigation_Tri::SaveData(HANDLE hFile, const TCHAR* pFilePath)
{
	DWORD dwBytes = 0;

	WriteFile(hFile, &m_dwVtxSize, sizeof(_ulong), &dwBytes, nullptr);
	WriteFile(hFile, &m_dwVtxMax, sizeof(_ulong), &dwBytes, nullptr);
	WriteFile(hFile, &m_dwVtxCnt, sizeof(_ulong), &dwBytes, nullptr);
	WriteFile(hFile, &m_dwVtxFVF, sizeof(_ulong), &dwBytes, nullptr);

	WriteFile(hFile, &m_dwTriCnt, sizeof(_ulong), &dwBytes, nullptr);
	WriteFile(hFile, &m_dwTriMax, sizeof(_ulong), &dwBytes, nullptr);

	WriteFile(hFile, &m_dwIdxSize, sizeof(_ulong), &dwBytes, nullptr);

	WriteFile(hFile, &m_IdxFmt, sizeof(D3DFORMAT), &dwBytes, nullptr);

	VTXCOL* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
		WriteFile(hFile, &pVertex[i].vPos, sizeof(_vec3), &dwBytes, nullptr);

	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (_ulong i = 0; i < m_dwTriCnt; ++i)
		WriteFile(hFile, &pIndex[i], sizeof(INDEX32), &dwBytes, nullptr);
	

	if (m_vecCellOption.size() != m_dwTriCnt)
		return S_OK;

	_ulong dwOptionSize = (_ulong)m_vecCellOption.size();
	WriteFile(hFile, &dwOptionSize, sizeof(_ulong), &dwBytes, nullptr);

	for (_ulong i = 0; i < dwOptionSize; ++i)
		WriteFile(hFile, &m_vecCellOption[i], sizeof(_ulong), &dwBytes, nullptr);

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CNavigation_Tri::LoadData(HANDLE hFile, const TCHAR* wstrFilePath)
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	m_vecCellOption.clear();

	DWORD dwBytes = 0;

	ReadFile(hFile, &m_dwVtxSize, sizeof(_ulong), &dwBytes, nullptr);
	ReadFile(hFile, &m_dwVtxMax, sizeof(_ulong), &dwBytes, nullptr);
	ReadFile(hFile, &m_dwVtxCnt, sizeof(_ulong), &dwBytes, nullptr);
	ReadFile(hFile, &m_dwVtxFVF, sizeof(_ulong), &dwBytes, nullptr);

	ReadFile(hFile, &m_dwTriCnt, sizeof(_ulong), &dwBytes, nullptr);
	ReadFile(hFile, &m_dwTriMax, sizeof(_ulong), &dwBytes, nullptr);

	ReadFile(hFile, &m_dwIdxSize, sizeof(_ulong), &dwBytes, nullptr);

	ReadFile(hFile, &m_IdxFmt, sizeof(D3DFORMAT), &dwBytes, nullptr);

	Reserve_Buffer(m_dwVtxMax, m_dwTriMax);

	VTXCOL* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
	{
		ReadFile(hFile, &pVertex[i].vPos, sizeof(_vec3), &dwBytes, nullptr);
		pVertex[i].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	}

	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (_ulong i = 0; i < m_dwTriCnt; ++i)
	{
		ReadFile(hFile, &pIndex[i], sizeof(INDEX32), &dwBytes, nullptr);
	}

	m_vecCellOption.reserve(m_dwTriMax);

	_ulong dwOptionSize;
	_ulong dwOption;
	ReadFile(hFile, &dwOptionSize, sizeof(_ulong), &dwBytes, nullptr);

	if (0 != dwBytes)
	{
		for (_ulong i = 0; i < m_dwTriCnt; ++i)
		{
			ReadFile(hFile, &dwOption, sizeof(_ulong), &dwBytes, nullptr);
			if (0 != dwBytes)
				m_vecCellOption.push_back(dwOption);
		}
	}
	else
	{
		for (_ulong i = 0; i < m_dwTriCnt; ++i)
		{
			m_vecCellOption.push_back(0);
		}
	}


	m_pIB->Unlock();


	return S_OK;
}

void CNavigation_Tri::Find_NearestVertex(const VTXCOL * pVertices, _ulong * pOutIdx1, _ulong * pOutIdx2, const _vec3 * pVtxPos)
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

CNavigation_Tri * CNavigation_Tri::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNavigation_Tri* pInst = new CNavigation_Tri(pGraphicDev);

	if (FAILED(pInst->Ready_Navigation_Tri()))
		Safe_Release(pInst);
	
	return pInst;
}

void CNavigation_Tri::Free(void)
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pGraphicDev);
}


