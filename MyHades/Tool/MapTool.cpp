// MapTool.cpp : implementation file
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"

#include "DirectoryMgr.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "Terrain.h"
#include "AnimatedObject.h"
#include "NonAnimatedObject.h"

// CMapTool dialog

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent),
	m_iOptionID(0),
	m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()), 
	m_ObjectScaleX(1.f),
	m_ObjectScaleY(1.f)
	, m_fSort(0)
	, m_fPositionX(0)
	, m_fPositionY(0)
	, m_RotationDegree(0)
{

}

CMapTool::~CMapTool()
{
	//for_each(m_umapFileInfo.begin(), m_umapFileInfo.end(), SafeDelete<FILE_INFO*>);
	for (auto& Pair : m_umapFileInfo)
		SafeDelete(Pair.second);
	
	m_umapFileInfo.clear();

	for (auto& Pair : m_umapAnimPathInfo)
		SafeDelete(Pair.second);

	m_umapAnimPathInfo.clear();

}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[1]);
	DDX_Control(pDX, IDC_RADIO5, m_Radio[2]);
	DDX_Control(pDX, IDC_RADIO6, m_Radio[3]);
	DDX_Control(pDX, IDC_LIST1, m_ObjectList);
	DDX_Text(pDX, IDC_EDIT1, m_ObjectScaleX);
	DDX_Text(pDX, IDC_EDIT2, m_ObjectScaleY);
	DDX_Control(pDX, IDC_LIST2, m_StateList);
	DDX_Control(pDX, IDC_LIST3, m_FileList);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
	DDX_Text(pDX, IDC_EDIT3, m_fSort);
	DDX_Text(pDX, IDC_EDIT4, m_fPositionX);
	DDX_Text(pDX, IDC_EDIT5, m_fPositionY);
	DDX_Control(pDX, IDC_RADIO7, m_RenderTypeRadio[0]);
	DDX_Control(pDX, IDC_RADIO8, m_RenderTypeRadio[1]);
	DDX_Control(pDX, IDC_RADIO9, m_RenderTypeRadio[2]);
	DDX_Text(pDX, IDC_EDIT6, m_RotationDegree);
	DDX_Control(pDX, IDC_RADIO3, m_TileOptionRadio[0]);
	DDX_Control(pDX, IDC_RADIO4, m_TileOptionRadio[1]);
	DDX_Control(pDX, IDC_RADIO10, m_TileOptionRadio[2]);
	DDX_Control(pDX, IDC_RADIO11, m_TileOptionRadio[3]);
	DDX_Control(pDX, IDC_RADIO12, m_TileOptionRadio[4]);
	DDX_Control(pDX, IDC_RADIO13, m_TileOptionRadio[5]);
	DDX_Control(pDX, IDC_RADIO14, m_TileOptionRadio[6]);
	DDX_Control(pDX, IDC_RADIO15, m_TileOptionRadio[7]);
	DDX_Control(pDX, IDC_RADIO16, m_TileOptionRadio[8]);
	DDX_Control(pDX, IDC_RADIO17, m_TileOptionRadio[9]);
	DDX_Control(pDX, IDC_RADIO18, m_TileOptionRadio[10]);
	DDX_Control(pDX, IDC_RADIO19, m_TileOptionRadio[11]);
	DDX_Control(pDX, IDC_RADIO20, m_TileOptionRadio[12]);
	DDX_Control(pDX, IDC_RADIO21, m_TileOptionRadio[13]);
	DDX_Control(pDX, IDC_CHECK1, m_ShowCheck[0]);
	DDX_Control(pDX, IDC_CHECK2, m_ShowCheck[1]);
	DDX_Control(pDX, IDC_CHECK3, m_ShowCheck[2]);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelchangeObjectKey)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnBnClickedAddObject)
	ON_LBN_SELCHANGE(IDC_LIST2, &CMapTool::OnLbnSelchangeStateKey)
	ON_LBN_SELCHANGE(IDC_LIST3, &CMapTool::OnLbnSelchangeFileName)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON3, &CMapTool::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_RADIO5, &CMapTool::OnBnClickedAnimated)
	ON_BN_CLICKED(IDC_RADIO6, &CMapTool::OnBnClickedNonAnimated)
	ON_BN_CLICKED(IDC_RADIO1, &CMapTool::OnBnClickedOption)
	ON_BN_CLICKED(IDC_RADIO2, &CMapTool::OnBnClickedImgObjRadio)
	ON_BN_CLICKED(IDC_BUTTON4, &CMapTool::OnBnClickedApply)
	ON_BN_CLICKED(IDC_CHECK1, &CMapTool::OnBnClickedCheckTerrain)
	ON_BN_CLICKED(IDC_CHECK2, &CMapTool::OnBnClickedCheckObject)
	ON_BN_CLICKED(IDC_CHECK3, &CMapTool::OnBnClickedCheckFrontObject)
