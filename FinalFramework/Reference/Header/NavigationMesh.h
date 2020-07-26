#ifndef NavigationMesh_h__
#define NavigationMesh_h__

#include "Mesh.h"
#include "Cell.h"

BEGIN(Engine)

class ENGINE_DLL CNavigationMesh : public CMesh
{
private:
	explicit CNavigationMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNavigationMesh(const CNavigationMesh& rhs);
	virtual ~CNavigationMesh(void);

public:
	void		Set_Index(const _ulong& dwIndex) { m_dwIndex = dwIndex; }

public:
	virtual HRESULT		Ready_Meshes(const wstring& wstrFilePath, const wstring& wstrFileName);
	virtual void		Render_Meshes(void);
	virtual void		Render_Meshes(LPD3DXEFFECT& pEffect) {}

	HRESULT		Ready_NavigationMesh(void);
	void		Render_NavigationMesh(void);

public:
	_vec3		Move_OnNavigationMesh(const _vec3* pTargetPos, const _vec3* pTargetDir);
	_vec3		Move_OnNavigationMesh(const _vec3* pTargetPos, const _vec3* pTargetDir, _ulong* dwOption);
	
	_ulong				Get_Index() const { return m_dwIndex; }
	_ulong				Get_Index(const _vec3* pPos);
	void				Get_Center(const _ulong& dwIndex, _vec3* pOut) const { m_vecCell[dwIndex]->Get_Center(pOut); }
	void				Get_CurIdxCenter(_vec3* pOut) const { m_vecCell[m_dwIndex]->Get_Center(pOut); }
	_ulong				Get_NearIndex(CCell::CELL_OPTION eOption) const;
	_ulong				Get_NearIndex(CCell::CELL_OPTION eOption, const _ulong& dwIndex) const;
	_ulong				Get_NearIndex(CCell::CELL_OPTION eOption, const _vec3* pPos);

	_float				Get_Height(const _vec3* pPos) const;
	CCell::CELL_OPTION	Get_Option() const { return m_vecCell[m_dwIndex]->Get_Option(); }
	CCell::CELL_OPTION	Get_Option(const _vec3* pPos);
	_bool				Get_AdjacentIndex(CCell::CELL_OPTION eOption, _ulong* pOut);


	void Change_CellOption(const _ulong& dwIndex, CCell::CELL_OPTION eOption) { m_vecCell[dwIndex]->Change_Option(eOption);	}

private:
	HRESULT		Link_Cell(void);

private:
	vector<CCell*>			m_vecCell;
	_ulong					m_dwIndex;

public:
	static	CNavigationMesh*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static	CNavigationMesh*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrFilePath, const wstring& wstrFileName);
	virtual CResources*			Clone(void);
	virtual void				Free(void);
};

END
#endif // NavigationMesh_h__
