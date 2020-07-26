// ObjectTool.cpp : implementation file
//

#include "stdafx.h"
#include "Tool.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "ObjectTool.h"
#include "afxdialogex.h"
#include "StaticObject.h"
#include "DynamicCamera.h"

#ifndef MAX_STR
#define MAX_STR 256
#endif

// CObjectTool dialog

IMPLEMENT_DYNAMIC(CObjectTool, CDialog)

CObjectTool::CObjectTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_OBJECTTOOL, pParent)
	, m_PosX(0)
	, m_PosY(0)
	, m_PosZ(0)
	, m_RotX(0)
	, m_RotY(0)
	, m_RotZ(0)
	, m_ScaleX(0)
	, m_ScaleY(0)
	, m_ScaleZ(0)
{

}

CObjectTool::~CObjectTool()
{
	//Safe_Release(m_pGraphicDev);
}

void CObjectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ObjList);
	DDX_Text(pDX, IDC_EDIT1, m_PosX);
	DDX_Text(pDX, IDC_EDIT2, m_PosY);
	DDX_Text(pDX, IDC_EDIT3, m_PosZ);
	DDX_Text(pDX, IDC_EDIT4, m_RotX);
	DDX_Text(pDX, IDC_EDIT5, m_RotY);
	DDX_Text(pDX, IDC_EDIT6, m_RotZ);
	DDX_Text(pDX, IDC_EDIT7, m_ScaleX);
	DDX_Text(pDX, IDC_EDIT8, m_ScaleY);
	DDX_Text(pDX, IDC_EDIT9, m_ScaleZ);
	DDX_Control(pDX, IDC_CHECK3, m_WireFrame);
	DDX_Control(pDX, IDC_RADIO2, m_PositionRadio);
	DDX_Control(pDX, IDC_RADIO3, m_RotationRadio);
	DDX_Control(pDX, IDC_RADIO4, m_ScaleRadio);
}


BEGIN_MESSAGE_MAP(CObjectTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CObjectTool::OnBnClickedAddObject)
	ON_BN_CLICKED(IDC_BUTTON2, &CObjectTool::OnBnClickedApply)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON3, &CObjectTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON4, &CObjectTool::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_CHECK3, &CObjectTool::OnBnClickedWireFrameOnOff)
END_MESSAGE_MAP()


// CObjectTool message handlers