END_MESSAGE_MAP()

// CMapTool message handlers

BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_Radio[0].SetCheck(TRUE);
	m_Radio[2].SetCheck(TRUE);
	m_Radio[5].SetCheck(TRUE);
	m_RenderTypeRadio[0].SetCheck(TRUE);

	int TileSize = sizeof(TILE_INFO);

	UpdateData(FALSE);

	RefreshMap();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CMapTool::SettingHorizontalScrollSize()
{
	// 리스트박스에 수평 스크롤 확장하기.

	CString strName = L"";

	int iSrcCX = 0, iDstCX = 0;

	CDC* pDC = m_ObjectList.GetDC();

	for (int i = 0; i < m_ObjectList.GetCount(); ++i)
	{
		m_ObjectList.GetText(i, strName);

		// 현재 문자열의 길이를 픽셀 단위로 환산.
		iSrcCX = pDC->GetTextExtent(strName).cx;

		if (iDstCX < iSrcCX)
			iDstCX = iSrcCX;
	}

	m_ObjectList.ReleaseDC(pDC);

	// 현재 리스트박스가 가지고 있는 수평 스크롤 범위보다 클 때
	if (iDstCX > m_ObjectList.GetHorizontalExtent())
		m_ObjectList.SetHorizontalExtent(iDstCX);// 수평 스크롤 크기 갱신.
}

BOOL CMapTool::RefreshMap()
{
	if (!m_umapFileInfo.empty())
	{
		for (auto& Pair : m_umapFileInfo)
			SafeDelete(Pair.second);
		m_umapFileInfo.clear();
	}

	TCHAR szCurrentPath[MAX_STR] = L"";
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	::PathRemoveFileSpec(szCurrentPath);

	wstring wstrFullPath = szCurrentPath;
	wstrFullPath += L"\\Texture\\Terrain\\NonAnimatedObject\\";
	CDirectoryMgr::ExtractFileInfo(wstrFullPath.c_str(), m_umapFileInfo);

	list<PATH_INFO*> PathInfoList;

	CDirectoryMgr::ExtractPathInfo(wstrFullPath.c_str(), PathInfoList);

	LPDIRECT3DDEVICE9 pGraphicDev = m_pDeviceMgr->GetDevice();

	for (auto& pPathInfo : PathInfoList)
		m_pTextureMgr->LoadTexture(pGraphicDev, TEXTURE_MULTI, pPathInfo->wstrRelative, pPathInfo->wstrObjectKey, pPathInfo->wstrStateKey, pPathInfo->iImgCount, 1);
		
	for (auto& pPathInfo : PathInfoList)
		SafeDelete(pPathInfo);

	PathInfoList.clear();

	vector<wstring> vecObjectKey;
	CDirectoryMgr::ExtractFolders(wstrFullPath.c_str(), vecObjectKey);

	m_ObjectList.ResetContent();

	for (auto& wstrObjectKey : vecObjectKey)
		m_ObjectList.AddString(wstrObjectKey.c_str());

	wstrFullPath = szCurrentPath;
	wstrFullPath += L"\\Texture\\Terrain\\AnimatedObject\\";

	CDirectoryMgr::ExtractPathInfoForState(wstrFullPath.c_str(), m_umapAnimPathInfo);

	for (auto& Pair : m_umapAnimPathInfo)
		m_pTextureMgr->LoadTexture(pGraphicDev, TEXTURE_MULTI, Pair.second->wstrRelative, Pair.second->wstrObjectKey, Pair.second->wstrStateKey, Pair.second->iImgCount, 1);

	UpdateData(FALSE);

	return TRUE;
}

