#ifndef Cell_h__
#define Cell_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CLine;

class ENGINE_DLL CCell : public CBase
{
public:
	enum POINT		{ POINT_A, POINT_B, POINT_C, POINT_END };
	enum NEIGHBOR	{ NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END };
	enum LINE		{ LINE_AB, LINE_BC, LINE_CA, LINE_END };
	enum COMPARE	{ COMPARE_MOVE, COMPARE_STOP, COMPARE_SLIDING, COMPARE_FALL };
	enum CELL_OPTION { NO_OPTION, FALL, NO_MOVE, CELL_OPTION_END };

private:
	explicit CCell(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCell(void);

public:
	const _ulong*		Get_Index(void)						{ return &m_dwIndex; }
	const _vec3*		Get_Point(POINT eType) const		{ return &m_vPoint[eType]; }
	CCell*				Get_Neighbor(NEIGHBOR eType) const	{ return m_pNeighbor[eType]; }
	void				Set_Neighbor(NEIGHBOR eType, CCell* pNeighbor) { m_pNeighbor[eType] = pNeighbor; }
	void				Get_Center(_vec3* pOut) const;
	_float				Get_Height(const _vec3* pPos);
	CELL_OPTION			Get_Option() const					{ return m_eOption; }
	_bool				Get_AdjacentIndex(CELL_OPTION eOption, _ulong* pOut);
	_ulong				Get_NeighborIdx(NEIGHBOR eNeighbor) const { return *m_pNeighbor[eNeighbor]->Get_Index(); }
	
	void				Change_Option(CELL_OPTION eOption) { m_eOption = eOption; }

public:
	HRESULT		Ready_Cell(	const _ulong& dwIndex,
							const _vec3* pPointA,
							const _vec3* pPointB,
							const _vec3* pPointC,
							CELL_OPTION eOption = NO_OPTION);

	_bool		Compare_Point(const _vec3* pPointF, 
							  const _vec3* pPointS, 
							  CCell* pCell);

	void		Render_Cell(void);

	_bool		IsInside(const _vec3* pPos);
	COMPARE		Compare(const _vec3* pEndPos, _ulong* pIndex);
	COMPARE		Compare(const _vec3* pTargetPos, const _vec3* pTargetDir, _ulong* pIndex, _vec3* pOut);


private:
	_vec3		m_vPoint[POINT_END];
	CCell*		m_pNeighbor[NEIGHBOR_END];
	CLine*		m_pLine[LINE_END];
	LPD3DXLINE	m_pD3DXLine;
	_ulong		m_dwIndex;
	CELL_OPTION	m_eOption = NO_OPTION;
	LPDIRECT3DDEVICE9 m_pGraphicDev;

public:
	static CCell*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC, CELL_OPTION eOption = NO_OPTION);
	virtual void		Free(void);		
};

END
#endif // Cell_h__
