#pragma once
class CAstarMgr
{
	DECLARE_SINGLETON(CAstarMgr)

private:
	CAstarMgr();
	~CAstarMgr();

public:
	list<ASTAR_TILE*>& GetBestPath();

public:
	HRESULT	ReadyAstarTile();
	HRESULT ReadyGraph();
	void StartAstar(const D3DXVECTOR3& vStartPos, const D3DXVECTOR3& vGoalPos);

private:
	bool PathFinding(int iStartIndex, int iGoalIndex);
	void MakePath(int iStartIndex, int iGoalIndex);
	bool IsInOpenlist(int iIndex);
	bool IsInCloselist(int iIndex);

public:
	const bool AstarProceeding()	const { return m_bAstarProceeding; }
	const int GetGoalIndex()		const { return m_iGoalIndex; }

private:
	vector<ASTAR_TILE*>		m_vecAstarTile;
	vector<list<ASTAR_TILE*>>	m_vecGraph;

	list<int>			m_OpenList;
	list<int>			m_CloseList;
	list<ASTAR_TILE*>	m_BestPath; // 만들어진 경로	

	CObjectMgr*			m_pObjectMgr;
	CTerrain*			m_pTerrain;

	int					m_iStartIndex;
	int					m_iGoalIndex;

	bool				m_bAstarProceeding;
};

