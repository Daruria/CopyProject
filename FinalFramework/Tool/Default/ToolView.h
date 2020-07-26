
// ToolView.h : interface of the CToolView class
//

#pragma once

class CToolDoc;
class CDynamicCamera;
class COriginGrid;
class CNavigation;
class CNavigation_Tri;
class CNaviTool;
class CObjectTool;
class CColliderTool;
class CMyForm;

enum TOOL_MODE { NAVI_TOOL, OBJECT_TOOL, COLLIDER_TOOL, TOOL_END };

enum LEFT_CLICK_FUNC	{ ADD_VERTEX, MODIFY_VERTEX, MOVE_OBJECT, CHANGE_CELL_OPTION, LEFT_CLICK_FUNC_END };
enum RIGHT_CLICK_FUNC	{ PICK_VERTEX, PICK_OBJECT, RIGHT_CLICK_FUNC_END };

namespace Engine
{
	class CStaticObject;
}
using namespace Engine;

class CToolView : public CView
{
protected: // create from serialization only
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// Attributes
public:
	CToolDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	Engine::CGraphicDev*	m_pDeviceClass;
	Engine::CManagement*	m_pManagement = nullptr;
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	CDynamicCamera*			m_pCamera;
	COriginGrid*			m_pOriginGrid;
	//CNavigation*			m_pNavigation;
	CNavigation_Tri*		m_pNavigation;
	CMyForm*				m_pMyForm;
	CNaviTool*				m_pNaviTool = nullptr;
	CObjectTool*			m_pObjectTool = nullptr;
	CColliderTool*			m_pColliderTool = nullptr;

	bool					m_bCtrl = false;
	bool					m_bShift = false;
	bool					m_bAlt = false;
	bool					m_bLButton = false;
	bool					m_bMButton = false;
	bool					m_bRButton = false;

	TOOL_MODE				m_eToolMode		= NAVI_TOOL;
	LEFT_CLICK_FUNC			m_eLeftFunc		= LEFT_CLICK_FUNC_END;
	RIGHT_CLICK_FUNC		m_eRightFunc	= RIGHT_CLICK_FUNC_END;

	deque<pair<DWORD, D3DXVECTOR3> >		m_qPickPair;

	LPD3DXMESH				m_pSphere;

	D3DXMATRIX				m_matAxisLines;
	class CAxisLines*		m_pAxisLines = nullptr;
	
	_bool					m_bObjWireFrame = false;
	_bool					m_bObjectPicked = false;
	list<CStaticObject*>	m_ObjectList;
	CStaticObject*			m_pPickedObject = nullptr;
	D3DXVECTOR2				m_vClickedPos = {};
	D3DXVECTOR3				m_vObjOldData = {};

	//	Collider Tool
	_bool					m_bWireFrame = false;
	CGameObject*			m_pCollToolObject = nullptr;
	CCollider*				m_pCollider = nullptr;

public:
	void	Disunite_Vertex();

private:
	void	Render_Scene();
	void	Draw_PickVertex();
	void	Draw_AxisSphere();
	void	Pick_Axis(const CPoint* pMousePoint);
	void	Move_Axis(const CPoint* pMousePoint);
	void	Move_Object(const CPoint* pMousePoint);
	void	Rotation_Object(const CPoint* pMousePoint);
	void	Scale_Object(const CPoint* pMousePoint);
	

	void	Add_Vertex(const CPoint* pMousePoint);
	void	Connect_Vertex(const CPoint* pMousePoint);
	void	Remove_Vertex();
	void	Remove_Object();

	void	Pick_VertexOnPlane(const CPoint* pMousePoint, D3DXPLANE* pPlane);
	void	Pick_VertexOnProj(const CPoint* pMousePoint);
	void	Add_PickPair(WORD wPickIdx, const D3DXVECTOR3& vIdxPos);

	void	Render_Objects();
	void	Pick_Object(const CPoint* pMousePoint);
	void	Update_ObjectToolVariable();
	void	Refresh_MatAxisFromPickedObject();

	void	Change_CellOption(const _ulong& dwCellIdx, const _ulong& dwCellOption);
	void	Change_CellOption(const CPoint* pMousePoint, const _ulong& dwCellOption);


public:	//	Collider Tool
	void	Render_ColliderTool();
	void	Render_Bones();

public:
	virtual void OnInitialUpdate();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in ToolView.cpp
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

