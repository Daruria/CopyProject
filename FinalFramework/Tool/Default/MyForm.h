#pragma once



// CMyForm form view
#include "NaviTool.h"
#include "ObjectTool.h"
#include "ColliderTool.h"
#include "afxwin.h"

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // protected constructor used by dynamic creation
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedNaviTool();
	afx_msg void OnBnClickedObjectTool();
	afx_msg void OnBnClickedColliderTool();
	virtual void OnInitialUpdate();

public:
	CNaviTool	m_NaviTool;
	CObjectTool m_ObjectTool;
	CColliderTool m_ColliderTool;
	CButton m_NaviRadio;
	CButton m_ObjectRadio;
	CButton m_ColliderRadio;
	afx_msg void OnBnClickedNaviRadio();
	afx_msg void OnBnClickedObjectRadio();
	afx_msg void OnBnClickedCollRadio();
};


