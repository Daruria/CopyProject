#include "stdafx.h"
#include "MainGame.h"
#include "Mouse.h"

DEFINITION_SINGLE(CMainGame);

CMainGame::CMainGame()
{
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	srand(unsigned(time(nullptr)));

	m_iFrame = 0;
	m_iMaxFrame = 0;
	m_dwFrameTime = GetTickCount();
	GET_SINGLE(CMouse)->Initialize();
	GET_SINGLE(CBitmapManager)->Initialize();
	GET_SINGLE(CSceneManager)->Initialize();
	GET_SINGLE(CKeyManager)->Initialize();
	GET_SINGLE(CObjectManager)->Initialize();
	GET_SINGLE(CEffectManager)->Initialize();
	GET_SINGLE(CBitmapManager)->InsertBitmap("BackBuffer", L"Image/BackBuffer.bmp");
	GET_SINGLE(CFontManager)->Initialize();

	GET_SINGLE(CPlayerUI)->Initialize();
	GET_SINGLE(CInventory)->Initialize();
	GET_SINGLE(CResult)->Initialize();
	GET_SINGLE(CSoundMgr)->Initialize();
	
}

int CMainGame::Update()
{
	GET_SINGLE(CSoundMgr)->UpdateSound();
	GET_SINGLE(CMouse)->Update();
	GET_SINGLE(CSceneManager)->Update();

	return 0;
}

void CMainGame::LateUpdate()
{
	GET_SINGLE(CKeyManager)->KeyUpdate();
	GET_SINGLE(CSceneManager)->LateUpdate();
}

void CMainGame::Render(HDC hDC)
{
	HDC hMemDC = GET_SINGLE(CBitmapManager)->FindBitmapDC("BackBuffer");

	Rectangle(hMemDC, -1, -1, WINCX + 1, WINCY + 1);

	GET_SINGLE(CSceneManager)->Render(hMemDC);

#ifdef _DEBUG
	m_iFrame++;
	if (m_dwFrameTime + 1000 < GetTickCount())
	{
		m_iMaxFrame = m_iFrame;
		m_iFrame = 0;
		m_dwFrameTime = GetTickCount();
	}
	//string strText = "FPS : " + to_string(m_iMaxFrame);
	//TextOutA(hMemDC, 10, 10, strText.c_str(), strText.length());
#endif

	//m_iFrame++;
	//if (m_dwFrameTime + 1000 < GetTickCount())
	//{
	//	m_iMaxFrame = m_iFrame;
	//	m_iFrame = 0;
	//	m_dwFrameTime = GetTickCount();
	//}
	//string strText = "FPS : " + to_string(m_iMaxFrame);
	//TextOutA(hMemDC, 10, 10, strText.c_str(), strText.length());


#ifdef _DEBUG
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	POSITION tPos = CScrollManager::ScreenToWorld((float)pt.x, (float)pt.y);

	string strText = "MouseCursor X : " + to_string(tPos.fx);
	TextOutA(hMemDC, 10, 130, strText.c_str(), strText.length());

	strText = "MouseCursor Y : " + to_string(tPos.fy);
	TextOutA(hMemDC, 10, 150, strText.c_str(), strText.length());
#endif // _DEBUG

	GET_SINGLE(CResult)->Render(hMemDC);

	GET_SINGLE(CMouse)->Render(hMemDC);


	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CMainGame::Release()
{
	DESTROY_SINGLE(CResult);
	DESTROY_SINGLE(CFontManager);
	DESTROY_SINGLE(CPlayerUI);
	DESTROY_SINGLE(CInventory);
	DESTROY_SINGLE(CEffectManager);
	DESTROY_SINGLE(CKeyManager);
	DESTROY_SINGLE(CSceneManager);
	DESTROY_SINGLE(CObjectManager);
	DESTROY_SINGLE(CBitmapManager);
	DESTROY_SINGLE(CMouse);
	DESTROY_SINGLE(CSoundMgr);
}