//	2 : Animated, 3 : NonAnimated
int CMapTool::GetCurImageObjectType()	
{
	if (m_Radio[2].GetCheck())
		return 2;
	else if (m_Radio[3].GetCheck())
		return 3;

	return 0;
}

//	0 : Option, 1 : ImageObject
int CMapTool::GetCurOptionImageRadio()	
{
	if (m_Radio[0].GetCheck())
		return 0;
	else if (m_Radio[1].GetCheck())
		return 1;
	return -1;
}

//	0 : No Option, 1 : No Move, 2: Push LT, 3: Push RT, 4: Push LB, 5: Push RB, 6:Push Up, 7:Push Right, 8: Push Down, 9: Push Left
//	10: Push LTR, 11 : Push TRB, 12: Push RBL 13: Push BLT
int CMapTool::GetCurOptionType()	
{
	for (int i = 0; i < 14; ++i)
	{
		if (m_TileOptionRadio[i].GetCheck())
			return i;
	}

	return -1;
}

int CMapTool::GetCurPushList()
{
	if (m_RenderTypeRadio[0].GetCheck())
		return 0;
	else if (m_RenderTypeRadio[1].GetCheck())
		return 1;
	else if (m_RenderTypeRadio[2].GetCheck())
		return 2;

	return -1;
}

CAnimatedObject * CMapTool::CreateAnimatedObject(CToolView * pToolView)
{
	auto iter = m_umapAnimPathInfo.find(m_wstrCurSelState);

	if (m_umapAnimPathInfo.end() == iter)
		return nullptr;

	m_fSort += 1.f;

	PATH_INFO tPathInfo = *(iter->second);
	bool bRenderListPush = false;
	if (m_RenderTypeRadio[0].GetCheck())
		bRenderListPush = false;
	else
		bRenderListPush = true;
	CAnimatedObject* pAnimatedObject = CAnimatedObject::Create(tPathInfo.wstrObjectKey, tPathInfo.wstrStateKey, tPathInfo.iImgCount,
		D3DXVECTOR3(WINCX * 0.5f + pToolView->GetScrollPos(0), WINCY * 0.5f + pToolView->GetScrollPos(1), 0.f),
		D3DXVECTOR3(m_ObjectScaleX, m_ObjectScaleY, 0.f), bRenderListPush, m_RotationDegree, m_fSort, RENDER_OBJECT, pToolView);


	return pAnimatedObject;
}

