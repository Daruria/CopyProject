// MyForm.cpp : implementation file
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"
#include "MainFrm.h"
#include "ToolView.h"


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
	DDX_Control(pDX, IDC_RADIO1, m_NaviRadio);
	DDX_Control(pDX, IDC_RADIO2, m_ObjectRadio);
	DDX_Control(pDX, IDC_RADIO3, m_ColliderRadio);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyForm::OnBnClickedNaviTool)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyForm::OnBnClickedObjectTool)
	ON_BN_CLICKED(IDC_RADIO1, &CMyForm::OnBnClickedNaviRadio)
	ON_BN_CLICKED(IDC_RADIO2, &CMyForm::OnBnClickedObjectRadio)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyForm::OnBnClickedColliderTool)
	ON_BN_CLICKED(IDC_RADIO3, &CMyForm::OnBnClickedCollRadio)
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


void CMyForm::OnBnClickedNaviTool()
{
	// TODO: Add your control notification handler code here
	NULL_CHECK_MSG(m_NaviTool.GetSafeHwnd(), L"NaviTool is null");
	m_NaviTool.ShowWindow(SW_SHOW);
}


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_NaviTool.Create(IDD_NAVITOOL);
	m_ObjectTool.Create(IDD_OBJECTTOOL);
	m_ColliderTool.Create(IDD_COLLIDERTOOL);

	m_NaviRadio.SetCheck(TRUE);

	UpdateData(FALSE);

	// TODO: Add your specialized code here and/or call the base class
}


void CMyForm::OnBnClickedObjectTool()
{
	// TODO: Add your control notification handler code here
	NULL_CHECK_MSG(m_ObjectTool.GetSafeHwnd(), L"ObjectTool is null");
	m_ObjectTool.ShowWindow(SW_SHOW);
}

void CMyForm::OnBnClickedColliderTool()
{
	// TODO: Add your control notification handler code here
	NULL_CHECK_MSG(m_ColliderTool.GetSafeHwnd(), L"ObjectTool is null");
	m_ColliderTool.ShowWindow(SW_SHOW);
}

void CMyForm::OnBnClickedNaviRadio()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	NULL_CHECK(pMainFrame);

	CToolView* pView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pView);

	pView->m_eLeftFunc	= MODIFY_VERTEX;
	pView->m_eRightFunc = PICK_VERTEX;
	pView->m_eToolMode	= NAVI_TOOL;

	D3DXMatrixIdentity(&pView->m_matAxisLines);

	pView->Invalidate(FALSE);
}

void CMyForm::OnBnClickedObjectRadio()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	NULL_CHECK(pMainFrame);

	CToolView* pView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pView);

	pView->m_eLeftFunc	= MOVE_OBJECT;
	pView->m_eRightFunc = PICK_OBJECT;
	pView->m_eToolMode	= OBJECT_TOOL;
	pView->Invalidate(FALSE);
}

void CMyForm::OnBnClickedCollRadio()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	NULL_CHECK(pMainFrame);

	CToolView* pView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pView);

	pView->m_eLeftFunc	= MOVE_OBJECT;
	pView->m_eRightFunc = PICK_OBJECT;
	pView->m_eToolMode	= COLLIDER_TOOL;
	pView->Invalidate(FALSE);
}
