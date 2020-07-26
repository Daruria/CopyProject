// ColliderTool.cpp : implementation file
//

#include "stdafx.h"
#include "Tool.h"
#include "ColliderTool.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "afxdialogex.h"

#include "DynamicObject.h"
#include "StaticObject.h"

#ifndef MAX_STR
#define MAX_STR 256
#endif

// CColliderTool dialog

IMPLEMENT_DYNAMIC(CColliderTool, CDialog)

CColliderTool::CColliderTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_COLLIDERTOOL, pParent)
{

}

CColliderTool::~CColliderTool()
{

}

void CColliderTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_MeshList);
	DDX_Control(pDX, IDC_LIST2, m_BoneList);
	DDX_Control(pDX, IDC_RADIO1, m_SphereRadio);
	DDX_Control(pDX, IDC_RADIO2, m_BoxRadio);
	DDX_Control(pDX, IDC_CHECK1, m_WireFrame);
}


BEGIN_MESSAGE_MAP(CColliderTool, CDialog)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CColliderTool::OnLbnSelchangeMeshList)
	ON_LBN_SELCHANGE(IDC_LIST2, &CColliderTool::OnLbnSelchangeBoneList)
	ON_BN_CLICKED(IDC_BUTTON3, &CColliderTool::OnBnClickedBoneLoad)
	ON_BN_CLICKED(IDC_BUTTON1, &CColliderTool::OnBnClickedPutMeshOnScreen)
	ON_BN_CLICKED(IDC_BUTTON2, &CColliderTool::OnBnClickedAddCollider)
	ON_BN_CLICKED(IDC_CHECK1, &CColliderTool::OnBnClickedWireFrame)
END_MESSAGE_MAP()


// CColliderTool message handlers


MESHTYPE CColliderTool::Find_MeshType(const wstring& wstrFullPath, const wstring& wstrRelative)
{
	D3DXFRAME*				pRootFrame;
	CHierarchyLoader*		pLoader;

	pLoader = CHierarchyLoader::Create(m_pGraphicDev, wstrRelative);
	NULL_CHECK_RETURN(pLoader, TYPE_STATIC);

	LPD3DXANIMATIONCONTROLLER		pAniCtrl = nullptr;

	MESHTYPE eType = TYPE_STATIC;

	if (FAILED(D3DXLoadMeshHierarchyFromX(wstrFullPath.c_str(), D3DXMESH_MANAGED, m_pGraphicDev, pLoader, NULL, &pRootFrame, &pAniCtrl)))
		eType = TYPE_STATIC;

	else
		eType = TYPE_DYNAMIC;

	pLoader->DestroyFrame(pRootFrame);
	Safe_Release(pLoader);
	Safe_Release(pAniCtrl);

	return eType;
}

void CColliderTool::Refresh_BonesData(const wstring & wstrFileName)
{
	D3DXFRAME* pRootFrame = dynamic_cast<CDynamicMesh*>(Get_Resources(RESOURCE_STAGE, wstrFileName))->Get_RootFrame();

	queue<D3DXFRAME_DERIVED*> qFrames;

	if (pRootFrame->pFrameSibling)
		qFrames.push((D3DXFRAME_DERIVED*)pRootFrame->pFrameSibling);
	if (pRootFrame->pFrameFirstChild)
		qFrames.push((D3DXFRAME_DERIVED*)pRootFrame->pFrameFirstChild);

	while (!qFrames.empty())
	{
		D3DXFRAME_DERIVED* pFrame = qFrames.front();
		m_vecFrames.push_back(pFrame);

		if (pFrame->pFrameSibling)
			qFrames.push((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);
		if (pFrame->pFrameFirstChild)
			qFrames.push((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);

		qFrames.pop();
	}

	for (auto& pFrame : m_vecFrames)
		m_BoneList.AddString(CString(pFrame->Name));
}

void CColliderTool::Render_ColliderTool()
{

}

void CColliderTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default

	int iCount = ::DragQueryFile(hDropInfo, -1, nullptr, 0);
	TCHAR szFullPath[MAX_STR] = L"";

	TCHAR szCurrent[MAX_STR] = L"";
	TCHAR szRelative[MAX_STR] = L"";
	wstring wstrFileName = L"";
	wstring wstrRelative = L"";
	::GetCurrentDirectory(MAX_STR, szCurrent);

	for (int i = 0; i < iCount; ++i)
	{
		::DragQueryFile(hDropInfo, i, szFullPath, MAX_STR);

		//	상대 경로 얻기.
		::PathRelativePathTo(szRelative, szCurrent, FILE_ATTRIBUTE_DIRECTORY, szFullPath, FILE_ATTRIBUTE_DIRECTORY);
		::PathRemoveFileSpec(szRelative);
		wstrRelative = szRelative;
		wstrRelative += L"\\";

		//	파일 이름 얻기.
		wstrFileName = ::PathFindFileName(szFullPath);

		MESHTYPE eType = Find_MeshType(szFullPath, wstrRelative);

		if (FAILED(Ready_Meshes(m_pGraphicDev, RESOURCE_STAGE, wstrFileName, eType, wstrRelative, wstrFileName)))
			return;

		m_mapFileInfo.emplace(wstrFileName, make_pair(eType, wstrRelative));
		m_MeshList.AddString(wstrFileName.c_str());

	}

	CDialog::OnDropFiles(hDropInfo);
}


BOOL CColliderTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	NULL_CHECK_RETURN(pMainFrame, FALSE);

	CToolView*	pView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK_RETURN(pView, FALSE);

	m_pGraphicDev = pView->m_pGraphicDev;
	m_SphereRadio.SetCheck(TRUE);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CColliderTool::OnLbnSelchangeMeshList()
{
	// TODO: Add your control notification handler code here

	CString		strSelected;
	m_MeshList.GetText(m_MeshList.GetCurSel(), strSelected);



}


void CColliderTool::OnLbnSelchangeBoneList()
{
	// TODO: Add your control notification handler code here
}


void CColliderTool::OnBnClickedBoneLoad()
{
	// TODO: Add your control notification handler code here

	CString strFileName;
	m_MeshList.GetText(m_MeshList.GetCurSel(), strFileName);

	m_vecFrames.clear();
	m_BoneList.ResetContent();

	Refresh_BonesData(wstring(strFileName));

}


void CColliderTool::OnBnClickedPutMeshOnScreen()
{
	// TODO: Add your control notification handler code here
	CString		strSelected;
	m_MeshList.GetText(m_MeshList.GetCurSel(), strSelected);

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	NULL_CHECK(pMainFrame);

	CToolView* pView = dynamic_cast<CToolView*>(pMainFrame->Get_ToolView());
	NULL_CHECK(pView);

	wstring wstrName = strSelected;
	CDynamicObject* pObj = CDynamicObject::Create(m_pGraphicDev, RESOURCE_STAGE, wstrName, wstrName);
	NULL_CHECK(pObj);

	pObj->Get_Transform()->Set_Scale(0.01f);
	pView->m_pCollToolObject = pObj;
	pView->Invalidate(FALSE);
}


void CColliderTool::OnBnClickedAddCollider()
{
	// TODO: Add your control notification handler code here



}


void CColliderTool::OnBnClickedWireFrame()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	NULL_CHECK(pMainFrame);

	CToolView* pView = dynamic_cast<CToolView*>(pMainFrame->Get_ToolView());
	NULL_CHECK(pView);

	_bool bWireFrame = m_WireFrame.GetCheck() ? true : false;
	pView->m_bWireFrame = bWireFrame;
	pView->Invalidate(FALSE);
	
}