BOOL CObjectTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	NULL_CHECK_RETURN(pMainFrm, FALSE);

	CToolView* pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK_RETURN(pToolView, FALSE);

	m_pGraphicDev = pToolView->m_pGraphicDev;
	//m_pGraphicDev->AddRef();

	m_ScaleX = 0.01f;
	m_ScaleY = 0.01f;
	m_ScaleZ = 0.01f;
	m_WireFrame.SetCheck(FALSE);
	m_PositionRadio.SetCheck(TRUE);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CObjectTool::SaveData(HANDLE hFile)
{
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CToolView* pView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pView);

	DWORD dwBytes = 0;

	set<wstring> setFileKeys;

	for (auto& pObj : pView->m_ObjectList)
	{
		auto iter = setFileKeys.find(pObj->Get_Name());

		if (setFileKeys.end() == iter)
			setFileKeys.insert(pObj->Get_Name());
	}


	_ulong dwSize = (_ulong)setFileKeys.size();
	WriteFile(hFile, &dwSize, sizeof(_ulong), &dwBytes, nullptr);

	for (auto& Key : setFileKeys)
	{
		auto iter = m_mapFilePath.find(Key);

		if (m_mapFilePath.end() == iter)
			MessageBox(L"Key is not Exist", L"Save FilePath Failed.", MB_OK);

		_ulong dwLength = (_ulong)iter->first.length();
		WriteFile(hFile, &dwLength, sizeof(_ulong), &dwBytes, nullptr);
		WriteFile(hFile, iter->first.c_str(), sizeof(_tchar) * dwLength, &dwBytes, nullptr);

		dwLength = (_ulong)iter->second.length();
		WriteFile(hFile, &dwLength, sizeof(_ulong), &dwBytes, nullptr);
		WriteFile(hFile, iter->second.c_str(), sizeof(_tchar) * dwLength, &dwBytes, nullptr);
	}


	//_ulong dwSize = (_ulong)m_mapFilePath.size();
	//WriteFile(hFile, &dwSize, sizeof(_ulong), &dwBytes, nullptr);

	//for (auto& Pair : m_mapFilePath)
	//{
	//	_ulong dwLength = (_ulong)Pair.first.length();
	//	WriteFile(hFile, &dwLength, sizeof(_ulong), &dwBytes, nullptr);
	//	WriteFile(hFile, Pair.first.c_str(), sizeof(_tchar) * dwLength, &dwBytes, nullptr);

	//	dwLength = (_ulong)Pair.second.length();
	//	WriteFile(hFile, &dwLength, sizeof(_ulong), &dwBytes, nullptr);
	//	WriteFile(hFile, Pair.second.c_str(), sizeof(_tchar) * dwLength, &dwBytes, nullptr);
	//}

	dwSize = (_ulong)pView->m_ObjectList.size();
	WriteFile(hFile, &dwSize, sizeof(_ulong), &dwBytes, nullptr);
	for (auto& pObj : pView->m_ObjectList)
	{
		CTransform* pTransform = pObj->Get_Transform();
		_vec3 vScale, vPos, vAngle;
		wstring wstrName = pObj->Get_Name();

		_ulong dwNameLength = (_ulong)wstrName.length();

		pTransform->Get_Scale(&vScale);
		pTransform->Get_Info(INFO_POS, &vPos);
		pTransform->Get_Angle(&vAngle);
		WriteFile(hFile, &vScale, sizeof(_vec3), &dwBytes, nullptr);
		WriteFile(hFile, &vPos, sizeof(_vec3), &dwBytes, nullptr);
		WriteFile(hFile, &vAngle, sizeof(_vec3), &dwBytes, nullptr);
		WriteFile(hFile, &dwNameLength, sizeof(_ulong), &dwBytes, nullptr);
		WriteFile(hFile, wstrName.c_str(), sizeof(_tchar) * dwNameLength, &dwBytes, nullptr);

	}
}

void CObjectTool::LoadData(HANDLE hFile)
{
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CToolView* pView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pView);

	for (auto& pObj : pView->m_ObjectList)
		Safe_Release(pObj);
	
	pView->m_ObjectList.clear();

	DWORD dwBytes = 0;

	_ulong dwSize = 0;
	ReadFile(hFile, &dwSize, sizeof(_ulong), &dwBytes, nullptr);

	for (_ulong i = 0; i < dwSize; ++i)
	{
		_ulong dwLength = 0;
		_tchar szRead[MAX_STR] = L"";
		ReadFile(hFile, &dwLength, sizeof(_ulong), &dwBytes, nullptr);
		ReadFile(hFile, szRead, sizeof(_tchar) * dwLength, &dwBytes, nullptr);

		wstring wstrFirst = szRead;

		ZeroMemory(szRead, sizeof(_tchar) * MAX_STR);
		ReadFile(hFile, &dwLength, sizeof(_ulong), &dwBytes, nullptr);
		ReadFile(hFile, szRead, sizeof(_tchar) * dwLength, &dwBytes, nullptr);
		wstring wstrSecond = szRead;

		auto iter = m_mapFilePath.find(wstrFirst);
		if (m_mapFilePath.end() == iter)
		{
			m_mapFilePath.emplace(wstrFirst, wstrSecond);
			m_ObjList.AddString(wstrFirst.c_str());
		}

		Ready_Meshes(m_pGraphicDev, RESOURCE_STAGE, wstrFirst, TYPE_STATIC, wstrSecond, wstrFirst);
	}

	ReadFile(hFile, &dwSize, sizeof(_ulong), &dwBytes, nullptr);

	for (_ulong i = 0; i < dwSize; ++i)
	{
		_vec3 vScale, vPos, vAngle;
		
		_tchar szRead[MAX_STR] = L"";
		_ulong dwNameLength = 0;

		ReadFile(hFile, &vScale, sizeof(_vec3), &dwBytes, nullptr);
		ReadFile(hFile, &vPos, sizeof(_vec3), &dwBytes, nullptr);
		ReadFile(hFile, &vAngle, sizeof(_vec3), &dwBytes, nullptr);
		ReadFile(hFile, &dwNameLength, sizeof(_ulong), &dwBytes, nullptr);
		ReadFile(hFile, szRead, sizeof(_tchar) * dwNameLength, &dwBytes, nullptr);
		wstring wstrName = szRead;

		CStaticObject* pObj = CStaticObject::Create(m_pGraphicDev, RESOURCE_STAGE, wstrName, wstrName);
		NULL_CHECK(pObj);

		CTransform* pTransform = pObj->Get_Transform();
		pTransform->Set_Pos(vPos);
		pTransform->Set_Angle(vAngle);
		pTransform->Set_Scale(vScale);

		pView->m_ObjectList.push_back(pObj);
	}

	pView->Invalidate(FALSE);
}