CNonAnimatedObject * CMapTool::CreateNonAnimatedObject(CToolView * pToolView)
{
	auto iter = m_umapFileInfo.find(m_wstrCurSelFile);

	if (m_umapFileInfo.end() == iter)
		return nullptr;

	m_fSort += 1.f;

	FILE_INFO tFileInfo = *(iter->second);

	int iDrawID = 0;
	int iNumStart = 0;
	for (int i = 0; i < tFileInfo.wstrFileName.size(); ++i)
	{
		if (tFileInfo.wstrFileName[i] == L'(')
			iNumStart = i;
		else if (tFileInfo.wstrFileName[i] == L')')
			iDrawID = _ttoi(tFileInfo.wstrFileName.substr(iNumStart + 1, i - iNumStart).c_str());
	}

	bool bRenderListPush = false;
	if (m_RenderTypeRadio[0].GetCheck())
		bRenderListPush = false;
	else
		bRenderListPush = true;

	CNonAnimatedObject* pNonAnimatedObject = CNonAnimatedObject::Create(tFileInfo.wstrObjectKey, tFileInfo.wstrStateKey,
		D3DXVECTOR3(WINCX * 0.5f + pToolView->GetScrollPos(0), WINCY * 0.5f + pToolView->GetScrollPos(1), 0.f), 
		D3DXVECTOR3(m_ObjectScaleX, m_ObjectScaleY, 0.f), bRenderListPush, m_RotationDegree, m_fSort, iDrawID, RENDER_OBJECT, pToolView);

	return pNonAnimatedObject;
}

void CMapTool::OnLbnSelchangeObjectKey()
{
	// TODO: Add your control notification handler code here

	//m_vecStateKey.clear();

	int iIndex = m_ObjectList.GetCurSel();

	if (-1 == iIndex)
		return;

	CString strObjectKey;
	m_ObjectList.GetText(iIndex, strObjectKey);
	m_wstrCurSelObject = strObjectKey;

	TCHAR szCurrentPath[MAX_STR] = L"";
	::GetCurrentDirectory(MAX_STR, szCurrentPath);
	::PathRemoveFileSpec(szCurrentPath);

	wstring wstrFullPath = szCurrentPath;

	if (m_Radio[2].GetCheck())		//	AnimatedObject
		wstrFullPath += L"\\Texture\\Terrain\\AnimatedObject\\";

	else if (m_Radio[3].GetCheck())	//	NonAnimatedObject
		wstrFullPath += L"\\Texture\\Terrain\\NonAnimatedObject\\";

	wstrFullPath += strObjectKey;

	vector<wstring> vecStateKey;
	CDirectoryMgr::ExtractFolders(wstrFullPath.c_str(), vecStateKey);

	m_StateList.ResetContent();

	for (auto& wstrStateKey : vecStateKey)
		m_StateList.AddString(wstrStateKey.c_str());

	UpdateData(FALSE);
}

void CMapTool::OnLbnSelchangeStateKey()
{
	// TODO: Add your control notification handler code here
	//m_vecFileKey.clear();

	int iIndex = m_ObjectList.GetCurSel();

	CString strStateKey;
	m_ObjectList.GetText(iIndex, strStateKey);
	m_wstrCurSelState = strStateKey;

	if (-1 == iIndex)
		return;

	if (m_Radio[2].GetCheck())
	{
		auto iter = m_umapAnimPathInfo.find(strStateKey.GetString());
		
		if (m_umapAnimPathInfo.end() == iter)
			return;

		PATH_INFO* pPathInfo = iter->second;

		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(pPathInfo->wstrObjectKey, pPathInfo->wstrStateKey, 1);
		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, WINCY * 0.5f, 0.f);

		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);

		m_pDeviceMgr->Render_Begin();

		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		m_pDeviceMgr->Render_End(m_Picture.m_hWnd);
	}
	else if (m_Radio[3].GetCheck())
	{
		TCHAR szCurrentPath[MAX_STR] = L"";
		::GetCurrentDirectory(MAX_STR, szCurrentPath);

		::PathRemoveFileSpec(szCurrentPath);

		wstring wstrFullPath = szCurrentPath;
		wstrFullPath += L"\\Texture\\Terrain\\NonAnimatedObject\\";

		wstrFullPath += strStateKey;
		wstrFullPath += L"\\";

		iIndex = m_StateList.GetCurSel();

		if (-1 == iIndex)
			return;

		CString strStateKey;
		m_StateList.GetText(iIndex, strStateKey);
		wstrFullPath += strStateKey;

		vector<wstring> vecFileKey;
		CDirectoryMgr::ExtractFiles(wstrFullPath.c_str(), vecFileKey);

		m_FileList.ResetContent();

		for (auto& wstrFileKey : vecFileKey)
			m_FileList.AddString(wstrFileKey.c_str());
	}

	UpdateData(FALSE);
}

