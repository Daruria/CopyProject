#include "stdafx.h"
#include "PlayerUI.h"
#include "Observer.h"
#include "UI.h"

#include "Export_Function.h"

CPlayerUI::CPlayerUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CPlayerUI::~CPlayerUI()
{
}

HRESULT CPlayerUI::Ready_GameObject(const wstring& wstrName)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_wstrName = wstrName;

	m_pPlayerObserver = Engine::CObserver::Create(L"Player");

	const Engine::CTexture* pTexture = nullptr;
	const Engine::TEX_INFO*	pTexInfo = nullptr;

	_vec2 vHPPos = { 100.f, -50.f };
	m_fHpScale = 0.5f;

	D3DXCOLOR dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_pWarHPFrame	= CUI::Create(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_RcColTex", L"Texture_HPBarFrame", L"UI_WarHPFrame", dwColor);
	NULL_CHECK_RETURN(m_pWarHPFrame, E_FAIL);
	pTexture = dynamic_cast<const Engine::CTexture*>(m_pWarHPFrame->Get_Component(Engine::ID_STATIC, L"Com_Texture"));
	pTexInfo = pTexture->Get_TexInfo();
	m_pWarHPFrame->Set_Scale((_float)pTexInfo->tImgInfo.Width * m_fHpScale, (_float)pTexInfo->tImgInfo.Height * m_fHpScale);
	m_pWarHPFrame->Set_Pos(vHPPos.x - WINCX * 0.5f, vHPPos.y + WINCY * 0.5f);
	m_pWarHPFrame->Set_RcPos(Engine::CRcTex::RTS_LEFT_TOP);
	
	m_pWarHP		= CUI::Create(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_RcColTex", L"Texture_HPBar", L"UI_WarHPBar", dwColor);
	NULL_CHECK_RETURN(m_pWarHP, E_FAIL);
	pTexture = dynamic_cast<const Engine::CTexture*>(m_pWarHP->Get_Component(Engine::ID_STATIC, L"Com_Texture"));
	pTexInfo = pTexture->Get_TexInfo();
	m_pWarHP->Set_Scale((_float)pTexInfo->tImgInfo.Width * m_fHpScale, (_float)pTexInfo->tImgInfo.Height * m_fHpScale);
	m_pWarHP->Set_Pos(vHPPos.x + 32.f * m_fHpScale - WINCX * 0.5f, vHPPos.y - 18.f * m_fHpScale + WINCY * 0.5f);
	m_pWarHP->Set_RcPos(Engine::CRcTex::RTS_LEFT_TOP);
	
	m_pWarPortraitFrame = CUI::Create(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_RcColTex", L"Texture_PortraitFrame", L"UI_WarPortraitFrame", dwColor);
	NULL_CHECK_RETURN(m_pWarPortraitFrame, E_FAIL);
	pTexture = dynamic_cast<const Engine::CTexture*>(m_pWarPortraitFrame->Get_Component(Engine::ID_STATIC, L"Com_Texture"));
	pTexInfo = pTexture->Get_TexInfo();
	m_pWarPortraitFrame->Set_Scale((_float)pTexInfo->tImgInfo.Width * m_fHpScale, (_float)pTexInfo->tImgInfo.Height * m_fHpScale);
	m_pWarPortraitFrame->Set_Pos(100.f - WINCX * 0.5f, -70.f + WINCY * 0.5f);
	m_pWarPortraitFrame->Set_RcPos(Engine::CRcTex::RTS_CENTER_CENTER);

	m_pWarPortrait = CUI::Create(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_RcColTex", L"Texture_WarPortrait", L"UI_WarPortrait", dwColor);
	NULL_CHECK_RETURN(m_pWarPortrait, E_FAIL);
	pTexture = dynamic_cast<const Engine::CTexture*>(m_pWarPortrait->Get_Component(Engine::ID_STATIC, L"Com_Texture"));
	pTexInfo = pTexture->Get_TexInfo();
	m_pWarPortrait->Set_Scale((_float)pTexInfo->tImgInfo.Width * m_fHpScale, (_float)pTexInfo->tImgInfo.Height * m_fHpScale);
	m_pWarPortrait->Set_Pos(104.f - WINCX * 0.5f, -70.f + WINCY * 0.5f);
	m_pWarPortrait->Set_RcPos(Engine::CRcTex::RTS_CENTER_CENTER);

	return S_OK;
}


_int CPlayerUI::Update_GameObject(const _float& fTimeDelta)
{
	m_pWarHP->Update_GameObject(fTimeDelta);
	m_pWarHPFrame->Update_GameObject(fTimeDelta);
	m_pWarPortrait->Update_GameObject(fTimeDelta);
	m_pWarPortraitFrame->Update_GameObject(fTimeDelta);

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	return 0;
}

_int CPlayerUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CPlayerUI::Render_GameObject(const _float& fTimeDelta)
{
	Update_PlayerHP();

	m_pWarHPFrame->Render_GameObject(fTimeDelta);
	m_pWarHP->Render_GameObject(fTimeDelta);
	m_pWarPortraitFrame->Render_GameObject(fTimeDelta);
	m_pWarPortrait->Render_GameObject(fTimeDelta);
}

HRESULT CPlayerUI::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	m_pRendererCom->AddRef();

	return S_OK;
}

void CPlayerUI::Update_PlayerHP()
{
	_int iPlayerHP = 0;
	_int iPlayerHPMax = 0;
	m_pPlayerObserver->Get_Data(SC_INT(EPlayerData::HP), &iPlayerHP);
	m_pPlayerObserver->Get_Data(SC_INT(EPlayerData::HPMAX), &iPlayerHPMax);

	_vec2 vOriginTexSize;
	m_pWarHP->Get_OriginSize(&vOriginTexSize);

	_float fHPRatio = (_float) iPlayerHP / (_float) iPlayerHPMax;
	m_pWarHP->Set_ScaleX(vOriginTexSize.x * fHPRatio * m_fHpScale);
	m_pWarHP->Set_U(fHPRatio);
	
	
}

CPlayerUI* CPlayerUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CPlayerUI* pInst = new CPlayerUI(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wstrName)))
		::Safe_Release(pInst);

	return pInst;
}

void CPlayerUI::Free()
{
	::Safe_Release(m_pWarHP);
	::Safe_Release(m_pWarHPFrame);
	::Safe_Release(m_pWarPortrait);
	::Safe_Release(m_pWarPortraitFrame);
	::Safe_Release(m_pPlayerObserver);

	Engine::CGameObject::Free();
}