void CObjectTool::OnBnClickedAddObject()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CString strFileName;
	m_ObjList.GetText(m_ObjList.GetCurSel(), strFileName);
	

	auto iter = m_mapFilePath.find(strFileName.GetString());
	
	if (m_mapFilePath.end() == iter)
		return;

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	NULL_CHECK(pMainFrm);

	CToolView* pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pToolView);

	CStaticObject* pObj = CStaticObject::Create(m_pGraphicDev, RESOURCE_STAGE, iter->first, iter->first);
	NULL_CHECK(pObj);

	CTransform* pTransform = pObj->Get_Transform();

	_vec3 vPos;
	pToolView->m_pCamera->Get_At(&vPos);

	pTransform->Set_Scale(m_ScaleX, m_ScaleY, m_ScaleZ);
	pTransform->Set_Angle(D3DXToRadian(m_RotX), D3DXToRadian(m_RotY), D3DXToRadian(m_RotZ));
	pTransform->Set_Pos(vPos);

	pToolView->m_ObjectList.push_back(pObj);
	pToolView->Invalidate(FALSE);

	UpdateData(FALSE);
}


void CObjectTool::OnBnClickedApply()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	NULL_CHECK(pMainFrm);

	CToolView* pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pToolView);

	if (!pToolView->m_pPickedObject)
		return;

	CTransform* pTransform = pToolView->m_pPickedObject->Get_Transform();
	pTransform->Set_Scale(m_ScaleX, m_ScaleY, m_ScaleZ);
	pTransform->Set_Angle(D3DXToRadian(m_RotX), D3DXToRadian(m_RotY), D3DXToRadian(m_RotZ));
	pTransform->Set_Pos(m_PosX, m_PosY, m_PosZ);

	pToolView->Invalidate(FALSE);

	UpdateData(FALSE);
}



void CObjectTool::OnDropFiles(HDROP hDropInfo)
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

		if (FAILED(Ready_Meshes(m_pGraphicDev, RESOURCE_STAGE, wstrFileName, TYPE_STATIC, wstrRelative, wstrFileName)))
			return;

		m_mapFilePath.emplace(wstrFileName, wstrRelative);

		m_ObjList.AddString(wstrFileName.c_str());
	}

	CDialog::OnDropFiles(hDropInfo);
}


void CObjectTool::OnBnClickedSave()
{
	// TODO: Add your control notification handler code here
	CFileDialog Dlg(FALSE, L".dat", L"제목 없음.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	::GetCurrentDirectory(MAX_STR, szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	if (IDOK == Dlg.DoModal())
	{
		HANDLE hFile = ::CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, nullptr,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		SaveData(hFile);

		CloseHandle(hFile);
	}
}


void CObjectTool::OnBnClickedLoad()
{
	// TODO: Add your control notification handler code here
	CFileDialog Dlg(TRUE, L".dat", L"제목 없음.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat|Text Files(*.txt)|*.txt||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	::GetCurrentDirectory(MAX_STR, szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	if (IDOK == Dlg.DoModal())
	{

		HANDLE hFile = ::CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, nullptr,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		LoadData(hFile);

		CloseHandle(hFile);

	}
}


void CObjectTool::OnBnClickedWireFrameOnOff()
{
	// TODO: Add your control notification handler code here

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	NULL_CHECK(pMainFrm);

	CToolView* pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pToolView);

	_bool	bObjWireFrame = m_WireFrame.GetCheck() ? true : false;
	pToolView->m_bObjWireFrame = bObjWireFrame;
	pToolView->Invalidate(FALSE);

}
