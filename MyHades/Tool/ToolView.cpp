
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "MainFrm.h"
#include "ToolDoc.h"
#include "ToolView.h"
#include "MyForm.h"
#include "MapTool.h"
#include "Terrain.h"
#include "ImageObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_MBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_MOVE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// ���� ����
HWND g_hWnd;

// CToolView ����/�Ҹ�

CToolView::CToolView() :
	m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pTerrain(nullptr),
	m_pImageObject(nullptr),
	m_bShowFrontObject(true),
	m_bShowObject(true),
	m_bShowTerrain(true)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CToolView::~CToolView()
{
	SafeDelete(m_pTerrain);
	m_pObjectMgr->DestroyInstance();
	m_pTextureMgr->DestroyInstance();
	m_pDeviceMgr->DestroyInstance();

}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView �׸���

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.

	m_pDeviceMgr->Render_Begin();

	m_pObjectMgr->Update();
	m_pObjectMgr->LateUpdate();

	m_pTerrain->LateUpdate();

	if (m_bShowTerrain)
		m_pTerrain->Render();

	if (m_bShowFrontObject || m_bShowObject)
		m_pObjectMgr->Render();

	DrawSelImageLine();

	m_pDeviceMgr->Render_End();

}


// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����


void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class

	g_hWnd = m_hWnd;

	int iCX = TILEX * TILECX;
	int iCY = TILEY * TILECY / 2;

	CScrollView::SetScrollSizes(MM_TEXT, CSize(iCX, iCY));

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK_MSG(pFrameWnd, L"pFrameWnd is null");

	RECT rcFrame = {};

	pFrameWnd->GetWindowRect(&rcFrame);
	::SetRect(&rcFrame, 0, 0, rcFrame.right - rcFrame.left, rcFrame.bottom - rcFrame.top);

	RECT rcView = {};

	// �������� Ŭ���̾�Ʈ ����(�׵θ��� �������� ����) ũ�� ������ �Լ�. (Ŭ���̾�Ʈ ��ǥ ����)
	GetClientRect(&rcView);

	int iGapX = rcFrame.right - rcView.right;
	int iGapY = rcFrame.bottom - rcView.bottom;

	// �������������� ũ�⸦ ���� ����.
	pFrameWnd->SetWindowPos(nullptr, 0, 0, WINCX + iGapX, WINCY + iGapY, SWP_NOZORDER | SWP_NOMOVE);

	HRESULT hr = m_pDeviceMgr->InitDevice(MODE_WIN);
	FAILED_CHECK_MSG(hr, L"InitDevice Failed");

	LPDIRECT3DDEVICE9 pGraphicDev = m_pDeviceMgr->GetDevice();
	NULL_CHECK_MSG(pGraphicDev, L"pGraphicDev is null");

	m_pTerrain = CTerrain::Create(this);
	NULL_CHECK_MSG(m_pTerrain, L"Terrain Create Failed");

	SetScrollPos(0, TILECX * TILEX / 2 - WINCX / 2);
	SetScrollPos(1, TILECY * TILEY / 4 - WINCY / 2);

}

void CToolView::OptionLButtonFunc(const D3DXVECTOR3& vPoint, CMyForm* pMyForm)
{
	//	Option ���¿��� ��Ŭ�� �Լ�
	int iOptionType = pMyForm->m_MapTool.GetCurOptionType();
	
	CTerrain::TILE_OPTION eOption = static_cast<CTerrain::TILE_OPTION>(iOptionType);

	m_pTerrain->TileChange(vPoint, 0, eOption);

	Invalidate(FALSE);
}

void CToolView::ImageObjectLButtonFunc(const D3DXVECTOR3& vPoint, CMyForm* pMyForm)
{
	//	ImageObject ���¿��� ��Ŭ�� �Լ�
	NULL_CHECK(m_pImageObject);

	m_pImageObject->SetPos(vPoint);
	pMyForm->m_MapTool.m_fPositionX = vPoint.x;
	pMyForm->m_MapTool.m_fPositionY = vPoint.y;
	pMyForm->m_MapTool.UpdateData(FALSE);

	Invalidate(FALSE);
}

