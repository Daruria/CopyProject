#include "stdafx.h"
#include "InvenSlot.h"
#include "Item.h"

CInvenSlot::CInvenSlot()
{
}

CInvenSlot::~CInvenSlot()
{
	Release();
}

void CInvenSlot::Initialize(const string & strKey, const wchar_t * pPath, const string & strOnKey, const wchar_t * pOnPath)
{
	CMyButton::Initialize(strKey, pPath, strOnKey, pOnPath);
	m_bIconRender = true;
	m_pItem = nullptr;
}

int CInvenSlot::Update()
{
	m_bIsOnMouse = IsOnMouse() ? true : false;
	return m_bIsOnMouse;
}

void CInvenSlot::LateUpdate()
{
	CMyButton::LateUpdate();
}

void CInvenSlot::Render(HDC hDC)
{
	CMyButton::Render(hDC);
	if (m_pItem && m_bIconRender)
		m_pItem->IconRender(hDC, static_cast<float>(m_tPoint.x), static_cast<float>(m_tPoint.y));
}

void CInvenSlot::Release()
{
	SAFE_DELETE(m_pItem);
}
