#include "stdafx.h"
#include "AstarMgr.h"
#include "Terrain.h"

IMPLEMENT_SINGLETON(CAstarMgr)

CAstarMgr::CAstarMgr():
	m_pTerrain(nullptr),
	m_pObjectMgr(CObjectMgr::GetInstance()),
	m_bAstarProceeding(false),
	m_iStartIndex(0)
{
}

CAstarMgr::~CAstarMgr()
{
	if (!m_vecAstarTile.empty())
	{
		for_each(m_vecAstarTile.begin(), m_vecAstarTile.end(), SafeDelete<ASTAR_TILE*>);
		m_vecAstarTile.clear();
	}
}

list<ASTAR_TILE*>& CAstarMgr::GetBestPath()
{
	// TODO: insert return statement here
	return m_BestPath;
}

HRESULT CAstarMgr::ReadyAstarTile()
{
	if (!m_vecAstarTile.empty())
	{
		for_each(m_vecAstarTile.begin(), m_vecAstarTile.end(), SafeDelete<ASTAR_TILE*>);
		m_vecAstarTile.clear();
	}

	if (!m_pTerrain)
	{
		m_pTerrain = m_pObjectMgr->GetTerrain();
		NULL_CHECK_RETURN(m_pTerrain, E_FAIL);
	}

	vector<TILE_INFO*>* pvecTile = m_pTerrain->GetVecTile();
	NULL_CHECK_RETURN(pvecTile, E_FAIL);

	m_vecAstarTile.reserve(TILEX * TILEY);

	int iIndex = 0;
	for (auto pTileInfo : *pvecTile)
	{
		ASTAR_TILE* pAstarTile = new ASTAR_TILE(pTileInfo);
		pAstarTile->iMyIndex = iIndex;
		++iIndex;
		m_vecAstarTile.push_back(pAstarTile);
		
	}
	
	return S_OK;
}

HRESULT CAstarMgr::ReadyGraph()
{
	if (!m_vecGraph.empty())
		m_vecGraph.clear();

	m_vecGraph.resize(TILEX * TILEY);

	int iIndex = 0;

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			iIndex = i * TILEX + j;

			if (0 != m_vecAstarTile[iIndex]->byOption)
				continue;

			// 좌상단
			if ((0 != i) && !(0 == j && !(i % 2)))
			{
				if (i % 2 && 0 == m_vecAstarTile[iIndex - TILEX]->byOption)
					m_vecGraph[iIndex].push_back(m_vecAstarTile[iIndex - TILEX]);
				else if (!(i % 2) && 0 == m_vecAstarTile[iIndex - (TILEX + 1)]->byOption)
					m_vecGraph[iIndex].push_back(m_vecAstarTile[iIndex - (TILEX + 1)]);
			}

			// 우상단
			if ((0 != i) && !(TILEX - 1 == j && (i % 2)))
			{
				if (i % 2 && 0 == m_vecAstarTile[iIndex - (TILEX - 1)]->byOption)
					m_vecGraph[iIndex].push_back(m_vecAstarTile[iIndex - (TILEX - 1)]);
				else if (!(i % 2) && 0 == m_vecAstarTile[iIndex - TILEX]->byOption)
					m_vecGraph[iIndex].push_back(m_vecAstarTile[iIndex - TILEX]);
			}

			// 좌하단
			if ((TILEY - 1 != i) && !(0 == j && !(i % 2)))
			{
				if (i % 2 && 0 == m_vecAstarTile[iIndex + TILEX]->byOption)
					m_vecGraph[iIndex].push_back(m_vecAstarTile[iIndex + TILEX]);
				else if (!(i % 2) && 0 == m_vecAstarTile[iIndex + (TILEX - 1)]->byOption)
					m_vecGraph[iIndex].push_back(m_vecAstarTile[iIndex + (TILEX - 1)]);
			}

			// 우하단
			if ((TILEY - 1 != i) && !(TILEX - 1 == j && (i % 2)))
			{
				if (i % 2 && 0 == m_vecAstarTile[iIndex + (TILEX + 1)]->byOption)
					m_vecGraph[iIndex].push_back(m_vecAstarTile[iIndex + (TILEX + 1)]);
				else if (!(i % 2) && 0 == m_vecAstarTile[iIndex + TILEX]->byOption)
					m_vecGraph[iIndex].push_back(m_vecAstarTile[iIndex + TILEX]);
			}
		}
	}


	return S_OK;
}