void CToolView::DrawSelImageLine()
{
	NULL_CHECK(m_pImageObject);
	
	m_pDeviceMgr->GetSprite()->End();

	const TEX_INFO* pTexInfo = m_pImageObject->GetTexInfo();
	NULL_CHECK(pTexInfo);

	INFO tInfo = m_pImageObject->GetInfo();
	
	tInfo.vPos.x -= GetScrollPos(0);
	tInfo.vPos.y -= GetScrollPos(1);

	D3DXVECTOR2 vPoint[5]
	{
		D3DXVECTOR2(tInfo.vPos.x - pTexInfo->tImgInfo.Width * 0.5f, tInfo.vPos.y - pTexInfo->tImgInfo.Height * 0.5f),
		D3DXVECTOR2(tInfo.vPos.x + pTexInfo->tImgInfo.Width * 0.5f, tInfo.vPos.y - pTexInfo->tImgInfo.Height * 0.5f),
		D3DXVECTOR2(tInfo.vPos.x + pTexInfo->tImgInfo.Width * 0.5f, tInfo.vPos.y + pTexInfo->tImgInfo.Height * 0.5f),
		D3DXVECTOR2(tInfo.vPos.x - pTexInfo->tImgInfo.Width * 0.5f, tInfo.vPos.y + pTexInfo->tImgInfo.Height * 0.5f),
		D3DXVECTOR2(tInfo.vPos.x - pTexInfo->tImgInfo.Width * 0.5f, tInfo.vPos.y - pTexInfo->tImgInfo.Height * 0.5f)
	};

	m_pDeviceMgr->GetLine()->SetWidth(1.f); // ���� ����
	m_pDeviceMgr->GetLine()->Draw(vPoint, 5, D3DCOLOR_ARGB(255, 255, 0, 255));

	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	D3DXVECTOR3 vPoint =
	{
		(float)point.x + CScrollView::GetScrollPos(0),
		(float)point.y + CScrollView::GetScrollPos(1),
		0.f
	};

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CMyForm* pMyForm = dynamic_cast<CMyForm*>(pFrameWnd->m_MainSplitter.GetPane(0, 0));
	NULL_CHECK(pMyForm);
	
	int iOptionImgRadio = pMyForm->m_MapTool.GetCurOptionImageRadio();	//	0 : Option, 1 : ImageObject

	switch (iOptionImgRadio)
	{
	case 0:	//	Option
		OptionLButtonFunc(vPoint, pMyForm);
		break;
	case 1:	//	ImageObject
		ImageObjectLButtonFunc(vPoint, pMyForm);
		break;
	}


	CScrollView::OnLButtonDown(nFlags, point);
}


void CToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CMyForm* pMyForm = dynamic_cast<CMyForm*>(pFrameWnd->m_MainSplitter.GetPane(0, 0));
	NULL_CHECK(pMyForm);

	D3DXVECTOR3 vPoint =
	{
		(float)point.x + CScrollView::GetScrollPos(0),
		(float)point.y + CScrollView::GetScrollPos(1),
		0.f
	};

	int iPushList = pMyForm->m_MapTool.GetCurPushList();

	float fDist = 10000.f;
	float fTemp = 0.f;
	D3DXVECTOR3 vDist = {};

	if (0 == iPushList)
	{
		list<CImageObject*>* pImgList = m_pTerrain->GetImgList();
		for (auto& pImgObj : *pImgList)
		{
			vDist = pImgObj->GetInfo().vPos - vPoint;
			fTemp = D3DXVec3Length(&vDist);
			if (fDist > fTemp)
			{
				fDist = fTemp;
				m_pImageObject = pImgObj;
			}
		}//	For
	}//	if (0 == iPushList)
	else if (1 == iPushList || 2 == iPushList)
	{
		list<CGameObject*>* pObjectList = m_pObjectMgr->GetObjectList(OBJECT_ENVIRONMENT);
		for (auto& pImgObj : *pObjectList)
		{
			vDist = pImgObj->GetInfo().vPos - vPoint;
			fTemp = D3DXVec3Length(&vDist);
			if (fDist > fTemp)
			{
				fDist = fTemp;
				m_pImageObject = dynamic_cast<CImageObject*>(pImgObj);
			}
		}//	For
	}//	else if (1 == iPushList || 2 == iPushList)

	NULL_CHECK(m_pImageObject);

	pMyForm->m_MapTool.m_fPositionX = m_pImageObject->GetInfo().vPos.x;
	pMyForm->m_MapTool.m_fPositionY = m_pImageObject->GetInfo().vPos.y;

	pMyForm->m_MapTool.m_ObjectScaleX = m_pImageObject->GetInfo().vScale.x;
	pMyForm->m_MapTool.m_ObjectScaleY = m_pImageObject->GetInfo().vScale.y;

	pMyForm->m_MapTool.m_fSort = m_pImageObject->GetSort();

	pMyForm->m_MapTool.UpdateData(FALSE);

	Invalidate(FALSE);

	CScrollView::OnRButtonDown(nFlags, point);
}


void CToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	NULL_CHECK(m_pImageObject);

	if (VK_DELETE == nChar)
	{
		m_pImageObject->Die();

		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		CMyForm* pMyForm = dynamic_cast<CMyForm*>(pFrameWnd->m_MainSplitter.GetPane(0, 0));
		NULL_CHECK(pMyForm);

		int iPushList = pMyForm->m_MapTool.GetCurPushList();
		if (0 == iPushList)
			m_pTerrain->ImgListUpdate();
		
		m_pImageObject = nullptr;
		Invalidate(FALSE);
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		D3DXVECTOR3 vPoint =
		{
			(float)point.x + CScrollView::GetScrollPos(0),
			(float)point.y + CScrollView::GetScrollPos(1),
			0.f
		};

		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		CMyForm* pMyForm = dynamic_cast<CMyForm*>(pFrameWnd->m_MainSplitter.GetPane(0, 0));
		NULL_CHECK(pMyForm);

		int iOptionImgRadio = pMyForm->m_MapTool.GetCurOptionImageRadio();	//	0 : Option, 1 : ImageObject

		switch (iOptionImgRadio)
		{
		case 0:	//	Option
			OptionLButtonFunc(vPoint, pMyForm);
			break;
		case 1:	//	ImageObject
			ImageObjectLButtonFunc(vPoint, pMyForm);
			break;
		}
	}


	CScrollView::OnMouseMove(nFlags, point);
}