void CMapTool::OnLbnSelchangeFileName()
{
	// TODO: Add your control notification handler code here
	int iIndex = m_FileList.GetCurSel();

	if (-1 == iIndex)
		return;

	CString strFileKey;
	m_FileList.GetText(iIndex, strFileKey);
	m_wstrCurSelFile = strFileKey;

	auto iter = m_umapFileInfo.find(strFileKey.GetString());

	if (m_umapFileInfo.end() == iter)
		return;

	FILE_INFO* pFileInfo = iter->second;

	int iDrawID = 0;
	int iNumStart = 0;
	for (int i = 0; i < (pFileInfo->wstrFileName).size(); ++i)
	{
		if ((pFileInfo->wstrFileName)[i] == L'(')
			iNumStart = i;
		else if ((pFileInfo->wstrFileName)[i] == L')')
			iDrawID = _ttoi((pFileInfo->wstrFileName).substr(iNumStart + 1, i - iNumStart).c_str());
	}

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(pFileInfo->wstrObjectKey, pFileInfo->wstrStateKey, iDrawID);
	NULL_CHECK(pTexInfo);
	
	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, WINCX * 0.5f, WINCY * 0.5f, 0.f);

	matWorld = matScale * matTrans;

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);

	m_pDeviceMgr->Render_Begin();

	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pDeviceMgr->Render_End(m_Picture.m_hWnd);
	
}

void CMapTool::OnBnClickedAnimated()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	//m_vecObjectKey.clear();
	//m_vecStateKey.clear();
	//m_vecFileKey.clear();

	m_wstrCurSelObject = L"";
	m_wstrCurSelState = L"";
	m_wstrCurSelFile = L"";

	m_ObjectList.ResetContent();
	m_StateList.ResetContent();
	m_FileList.ResetContent();

	if (!m_umapAnimPathInfo.empty())
	{
		for (auto& Pair : m_umapAnimPathInfo)
			SafeDelete(Pair.second);
		m_umapAnimPathInfo.clear();
	}

	TCHAR szCurrentPath[MAX_STR] = L"";
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	::PathRemoveFileSpec(szCurrentPath);

	wstring wstrFullPath = szCurrentPath;
	wstrFullPath += L"\\Texture\\Terrain\\AnimatedObject\\";


	CDirectoryMgr::ExtractPathInfoForState(wstrFullPath.c_str(), m_umapAnimPathInfo);

	vector<wstring> vecObjectKey;
	CDirectoryMgr::ExtractFolders(wstrFullPath.c_str(), vecObjectKey);
	for (auto& wstrObjectKey : vecObjectKey)
		m_ObjectList.AddString(wstrObjectKey.c_str());
}

void CMapTool::OnBnClickedNonAnimated()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	//m_vecObjectKey.clear();
	//m_vecStateKey.clear();
	//m_vecFileKey.clear();

	m_wstrCurSelObject = L"";
	m_wstrCurSelState = L"";
	m_wstrCurSelFile = L"";

	m_ObjectList.ResetContent();
	m_StateList.ResetContent();
	m_FileList.ResetContent();

	if (!m_umapFileInfo.empty())
	{
		for (auto& Pair : m_umapFileInfo)
			SafeDelete(Pair.second);
		m_umapFileInfo.clear();
	}

	TCHAR szCurrentPath[MAX_STR] = L"";
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	::PathRemoveFileSpec(szCurrentPath);

	wstring wstrFullPath = szCurrentPath;
	wstrFullPath += L"\\Texture\\Terrain\\NonAnimatedObject\\";
	CDirectoryMgr::ExtractFileInfo(wstrFullPath.c_str(), m_umapFileInfo);

	vector<wstring> vecObjectKey;
	CDirectoryMgr::ExtractFolders(wstrFullPath.c_str(), vecObjectKey);
	for (auto& wstrObjectKey : vecObjectKey)
		m_ObjectList.AddString(wstrObjectKey.c_str());


}

