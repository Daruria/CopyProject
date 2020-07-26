// MyForm.cpp : implementation file
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyForm::OnBnClickedMapTool)
	ON_BN_CLICKED(IDC_BUTTON5, &CMyForm::OnBnClickedPathTool)
END_MESSAGE_MAP()


// CMyForm diagnostics

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm message handlers


void CMyForm::OnBnClickedMapTool()
{
	// TODO: Add your control notification handler code here

	NULL_CHECK_MSG(m_MapTool.GetSafeHwnd(), L"MapTool is null");
	m_MapTool.ShowWindow(SW_SHOW); // SW_SHOW: 창모양으로 출력.
}


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();


	m_MapTool.Create(IDD_MAPTOOL);
	m_PathTool.Create(IDD_PATHTOOL);


	// TODO: Add your specialized code here and/or call the base class
}


void CMyForm::OnBnClickedPathTool()
{
	// TODO: Add your control notification handler code here

	NULL_CHECK_MSG(m_PathTool.GetSafeHwnd(), L"PathTool is null");
	m_PathTool.ShowWindow(SW_SHOW); // SW_SHOW: 창모양으로 출력.
}
