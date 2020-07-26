#include "stdafx.h"
#include "BossUI.h"
#include "Observer.h"
#include "UI.h"

#include "Export_Function.h"

CBossUI::CBossUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CBossUI::~CBossUI()
{
}

HRESULT CBossUI::Ready_GameObject(const wstring& wstrName)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_wstrName = wstrName;

	m_pBossObserver = Engine::CObserver::Create(L"Boss");

	m_fHpScale = 1.5f;

	const Engine::CTexture* pTexture = nullptr;
	const Engine::TEX_INFO*	pTexInfo = nullptr;


	m_pBossHPFrame	= CUI::Create(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_RcColTex", L"Texture_BossHPBarFrame", L"UI_BossHPFrame", D3DXCOLOR(1.f,1.f,1.f,1.f));
	NULL_CHECK_RETURN(m_pBossHPFrame, E_FAIL);
	pTexture = dynamic_cast<const Engine::CTexture*>(m_pBossHPFrame->Get_Component(Engine::ID_STATIC, L"Com_Texture"));
	pTexInfo = pTexture->Get_TexInfo();

	_vec2 vFrameSize = { (_float)pTexInfo->tImgInfo.Width * 1.88f * m_fHpScale, (_float)pTexInfo->tImgInfo.Height * 1.5f * m_fHpScale };
	_vec2 vHPPos = { -vFrameSize.x * 0.5f, -150.f };

	m_pBossHPFrame->Set_Scale(vFrameSize.x, vFrameSize.y);
	m_pBossHPFrame->Set_Pos(vHPPos.x , vHPPos.y + WINCY * 0.5f);
	m_pBossHPFrame->Set_RcPos(Engine::CRcTex::RTS_LEFT_TOP);
	
	m_pBossHP		= CUI::Create(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_RcColTex", L"Texture_BossHPBar", L"UI_BossHPBar", D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	NULL_CHECK_RETURN(m_pBossHP, E_FAIL);
	pTexture = dynamic_cast<const Engine::CTexture*>(m_pBossHP->Get_Component(Engine::ID_STATIC, L"Com_Texture"));
	pTexInfo = pTexture->Get_TexInfo();
	m_pBossHP->Set_Scale((_float)pTexInfo->tImgInfo.Width * m_fHpScale, (_float)pTexInfo->tImgInfo.Height * m_fHpScale);
	m_pBossHP->Set_Pos(vHPPos.x + 10.f * m_fHpScale, vHPPos.y -10.f * m_fHpScale + WINCY * 0.5f);
	m_pBossHP->Set_RcPos(Engine::CRcTex::RTS_LEFT_TOP);


	return S_OK;
}


_int CBossUI::Update_GameObject(const _float& fTimeDelta)
{

	//TCHAR szTest[64] = L"";
	//GetPrivateProfileString(L"Test", L"X", nullptr, szTest, 64, L"../../Test.ini");
	//float fX = _wtof(szTest);
	//GetPrivateProfileString(L"Test", L"Y", nullptr, szTest, 64, L"../../Test.ini");
	//float fY = _wtof(szTest);
	//
	//_vec3 vPos = { fX, fY, 0.f};

	//m_pBossHP->Set_PosX(fX + 10.f);
	//m_pBossHP->Set_PosY(fY - 10.f);

	//m_pBossHPFrame->Set_PosX(fX );
	//m_pBossHPFrame->Set_PosY(fY );

	m_pBossHP->Update_GameObject(fTimeDelta);
	m_pBossHPFrame->Update_GameObject(fTimeDelta);

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	return 0;
}

_int CBossUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	_int iExit = Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return 0;
}

void CBossUI::Render_GameObject(const _float& fTimeDelta)
{
	Update_BossHP();

	m_pBossHPFrame->Render_GameObject(fTimeDelta);
	m_pBossHP->Render_GameObject(fTimeDelta);
}

HRESULT CBossUI::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	m_pRendererCom->AddRef();

	return S_OK;
}

void CBossUI::Update_BossHP()
{
	_int iPlayerHP = 0;
	_int iPlayerHPMax = 0;
	m_pBossObserver->Get_Data(SC_INT(EPlayerData::HP), &iPlayerHP);
	m_pBossObserver->Get_Data(SC_INT(EPlayerData::HPMAX), &iPlayerHPMax);

	_vec2 vOriginTexSize;
	m_pBossHP->Get_OriginSize(&vOriginTexSize);

	_float fHPRatio = (_float) iPlayerHP / (_float) iPlayerHPMax;
	m_pBossHP->Set_ScaleX(vOriginTexSize.x * fHPRatio * m_fHpScale);
	m_pBossHP->Set_U(fHPRatio);
	
	
}

CBossUI* CBossUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CBossUI* pInst = new CBossUI(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wstrName)))
		::Safe_Release(pInst);

	return pInst;
}

void CBossUI::Free()
{
	::Safe_Release(m_pBossHP);
	::Safe_Release(m_pBossHPFrame);
	::Safe_Release(m_pBossObserver);

	Engine::CGameObject::Free();
}
