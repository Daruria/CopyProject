#include "stdafx.h"
#include "MyButton.h"
#include "Mouse.h"

CMyButton::CMyButton():
	m_bIsOn(false),
	m_eType(BUTTON_END),
	m_wstrFont(L""),
	m_wstrObjectKey(L""),
	m_wstrStateKey(L"")
{
	ZeroMemory(&m_tSize, sizeof(SIZE));
}


CMyButton::~CMyButton()
{
	Release();
}

int CMyButton::Update()
{
	m_bIsOn = IsOn();

	m_pObjectMgr->AddLayer(m_eRenderType, this);

	return m_bIsOn;
}

void CMyButton::LateUpdate()
{
	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_tInfo.vScale.x, m_tInfo.vScale.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	m_tInfo.matWorld = matScale * matTrans;
}

void CMyButton::Render()
{
	const TEX_INFO* pTexInfo = nullptr;

	LPD3DXSPRITE pSprite = m_pDeviceMgr->GetSprite();
	pSprite->SetTransform(&m_tInfo.matWorld);
	D3DXVECTOR3 vCenter = {};
	switch (m_eType)
	{
	case CMyButton::IMAGE_ONLY:
		pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjectKey, GetState(), 0);
		vCenter = { pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height*0.5f, 0.f };
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		break;
	case CMyButton::FONT_ONLY:
		m_pDeviceMgr->GetFont()->DrawTextW(pSprite, m_wstrFont.c_str(), m_wstrFont.size(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		break;
	case CMyButton::IMAGE_AND_FONT:
		pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjectKey, GetState(), 0);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pDeviceMgr->GetFont()->DrawTextW(pSprite, m_wstrFont.c_str(), m_wstrFont.size(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		break;
	}


}

HRESULT CMyButton::Initialize()
{
	m_tInfo.vDir = {1.f, 0.f, 0.f};
	m_tInfo.vLook = {1.f, 0.f, 0.f};

	

	return S_OK;
}

void CMyButton::Release()
{
}

CMyButton * CMyButton::Create(BUTTON_TYPE eType, const wstring & wstrObjectKey, float fx, float fy, const wstring& wstrFont, float fScale)
{
	CMyButton* pInstance = new CMyButton;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	
	const TEX_INFO* pTexInfo = nullptr;

	pInstance->m_tInfo.vScale = {fScale, fScale, 0.f};
	pInstance->m_tInfo.vPos = { fx, fy, 0.f };
	pInstance->m_eType = eType;
	switch (eType)
	{
	case CMyButton::IMAGE_ONLY:
		pInstance->m_wstrObjectKey = wstrObjectKey;
		pTexInfo = pInstance->m_pTextureMgr->GetTexInfo(wstrObjectKey, L"Off", 0);
		pInstance->m_tSize = { static_cast<LONG>(pTexInfo->tImgInfo.Width), static_cast<LONG>(pTexInfo->tImgInfo.Height) };
		break;
	case CMyButton::FONT_ONLY:
		pInstance->m_wstrFont = wstrFont;
		break;
	case CMyButton::IMAGE_AND_FONT:
		pInstance->m_wstrObjectKey = wstrObjectKey;
		pTexInfo = pInstance->m_pTextureMgr->GetTexInfo(wstrObjectKey, L"Off", 0);
		pInstance->m_tSize = { static_cast<LONG>(pTexInfo->tImgInfo.Width), static_cast<LONG>(pTexInfo->tImgInfo.Height) };
		pInstance->m_wstrFont = wstrFont;
		break;
	}


	return pInstance;
}

bool CMyButton::IsOn()
{
	D3DXVECTOR3 vMouse = CMouse::GetInstance()->GetMousePos();

	RECT rc =
	{
		static_cast<LONG>(m_tInfo.vPos.x - m_tSize.cx * 0.5f),
		static_cast<LONG>(m_tInfo.vPos.y - m_tSize.cy * 0.5f),
		static_cast<LONG>(m_tInfo.vPos.x + m_tSize.cx * 0.5f),
		static_cast<LONG>(m_tInfo.vPos.y + m_tSize.cy * 0.5f)
	};

	if (rc.left		> vMouse.x) return false;
	if (rc.right	< vMouse.x)	return false;
	if (rc.top		> vMouse.y)	return false;
	if (rc.bottom	< vMouse.y)	return false;

	return true;
}

wstring CMyButton::GetState()
{
	// TODO: insert return statement here

	return m_bIsOn ? L"On" : L"Off";
}
