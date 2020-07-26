#ifndef Navigation_Tri_h__
#define Navigation_Tri_h__

#include "Base.h"

using namespace Engine;

class CNavigation_Tri : public CBase
{
private:
	typedef struct tWorkHistory 
	{
		_ulong dwVtxCnt;
		_ulong dwTriCnt;
	}WORK_HISTORY;

private:
	explicit CNavigation_Tri(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNavigation_Tri();

private:
	HRESULT		Ready_Navigation_Tri();
	HRESULT		Ready_Buffer(const _ulong& dwVtxMax, const _ulong& dwTriMax, LPDIRECT3DVERTEXBUFFER9* ppVB, LPDIRECT3DINDEXBUFFER9* ppIB);
	HRESULT		Reserve_Buffer(const _ulong& dwVtxMax, const _ulong& dwTriMax);

public:
	_bool	Picking_Vertex(const _vec3* pPos, _ulong* dwOutIdx, _vec3* pOutPos = nullptr);
	_bool	Picking_Vertex(const _vec2* pMousePos, const _matrix* pmatView, const _matrix* pmatProj, _ulong* dwOutIdx, _vec3* pOutPos = nullptr);

	_bool	Picking_Triangle(const _vec3* pRayPos, const _vec3* pRayDir, _ulong* dwOutIdx);

	void	Set_Y(const _ulong& dwVtxIdx, const _float& fy);

	void	Change_VertexColor(const _ulong& dwVtxIdx, D3DXCOLOR d3dColor);
	void	Change_VertexPosY(const _ulong& dwVtxIdx, const _float& fY);
	
	_ulong	Add_Vertex(const _vec3* pVtxPos);
	_ulong	Add_Vertex(const vector<_vec3>* pvecVertices);
	_ulong	Add_Vertex(const _vec3* pVtxPos, const _ulong& dwPickedIdx1, const _ulong& dwPickedIdx2);

	_bool	Remove_Vertex(const _ulong& dwIndex);
	
	_ulong	Connect_Vertex(const _ulong& dwConnectIdx, const _ulong& dwPickedIdx1, const _ulong& dwPickedIdx2);
	_bool	Disunite_Vertex(const _ulong& dwDisuniteIdx, const _ulong& dwPickedIdx1, const _ulong& dwPickedIdx2);

	void	Undo_Vertex();
	void	Redo_Vertex();

	void	Click(const _vec2* pClickedPos, const _vec3* pVtxPos);
	void	Click_Update(const _vec2* pMousePos, const _vec3* pMoveDir, const _ulong& dwVtxIdx, 
		const _matrix* pmatView, const _matrix* pmatProj, _vec3* pChangedPos = nullptr, const _float& fCameraDist = 1.f);
	void	ClickReset();

	void	Set_WireFrame(_bool bWireFrame) { m_bWireFrame = bWireFrame; }
	void	Set_Culling(_bool bCulling) { m_bCulling = bCulling; }

	_ulong	Get_VertexOption(const _ulong& dwIndex) { return m_vecCellOption[dwIndex]; }
	_ulong	Get_VertexOption(const _vec3* pRayPos, const _vec3* pRayDir);

	void	Change_CellOption(const _ulong& dwIndex, const _ulong& dwOption) { m_vecCellOption[dwIndex] = dwOption; }
	_bool	Change_CellOption(const _vec3* pRayPos, const _vec3* pRayDir, const _ulong& dwOption);

	void	Refresh_VertexColor();

	void	Render_Navigation_Tri();

	HRESULT SaveData(HANDLE hFile, const TCHAR* pFilePath);
	HRESULT LoadData(HANDLE hFile, const TCHAR* pFilePath);

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

	_ulong	m_dwTriCnt = 0;
	_ulong	m_dwTriMax;

	_ulong	m_dwIdxSize;
	D3DFORMAT	m_IdxFmt;

	vector<_uint>				m_vecCellOption;
	list<pair<_ulong, _ulong> > m_WorkHistory;
	list<pair<_ulong, _ulong> >::iterator m_iterWorkHistroy;

	_float	m_fPickingRadius = 1.f;

	_bool	m_bWireFrame	= true;
	_bool	m_bCulling		= true;

	_bool	m_bClicked = false;
	
	_vec2	m_vClickPos = { 0.f, 0.f };
	_vec3	m_vOldData	= { 0.f, 0.f, 0.f };

public:
	static CNavigation_Tri* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual void Free(void) override;
};


#endif // Navigation_Tri_h__