void CMapTool::OnBnClickedAddObject()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pMainFrame);

	CToolView* pToolView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pToolView);

	CTerrain* pTerrain = pToolView->m_pTerrain;

	CImageObject* pImageObject = nullptr;

	//m_fSort += 1.f;

	if (m_Radio[2].GetCheck())		//	Animated
	{
		pImageObject = CreateAnimatedObject(pToolView);
		NULL_CHECK(pImageObject);

	}
	else if (m_Radio[3].GetCheck())	//	NonAnimated
	{
		pImageObject = CreateNonAnimatedObject(pToolView);
		NULL_CHECK(pImageObject);
	}

	if (m_RenderTypeRadio[0].GetCheck())	//	Terrain에 넣을것.
	{
		pImageObject->SetRenderListPush(false);
		pTerrain->AddImageObject(pImageObject);
	}
	else									//	ObjectMgr에 넘길 것.
	{
		RENDER_TYPE eRenderType = RENDER_END;
		pImageObject->SetRenderListPush(true);
		if (m_RenderTypeRadio[1].GetCheck())	//	플레이어와 위치 비교를 해야하는 경우.
			eRenderType = RENDER_OBJECT;
		else if (m_RenderTypeRadio[2].GetCheck())	//	플레이어 보다 앞에 위치해야 하는 경우.
			eRenderType = RENDER_FRONT_OBJECT;
		pImageObject->SetRenderType(eRenderType);
		pToolView->m_pObjectMgr->AddObject(OBJECT_ENVIRONMENT, pImageObject);
	}

	pToolView->Invalidate(FALSE);

	UpdateData(FALSE);
}

void CMapTool::OnBnClickedSave()
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
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		CToolView* pView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
		NULL_CHECK(pView);

		NULL_CHECK(pView->m_pTerrain);

		HANDLE hFile = ::CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, nullptr,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		pView->m_pTerrain->SaveData(hFile, Dlg.GetPathName());
		pView->m_pObjectMgr->SaveData(hFile, Dlg.GetPathName(), pView);

		CloseHandle(hFile);
	}
}


void CMapTool::OnBnClickedLoad()
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
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		CToolView* pView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
		NULL_CHECK(pView);

		NULL_CHECK(pView->m_pTerrain);

		HANDLE hFile = ::CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, nullptr,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		pView->m_pTerrain->LoadData(hFile, Dlg.GetPathName());
		pView->m_pObjectMgr->LoadData(hFile, Dlg.GetPathName(), pView);

		CloseHandle(hFile);

		pView->Invalidate(FALSE);
	}
}


void CMapTool::OnBnClickedOption()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}


