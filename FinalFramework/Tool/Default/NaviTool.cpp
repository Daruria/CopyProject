// NaviTool.cpp : implementation file
//

#include "stdafx.h"
#include "Tool.h"
#include "NaviTool.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "ToolView.h"
#include "Navigation_Tri.h"

// CNaviTool dialog

#ifndef MAX_STR
#define MAX_STR 256
#endif

IMPLEMENT_DYNAMIC(CNaviTool, CDialog)

CNaviTool::CNaviTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_NAVITOOL, pParent)
	, m_fVertexY(0)
{

}

CNaviTool::~CNaviTool()
{
}

void CNaviTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_Auto);
	DDX_Control(pDX, IDC_RADIO2, m_ConnectVertex);
	DDX_Control(pDX, IDC_RADIO3, m_AddFromPicked);

	DDX_Control(pDX, IDC_RADIO4, m_AddVertex);
	DDX_Control(pDX, IDC_RADIO5, m_ModifyVertex);
	DDX_Control(pDX, IDC_CHECK1, m_NaviWireFrame);
	DDX_Control(pDX, IDC_CHECK2, m_NaviCulling);
	DDX_Control(pDX, IDC_RADIO6, m_AllNew);


	DDX_Control(pDX, IDC_RADIO7, m_ChangeOption);
	DDX_Control(pDX, IDC_RADIO8, m_NoOption);
	DDX_Control(pDX, IDC_RADIO9, m_Fall);

	DDX_Text(pDX, IDC_EDIT1, m_fVertexY);
}


BEGIN_MESSAGE_MAP(CNaviTool, CDialog)
	ON_BN_CLICKED(IDC_RADIO4, &CNaviTool::OnBnClickedAddVertex)
	ON_BN_CLICKED(IDC_RADIO5, &CNaviTool::OnBnClickedModifyVertex)
	ON_BN_CLICKED(IDC_CHECK1, &CNaviTool::OnBnClickedWireFrame)
	ON_BN_CLICKED(IDC_CHECK2, &CNaviTool::OnBnClickedCulling)
	ON_BN_CLICKED(IDC_BUTTON3, &CNaviTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CNaviTool::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_BUTTON5, &CNaviTool::OnBnClickedDisunite)
	ON_BN_CLICKED(IDC_BUTTON1, &CNaviTool::OnBnClickedApply)
	ON_BN_CLICKED(IDC_BUTTON6, &CNaviTool::OnBnClickedColorRefresh)
	ON_BN_CLICKED(IDC_RADIO7, &CNaviTool::OnBnClickedChangeOption)
END_MESSAGE_MAP()


// CNaviTool message handlers




DWORD CNaviTool::Get_ChangeOption()
{
	if (m_NoOption.GetCheck())
		return 0;
	else if (m_Fall.GetCheck())
		return 1;


	return 0;
}

void CNaviTool::OnBnClickedAddVertex()
{
	// TODO: Add your control notification handler code here

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	NULL_CHECK(pMainFrame);

	CToolView* pToolView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pToolView);

	pToolView->m_eLeftFunc = ADD_VERTEX;
	pToolView->m_eRightFunc = PICK_VERTEX;
}


void CNaviTool::OnBnClickedModifyVertex()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	NULL_CHECK(pMainFrame);

	CToolView* pToolView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pToolView);

	pToolView->m_eLeftFunc = MODIFY_VERTEX;
	pToolView->m_eRightFunc = PICK_VERTEX;
}


void CNaviTool::OnBnClickedWireFrame()
{
	// TODO: Add your control notification handler code here

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	NULL_CHECK(pMainFrame);

	CToolView* pToolView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pToolView);

	if (m_NaviWireFrame.GetCheck())
		pToolView->m_pNavigation->Set_WireFrame(true);
	else
		pToolView->m_pNavigation->Set_WireFrame(false);
	pToolView->Invalidate(FALSE);
}


void CNaviTool::OnBnClickedCulling()
{
	// TODO: Add your control notification handler code here

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	NULL_CHECK(pMainFrame);

	CToolView* pToolView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pToolView);

	_bool bCull = m_NaviCulling.GetCheck() ? true : false;
	pToolView->m_pNavigation->Set_Culling(bCull);
	pToolView->Invalidate(FALSE);

}


void CNaviTool::OnBnClickedSave()
{
	// TODO: Add your control notification handler code here

	CFileDialog Dlg(FALSE, L".dat", L"力格 绝澜.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	::GetCurrentDirectory(MAX_STR, szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	if (IDOK == Dlg.DoModal())
	{
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		CToolView* pView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
		NULL_CHECK(pView);

		NULL_CHECK(pView->m_pNavigation);

		HANDLE hFile = ::CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, nullptr,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		pView->m_pNavigation->SaveData(hFile, Dlg.GetPathName());

		CloseHandle(hFile);
	}
}


void CNaviTool::OnBnClickedLoad()
{
	// TODO: Add your control notification handler code here

	CFileDialog Dlg(TRUE, L".dat", L"力格 绝澜.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	::GetCurrentDirectory(MAX_STR, szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	if (IDOK == Dlg.DoModal())
	{
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		CToolView* pView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
		NULL_CHECK(pView);

		NULL_CHECK(pView->m_pNavigation);

		HANDLE hFile = ::CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, nullptr,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		pView->m_pNavigation->LoadData(hFile, Dlg.GetPathName());

		CloseHandle(hFile);

		pView->Invalidate(FALSE);
	}
}


void CNaviTool::OnBnClickedDisunite()
{
	// TODO: Add your control notification handler code here

	m_pToolView->Disunite_Vertex();

}


BOOL CNaviTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	NULL_CHECK_RETURN(pMainFrame, FALSE);

	m_pToolView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK_RETURN(m_pToolView, FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CNaviTool::OnBnClickedApply()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	DWORD dwVertexIdx = m_pToolView->m_qPickPair.back().first;
	
	m_pToolView->m_pNavigation->Change_VertexPosY(dwVertexIdx, m_fVertexY);
	m_pToolView->Invalidate(FALSE);

	UpdateData(FALSE);
}


void CNaviTool::OnBnClickedColorRefresh()
{
	// TODO: Add your control notification handler code here

	m_pToolView->m_pNavigation->Refresh_VertexColor();

	m_pToolView->Invalidate(FALSE);
}


void CNaviTool::OnBnClickedChangeOption()
{
	// TODO: Add your control notification handler code here

	m_pToolView->m_eLeftFunc = CHANGE_CELL_OPTION;
}
