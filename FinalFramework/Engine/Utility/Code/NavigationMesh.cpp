#include "NavigationMesh.h"

USING(Engine)

Engine::CNavigationMesh::CNavigationMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMesh(pGraphicDev)
	, m_dwIndex(0)
{

}

Engine::CNavigationMesh::CNavigationMesh(const CNavigationMesh& rhs)
	: CMesh(rhs)
	, m_vecCell(rhs.m_vecCell)
	, m_dwIndex(rhs.m_dwIndex)	
{
	for (auto& iter : m_vecCell)
		iter->AddRef();
}

Engine::CNavigationMesh::~CNavigationMesh(void)
{

}

HRESULT CNavigationMesh::Ready_Meshes(const wstring & wstrFilePath, const wstring & wstrFileName)
{
	_tchar szFullPath[256] = L"";

	lstrcpy(szFullPath, wstrFilePath.c_str());
	lstrcat(szFullPath, wstrFileName.c_str());

	HANDLE hFile = ::CreateFile(szFullPath, GENERIC_READ, 0, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwBytes = 0;
	DWORD dwVtxSize, dwVtxMax, dwVtxCnt,dwVtxFVF, dwTriCnt, dwTriMax, dwIdxSize;
	D3DFORMAT IdxFmt;

	ReadFile(hFile, &dwVtxSize, sizeof(_ulong), &dwBytes, nullptr);
	ReadFile(hFile, &dwVtxMax, sizeof(_ulong), &dwBytes, nullptr);
	ReadFile(hFile, &dwVtxCnt, sizeof(_ulong), &dwBytes, nullptr);
	ReadFile(hFile, &dwVtxFVF, sizeof(_ulong), &dwBytes, nullptr);

	ReadFile(hFile, &dwTriCnt, sizeof(_ulong), &dwBytes, nullptr);
	ReadFile(hFile, &dwTriMax, sizeof(_ulong), &dwBytes, nullptr);

	ReadFile(hFile, &dwIdxSize, sizeof(_ulong), &dwBytes, nullptr);

	ReadFile(hFile, &IdxFmt, sizeof(D3DFORMAT), &dwBytes, nullptr);

	_vec3* pPoint = new _vec3[dwVtxCnt];
	ReadFile(hFile, pPoint, sizeof(_vec3) * dwVtxCnt, &dwBytes, nullptr);

	INDEX32* pIndex = new INDEX32[dwTriCnt];
	ReadFile(hFile, pIndex, sizeof(INDEX32) * dwTriCnt, &dwBytes, nullptr);

	_ulong dwOptionSize;
	ReadFile(hFile, &dwOptionSize, sizeof(_ulong), &dwBytes, nullptr);

	_ulong* pOptions = nullptr;
	if (0 != dwBytes)
	{
		pOptions = new _ulong[dwOptionSize];
		ReadFile(hFile, pOptions, sizeof(_ulong) * dwOptionSize, &dwBytes, nullptr);
	}

	CloseHandle(hFile);

	m_vecCell.reserve(dwTriCnt);

	CCell* pCell = nullptr;

	_ulong dwOption = 0;
	for (_uint i = 0; i < dwTriCnt; ++i)
	{
		dwOption = pOptions ? pOptions[i] : 0;

		pCell = CCell::Create(m_pGraphicDev, (_ulong)m_vecCell.size(), &pPoint[pIndex[i]._0], &pPoint[pIndex[i]._1], &pPoint[pIndex[i]._2], (CCell::CELL_OPTION)dwOption);
		NULL_CHECK_RETURN(pCell, E_FAIL);
		m_vecCell.push_back(pCell);
	}

	Safe_Delete_Array(pPoint);
	Safe_Delete_Array(pIndex);
	Safe_Delete_Array(pOptions);

	FAILED_CHECK_RETURN(Link_Cell(), E_FAIL);

	return S_OK;
}

void CNavigationMesh::Render_Meshes(void)
{

}

HRESULT Engine::CNavigationMesh::Ready_NavigationMesh(void)
{
	m_vecCell.reserve(4);

	CCell*		pCell = nullptr;

	// 0
	pCell = CCell::Create(m_pGraphicDev, (_ulong)m_vecCell.size(),  
						  &_vec3(0.f, 0.f, 2.f), &_vec3(2.f, 0.f, 0.f), &_vec3(0.f, 0.f, 0.f));
	NULL_CHECK_RETURN(pCell, E_FAIL);
	m_vecCell.push_back(pCell);

	// 1
	pCell = CCell::Create(m_pGraphicDev, (_ulong)m_vecCell.size(),
						  &_vec3(0.f, 0.f, 2.f), &_vec3(2.f, 0.f, 2.f), &_vec3(2.f, 0.f, 0.f));
	NULL_CHECK_RETURN(pCell, E_FAIL);
	m_vecCell.push_back(pCell);

	// 2
	pCell = CCell::Create(m_pGraphicDev, (_ulong)m_vecCell.size(),
						  &_vec3(0.f, 0.f, 4.f), &_vec3(2.f, 0.f, 2.f), &_vec3(0.f, 0.f, 2.f));
	NULL_CHECK_RETURN(pCell, E_FAIL);
	m_vecCell.push_back(pCell);

	// 3
	pCell = CCell::Create(m_pGraphicDev, (_ulong)m_vecCell.size(), 
						  &_vec3(2.f, 0.f, 2.f), &_vec3(4.f, 0.f, 0.f), &_vec3(2.f, 0.f, 0.f));
	NULL_CHECK_RETURN(pCell, E_FAIL);
	m_vecCell.push_back(pCell);

	FAILED_CHECK_RETURN(Link_Cell(), E_FAIL);

	return S_OK;
}

void CNavigationMesh::Render_NavigationMesh(void)
{
	for (auto& iter : m_vecCell)
		iter->Render_Cell();

}

_vec3 CNavigationMesh::Move_OnNavigationMesh(const _vec3 * pTargetPos, const _vec3 * pTargetDir)
{
	_vec3	vEndPos = *pTargetPos + *pTargetDir;

	if (CCell::COMPARE_MOVE == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
		return vEndPos;

	else if (CCell::COMPARE_STOP == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
		return *pTargetPos;

	return *pTargetPos;
}

_vec3 CNavigationMesh::Move_OnNavigationMesh(const _vec3 * pTargetPos, const _vec3 * pTargetDir, _ulong* dwOption)
{
	_vec3 vOut = {};

	switch (m_vecCell[m_dwIndex]->Compare(pTargetPos, pTargetDir, &m_dwIndex, &vOut))
	{
	case CCell::COMPARE_MOVE:
		return vOut;

	case CCell::COMPARE_STOP:
		return *pTargetPos;

	case CCell::COMPARE_SLIDING:
		return vOut;
	}

	if (m_vecCell[m_dwIndex]->Get_Option() == CCell::NO_MOVE)
	{
		m_dwIndex = Get_NearIndex(CCell::NO_OPTION);
		m_vecCell[m_dwIndex]->Get_Center(&vOut);
		return vOut;
	}
	
	

	return *pTargetPos;
}

_ulong CNavigationMesh::Get_Index(const _vec3 * pPos)
{
	queue<_ulong> qIndices;

	_bool* pVisit = new _bool[m_vecCell.size()];
	ZeroMemory(pVisit, sizeof(_bool) * m_vecCell.size());

	qIndices.push(m_dwIndex);

	_ulong dwRet = 0;
	while (!qIndices.empty())
	{
		_ulong dwIdx = qIndices.front();
		qIndices.pop();

		if (m_vecCell[dwIdx]->IsInside(pPos))
		{
			dwRet = dwIdx;
			break;
		}

		pVisit[dwIdx] = true;

		_ulong dwNeighborIdx = 0;
		if (m_vecCell[dwIdx]->Get_Neighbor(CCell::NEIGHBOR_AB))
		{
			dwNeighborIdx = *m_vecCell[dwIdx]->Get_Neighbor(CCell::NEIGHBOR_AB)->Get_Index();
			if (!pVisit[dwNeighborIdx])
				qIndices.push(dwNeighborIdx);
		}

		if (m_vecCell[dwIdx]->Get_Neighbor(CCell::NEIGHBOR_BC))
		{
			dwNeighborIdx = *m_vecCell[dwIdx]->Get_Neighbor(CCell::NEIGHBOR_BC)->Get_Index();
			if (!pVisit[dwNeighborIdx])
				qIndices.push(dwNeighborIdx);
		}

		if (m_vecCell[dwIdx]->Get_Neighbor(CCell::NEIGHBOR_CA))
		{
			dwNeighborIdx = *m_vecCell[dwIdx]->Get_Neighbor(CCell::NEIGHBOR_CA)->Get_Index();
			if (!pVisit[dwNeighborIdx])
				qIndices.push(dwNeighborIdx);
		}
	}

	Safe_Delete_Array(pVisit);

	return dwRet;
}

_ulong CNavigationMesh::Get_NearIndex(CCell::CELL_OPTION eOption) const
{
	queue<_ulong> qIndices;
	
	set<_ulong> setVisit;

	qIndices.push(m_dwIndex);

	while (!qIndices.empty())
	{
		_ulong dwIdx = qIndices.front();
		qIndices.pop();

		if (m_vecCell[dwIdx]->Get_Option() == eOption)
			return dwIdx;

		setVisit.insert(dwIdx);

		_ulong dwNeighborIdx = 0;
		if (m_vecCell[dwIdx]->Get_Neighbor(CCell::NEIGHBOR_AB))
		{
			dwNeighborIdx = *m_vecCell[dwIdx]->Get_Neighbor(CCell::NEIGHBOR_AB)->Get_Index();
			if (setVisit.find(dwNeighborIdx) == setVisit.end())
				qIndices.push(dwNeighborIdx);
		}

		if (m_vecCell[dwIdx]->Get_Neighbor(CCell::NEIGHBOR_BC))
		{
			dwNeighborIdx = *m_vecCell[dwIdx]->Get_Neighbor(CCell::NEIGHBOR_BC)->Get_Index();
			if (setVisit.find(dwNeighborIdx) == setVisit.end())
				qIndices.push(dwNeighborIdx);
		}
		
		if (m_vecCell[dwIdx]->Get_Neighbor(CCell::NEIGHBOR_CA))
		{
			dwNeighborIdx = *m_vecCell[dwIdx]->Get_Neighbor(CCell::NEIGHBOR_CA)->Get_Index();
			if (setVisit.find(dwNeighborIdx) == setVisit.end())
				qIndices.push(dwNeighborIdx);
		}
	}

	return 0;
}

_ulong CNavigationMesh::Get_NearIndex(CCell::CELL_OPTION eOption, const _ulong & dwIndex) const
{
	queue<_ulong> qIndices;

	set<_ulong> setVisit;

	qIndices.push(dwIndex);

	while (!qIndices.empty())
	{
		_ulong dwIdx = qIndices.front();
		qIndices.pop();

		if (m_vecCell[dwIdx]->Get_Option() == eOption)
			return dwIdx;

		setVisit.insert(dwIdx);

		_ulong dwNeighborIdx = 0;
		if (m_vecCell[dwIdx]->Get_Neighbor(CCell::NEIGHBOR_AB))
		{
			dwNeighborIdx = *m_vecCell[dwIdx]->Get_Neighbor(CCell::NEIGHBOR_AB)->Get_Index();
			if (setVisit.find(dwNeighborIdx) == setVisit.end())
				qIndices.push(dwNeighborIdx);
		}

		if (m_vecCell[dwIdx]->Get_Neighbor(CCell::NEIGHBOR_BC))
		{
			dwNeighborIdx = *m_vecCell[dwIdx]->Get_Neighbor(CCell::NEIGHBOR_BC)->Get_Index();
			if (setVisit.find(dwNeighborIdx) == setVisit.end())
				qIndices.push(dwNeighborIdx);
		}

		if (m_vecCell[dwIdx]->Get_Neighbor(CCell::NEIGHBOR_CA))
		{
			dwNeighborIdx = *m_vecCell[dwIdx]->Get_Neighbor(CCell::NEIGHBOR_CA)->Get_Index();
			if (setVisit.find(dwNeighborIdx) == setVisit.end())
				qIndices.push(dwNeighborIdx);
		}
	}

	return 0;
}

_ulong CNavigationMesh::Get_NearIndex(CCell::CELL_OPTION eOption, const _vec3 * pPos)
{
	_ulong dwIndex = Get_Index(pPos);

	return Get_NearIndex(eOption, dwIndex);
}

_float CNavigationMesh::Get_Height(const _vec3 * pPos) const
{
	return m_vecCell[m_dwIndex]->Get_Height(pPos);
}

CCell::CELL_OPTION CNavigationMesh::Get_Option(const _vec3 * pPos)
{
	return m_vecCell[Get_Index(pPos)]->Get_Option();
}

_bool CNavigationMesh::Get_AdjacentIndex(CCell::CELL_OPTION eOption, _ulong* pOut)
{
	return m_vecCell[m_dwIndex]->Get_AdjacentIndex(eOption, pOut);
}

HRESULT Engine::CNavigationMesh::Link_Cell(void)
{
	size_t	dwSize = m_vecCell.size();

	for (_ulong i = 0; i < dwSize; ++i)
	{
		for (_ulong j = 0; j < dwSize; ++j)
		{
			if(i == j)
				continue;

			if(nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_AB) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_A), 
													m_vecCell[i]->Get_Point(CCell::POINT_B), 
													m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_AB, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_BC) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_B),
													m_vecCell[i]->Get_Point(CCell::POINT_C),
													m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_BC, m_vecCell[j]);
				continue;
			}


			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_CA) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_C),
													m_vecCell[i]->Get_Point(CCell::POINT_A),
													m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_CA, m_vecCell[j]);
				continue;
			}
		}
	}

	return S_OK;
}

Engine::CNavigationMesh* Engine::CNavigationMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNavigationMesh*	pInstance = new CNavigationMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_NavigationMesh()))
		Safe_Release(pInstance);

	return pInstance;
}

CNavigationMesh * CNavigationMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrFilePath, const wstring & wstrFileName)
{
	CNavigationMesh*	pInstance = new CNavigationMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(wstrFilePath, wstrFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CResources* Engine::CNavigationMesh::Clone(void)
{
	return new CNavigationMesh(*this);
}

void Engine::CNavigationMesh::Free(void)
{
	for_each(m_vecCell.begin(), m_vecCell.end(), CDeleteObj());
	m_vecCell.clear();

	CMesh::Free();
}

