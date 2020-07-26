#pragma once
#include "afxwin.h"


// CNaviTool dialog
class CToolView;
class CNaviTool : public CDialog
{
	DECLARE_DYNAMIC(CNaviTool)

public:
	CNaviTool(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNaviTool();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAVITOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CToolView* m_pToolView = nullptr;

	CButton m_AddVertex;
	CButton m_ModifyVertex;
	CButton m_Auto;
	CButton m_ConnectVertex;
	CButton m_AddFromPicked;
	CButton m_NaviWireFrame;
	CButton m_NaviCulling;
	CButton m_AllNew;
	CButton m_ChangeOption;
	CButton m_NoOption;
	CButton m_Fall;
	float	m_fVertexY;

	vector<D3DXVECTOR3> m_vecVertexPos;

	DWORD Get_ChangeOption();
	

	afx_msg void OnBnClickedAddVertex();
	afx_msg void OnBnClickedModifyVertex();
	afx_msg void OnBnClickedWireFrame();
	afx_msg void OnBnClickedCulling();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedDisunite();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedColorRefresh();
	afx_msg void OnBnClickedChangeOption();
};
