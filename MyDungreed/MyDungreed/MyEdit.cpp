#include "stdafx.h"
#include "MyEdit.h"
#include "MyButton.h"
#include "Tile.h"
#include "TileManager.h"
#include "ScrollManager.h"
#include "SelectTile.h"

CMyEdit::CMyEdit() :
	m_iDrawID(0), m_iOptionID(0), m_bSelectTile(false), m_bSelectOptionTile(false), m_bShowButton(false)
{
}


CMyEdit::~CMyEdit()
{
	Release();
}

void CMyEdit::ChangeTile()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	pt.x = pt.x < 0 ? 0 : pt.x > WINCX ? WINCX : pt.x;
	pt.y = pt.y < 0 ? 0 : pt.y > WINCY ? WINCY : pt.y;
	POSITION tMouseWorldPos = CScrollManager::ScreenToWorld(pt);

	int iIndex = GET_SINGLE(CTileManager)->GetTileIndex(static_cast<int>(tMouseWorldPos.fx), static_cast<int>(tMouseWorldPos.fy));

	vector<CTile*>* pVecTile = GET_SINGLE(CTileManager)->GetpVecTile();
	(*pVecTile)[iIndex]->SetDrawID(m_iDrawID);
	(*pVecTile)[iIndex]->SetOptionID(static_cast<TILE_OPTION_ID>(m_iOptionID));
}

void CMyEdit::ShowButtons()
{
	GetCursorPos(&m_tButtonPos);
	ScreenToClient(g_hWnd, &m_tButtonPos);
	m_pTileSelectButton->SetPoint(m_tButtonPos);
	m_pOptionSelectButton->SetPoint(m_tButtonPos.x + m_pTileSelectButton->GetSize().cx, m_tButtonPos.y);
	m_bShowButton = true;
}

void CMyEdit::Initialize()
{
	INSERT_BITMAP("OptionMapTile", L"Image/OptionMapTile(64).bmp");

	m_pTileSelectButton = new CMyButton;
	m_pTileSelectButton->Initialize("SelectTileButton", L"Image/Buttons/SelectTileButtonOff.bmp", "SelectTileButtonOn", L"Image/Buttons/SelectTileButtonOn.bmp");
	
	m_pOptionSelectButton = new CMyButton;
	m_pOptionSelectButton->Initialize("SelectOptionButton", L"Image/Buttons/OptionButtonOff.bmp","SelectOptionButtonOn", L"Image/Buttons/OptionButtonOn.bmp");

	m_pTileSelect = new CSelectTile;
	m_pTileSelect->Initialize("TileSelect(16)", L"Image/MapPickTile.bmp", 16, 16, 512, 512);

	m_pOptionSelect = new CSelectTile;
	m_pOptionSelect->Initialize("OptionSelect(32)", L"Image/OptionMapTile(32).bmp", 32, 32, 448, 32);

	GET_SINGLE(CTileManager)->Initialize();
	CScrollManager::SetScrollX(WINCX / 2);
	CScrollManager::SetScrollY(WINCY / 2);
	CScrollManager::SetScrollSizeX(TILECX * TILEX);
	CScrollManager::SetScrollSizeY(TILECY * TILEY);
	CScrollManager::SetScrollStartX(0);
	CScrollManager::SetScrollStartY(0);
	m_tButtonPos.x = 0;
	m_tButtonPos.y = 0;
}

int CMyEdit::Update()
{
	if (KEYDOWN(VK_RBUTTON)) ShowButtons();

	if (KEYPRESS(VK_CONTROL) && KEYDOWN('S'))	GET_SINGLE(CTileManager)->SaveData();

	if (KEYPRESS(VK_CONTROL) && KEYDOWN('L'))	GET_SINGLE(CTileManager)->LoadData();

	if (KEYPRESS('W'))	CScrollManager::AddScrollY( - 10.f);

	if (KEYPRESS('S'))	CScrollManager::AddScrollY( + 10.f);

	if (KEYPRESS('A'))	CScrollManager::AddScrollX( - 10.f);

	if (KEYPRESS('D'))	CScrollManager::AddScrollX( + 10.f);


	if (!m_bShowButton && !m_bSelectTile && !m_bSelectOptionTile && KEYPRESS(VK_LBUTTON)) ChangeTile();

	if (m_bShowButton)
	{
		if (m_pTileSelectButton->Update()) 
			m_bSelectTile = true;
		
		if (m_pOptionSelectButton->Update()) 
			m_bSelectOptionTile = true;
		
		if (KEYUP(VK_LBUTTON))
			m_bShowButton = false;
	}

	if (m_bSelectTile)
	{
		int iRet = m_pTileSelect->Update();
		if (iRet != -1)
			m_iDrawID = iRet;
		
		if (KEYUP(VK_LBUTTON))
			m_bSelectTile = false;
	}

	if (m_bSelectOptionTile)
	{
		int iRet = m_pOptionSelect->Update();
		if (iRet != -1)
			m_iOptionID = iRet;
		
		if (KEYUP(VK_LBUTTON))
			m_bSelectOptionTile = false;
	}


	CScrollManager::ScrollLock();

	return 0;
}

void CMyEdit::LateUpdate()
{
}

void CMyEdit::Render(HDC hDC)
{
	GET_SINGLE(CTileManager)->RenderForEdit(hDC);
	if (m_bShowButton)
	{
		m_pTileSelectButton->Render(hDC);
		m_pOptionSelectButton->Render(hDC);
	}

	if (m_bSelectTile)		m_pTileSelect->Render(hDC);
	if (m_bSelectOptionTile)m_pOptionSelect->Render(hDC);
	
	string strText = "Draw ID : " + to_string(m_iDrawID);
	TextOutA(hDC, 10, 10, strText.c_str(), strText.length());

	strText = "Option ID : " + to_string(m_iOptionID);
	TextOutA(hDC, 10, 30, strText.c_str(), strText.length());

	strText = "ScrollX : " + to_string(CScrollManager::GetScrollX());
	TextOutA(hDC, 10, 50, strText.c_str(), strText.length());

	strText = "ScrollY : " + to_string(CScrollManager::GetScrollY());
	TextOutA(hDC, 10, 70, strText.c_str(), strText.length());

}

void CMyEdit::Release()
{
	DESTROY_SINGLE(CTileManager);
	SAFE_DELETE(m_pOptionSelectButton);
	SAFE_DELETE(m_pTileSelectButton);
	SAFE_DELETE(m_pTileSelect);
	SAFE_DELETE(m_pOptionSelect);
}