void CMapTool::OnBnClickedImgObjRadio()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CMapTool::OnBnClickedApply()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pToolView);

	CImageObject* pImgObj = pToolView->m_pImageObject;
	NULL_CHECK(pImgObj);

	CImageObject* pImageObject = nullptr;
	if (m_RenderTypeRadio[0].GetCheck())	//	Terrain List Push
	{
		switch (pImgObj->GetImgType())
		{
		case CImageObject::ANIM_OBJ:
		{
			auto iter = m_umapAnimPathInfo.find(pImgObj->GetStateKey());

			if (m_umapAnimPathInfo.end() == iter)
				return;

			PATH_INFO tPathInfo = *iter->second;

			pImageObject = CAnimatedObject::Create(tPathInfo.wstrObjectKey, tPathInfo.wstrStateKey, tPathInfo.iImgCount,
				D3DXVECTOR3(m_fPositionX, m_fPositionY, 0.f), D3DXVECTOR3(m_ObjectScaleX, m_ObjectScaleY, 0.f),
				false, m_RotationDegree, m_fSort, RENDER_OBJECT, pToolView);

		}
			break;
		case CImageObject::NON_ANIM_OBJ:
			pImageObject = CNonAnimatedObject::Create(pImgObj->GetObjectKey(), pImgObj->GetStateKey(),
				D3DXVECTOR3(m_fPositionX, m_fPositionY, 0.f), D3DXVECTOR3(m_ObjectScaleX, m_ObjectScaleY, 0.f), 
				false, m_RotationDegree, m_fSort, dynamic_cast<CNonAnimatedObject*>(pImgObj)->GetDrawID(), RENDER_OBJECT, pToolView);
			break;
		}//	Switch End
		NULL_CHECK(pImageObject);
		
		pToolView->m_pTerrain->AddImageObject(pImageObject);
	}
	else	//	ObjectMgr List Push
	{

		RENDER_TYPE eRenderType = RENDER_END;
		if (m_RenderTypeRadio[1].GetCheck())
			eRenderType = RENDER_OBJECT;
		else if (m_RenderTypeRadio[2].GetCheck())
			eRenderType = RENDER_FRONT_OBJECT;
		
		switch (pImgObj->GetImgType())
		{
		case CImageObject::ANIM_OBJ:
		{
			auto iter = m_umapAnimPathInfo.find(pImgObj->GetStateKey());

			if (m_umapAnimPathInfo.end() == iter)
				return;

			PATH_INFO tPathInfo = *iter->second;

			pImageObject = CAnimatedObject::Create(tPathInfo.wstrObjectKey, tPathInfo.wstrStateKey, tPathInfo.iImgCount,
				D3DXVECTOR3(m_fPositionX, m_fPositionY, 0.f), D3DXVECTOR3(m_ObjectScaleX, m_ObjectScaleY, 0.f),
				true, m_RotationDegree, m_fSort, eRenderType, pToolView);
		}
		break;
		case CImageObject::NON_ANIM_OBJ:
			pImageObject = CNonAnimatedObject::Create(pImgObj->GetObjectKey(), pImgObj->GetStateKey(),
				D3DXVECTOR3(m_fPositionX, m_fPositionY, 0.f), D3DXVECTOR3(m_ObjectScaleX, m_ObjectScaleY, 0.f),
				true, m_RotationDegree, m_fSort, dynamic_cast<CNonAnimatedObject*>(pImgObj)->GetDrawID(), eRenderType, pToolView);
			pImageObject->SetRenderType(eRenderType);
			break;
		}//	Switch End
		NULL_CHECK(pImageObject);

		pToolView->m_pObjectMgr->AddObject(OBJECT_ENVIRONMENT, pImageObject);

	}
		
	pImgObj->Die();
	pToolView->m_pTerrain->ImgListUpdate();

	pToolView->m_pImageObject = pImageObject;
	pToolView->Invalidate(FALSE);

}


void CMapTool::OnBnClickedCheckTerrain()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pToolView);

	if (m_ShowCheck[0].GetCheck())
		pToolView->m_bShowTerrain = true;
	else
		pToolView->m_bShowTerrain = false;

	pToolView->Invalidate(FALSE);
}

void CMapTool::OnBnClickedCheckObject()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pToolView);

	if (m_ShowCheck[1].GetCheck())
		pToolView->m_bShowObject = true;
	else
		pToolView->m_bShowObject = false;

	pToolView->Invalidate(FALSE);
}


void CMapTool::OnBnClickedCheckFrontObject()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pToolView);

	if (m_ShowCheck[2].GetCheck())
		pToolView->m_bShowFrontObject = true;
	else
		pToolView->m_bShowFrontObject = false;

	pToolView->Invalidate(FALSE);
}
