#pragma once



// CMyForm form view
#include "MapTool.h"
#include "PathTool.h"

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
	virtual void OnInitialUpdate();

public:
	afx_msg void OnBnClickedMapTool();
	afx_msg void OnBnClickedPathTool();

public:
	CMapTool	m_MapTool;
	CPathTool	m_PathTool;
	
};