void CAstarMgr::StartAstar(const D3DXVECTOR3 & vStartPos, const D3DXVECTOR3 & vGoalPos)
{
	m_bAstarProceeding = true;

	if (m_vecAstarTile.empty())
	{
		m_bAstarProceeding = false;
		return;
	}

	m_OpenList.clear();
	m_CloseList.clear();

	for (auto pAstarTile : m_vecAstarTile)
		pAstarTile->iCost = 1;
	
	m_BestPath.clear();

	m_iStartIndex = m_pTerrain->GetTileIndex(vStartPos);
	int iGoalIndex = m_pTerrain->GetTileIndex(vGoalPos);
	m_iGoalIndex = iGoalIndex;
	if (-1 == m_iStartIndex || -1 == iGoalIndex)
	{
		m_bAstarProceeding = false;
		return;
	}

	if (m_iStartIndex == iGoalIndex)
	{
		m_bAstarProceeding = false;
		return;
	}

	if (0 != m_vecAstarTile[iGoalIndex]->byOption)
	{
		m_bAstarProceeding = false;
		return;
	}

	if (PathFinding(m_iStartIndex, iGoalIndex))
		MakePath(m_iStartIndex, iGoalIndex);

	m_bAstarProceeding = false;
}

bool CAstarMgr::PathFinding(int iStartIndex, int iGoalIndex)
{
	if (!m_OpenList.empty())
		m_OpenList.pop_front();

	m_CloseList.push_back(iStartIndex);

	for (auto pTile : m_vecGraph[iStartIndex])
	{
		if (iGoalIndex == pTile->iMyIndex)
		{
			pTile->iParentIndex = iStartIndex;
			return true;
		}

		if (!IsInOpenlist(pTile->iMyIndex) && !IsInCloselist(pTile->iMyIndex))
		{
			pTile->iParentIndex = iStartIndex;
			pTile->iCost += m_vecAstarTile[iStartIndex]->iCost;
			m_OpenList.push_back(pTile->iMyIndex);
		}
	}

	if (m_OpenList.empty()) // 더이상 찾을 길이 없다.
		return false;

	// 휴리스틱 비용을 조사하고 난 후에 오픈 리스트를 비용이 적은 쪽으로 정렬.
	m_OpenList.sort(
		[&](int iPreIndex, int iNextIndex)
	{
		// 휴리스틱 = 최초 출발점과의 거리 + 최종 도착점과의 거리
		D3DXVECTOR3 v1 = m_vecAstarTile[m_iStartIndex]->vPos	- m_vecAstarTile[iPreIndex]->vPos;
		D3DXVECTOR3 v2 = m_vecAstarTile[iGoalIndex]->vPos		- m_vecAstarTile[iPreIndex]->vPos;
		float fHueristikA = (D3DXVec3Length(&v1) + D3DXVec3Length(&v2)) * m_vecAstarTile[iPreIndex]->iCost;

		D3DXVECTOR3 v3 = m_vecAstarTile[m_iStartIndex]->vPos - m_vecAstarTile[iNextIndex]->vPos;
		D3DXVECTOR3 v4 = m_vecAstarTile[iGoalIndex]->vPos - m_vecAstarTile[iNextIndex]->vPos;
		float fHueristikB = (D3DXVec3Length(&v3) + D3DXVec3Length(&v4)) * m_vecAstarTile[iNextIndex]->iCost;

		return fHueristikA < fHueristikB;
	});

	return PathFinding(m_OpenList.front(), iGoalIndex);
}

void CAstarMgr::MakePath(int iStartIndex, int iGoalIndex)
{
	m_BestPath.push_front(m_vecAstarTile[iGoalIndex]);
	int iParentIndex = m_vecAstarTile[iGoalIndex]->iParentIndex;

	while (true)
	{
		if (iParentIndex == iStartIndex)
			break;

		m_BestPath.push_front(m_vecAstarTile[iParentIndex]);
		iParentIndex = m_vecAstarTile[iParentIndex]->iParentIndex;
	}
}

bool CAstarMgr::IsInOpenlist(int iIndex)
{
	// find 함수: <algorithm>에서 제공.
	// 현재 컨테이너를 순회하면서 세번째 인자로 전달 받은 값이 존재하는지 탐색하는 함수.
	auto iter_find = find(m_OpenList.begin(), m_OpenList.end(), iIndex);

	if (m_OpenList.end() == iter_find)
		return false;

	return true;
}

bool CAstarMgr::IsInCloselist(int iIndex)
{
	auto iter_find = find(m_CloseList.begin(), m_CloseList.end(), iIndex);

	if (m_CloseList.end() == iter_find)
		return false;

	return true;
}
