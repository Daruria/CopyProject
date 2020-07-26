#include "stdafx.h"
#include "MainApp.h"

#include "Export_Function.h"
#include "CollisionFunctions.h"
#include "Logo.h"
#include "Mouse.h"
#include "PlayerPool.h"

CMainApp::CMainApp()
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp()
{
	//::ShowCursor(FALSE);

	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_VideoPlayer(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagement), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_CameraMgr(m_pGraphicDev, WINCX, WINCY), E_FAIL);
	FAILED_CHECK_RETURN(CCollisionFunctions::Ready_Functions(), E_FAIL);

	m_pMouse = CMouse::Create(m_pGraphicDev);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	//	Ãà¼Ò ½Ã ¿É¼Ç. 
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//	È®´ë ½Ã ¿É¼Ç.
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

_int CMainApp::Update_MainApp(const _float & fTimeDelta)
{
	m_iExit = 0;

	Engine::Set_InputDev(fTimeDelta);

	if (nullptr != m_pManagement)
		m_iExit = m_pManagement->Update_Scene(fTimeDelta);

	if (1 == m_iExit)
		return m_iExit;

	Engine::Update_MainCamera(fTimeDelta);

	m_pMouse->Update_GameObject(fTimeDelta);

	//Engine::Update_Sound();

	return m_iExit;
}

_int CMainApp::LateUpdate_MainApp(const _float & fTimeDelta)
{
	if (1 == m_iExit)
		return m_iExit;


	if (nullptr != m_pManagement)
		m_iExit = m_pManagement->LateUpdate_Scene(fTimeDelta);

	if (1 == m_iExit)
		return m_iExit;

	m_iExit = Engine::Collision(fTimeDelta);

	if (1 == m_iExit)
		return m_iExit;

	m_iExit = CPlayerPool::GetInstance()->LateUpdate_PlayerPool(fTimeDelta);

	if (1 == m_iExit)
		return m_iExit;

	return 0;
}

void CMainApp::Render_MainApp(const _float & fTimeDelta)
{
	if (1 == m_iExit)
		return ;

	Engine::Render_Begin(D3DXCOLOR(181.f/255.f, 214.f/255.f, 146.f/255.f, 1.f));

	if (nullptr != m_pManagement)
		m_pManagement->Render_Scene(fTimeDelta);

	if (Engine::Is_VideoOpen())
	{
		Engine::Render_Video();
	}
	
	Engine::Render_End();
}

HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, Engine::MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);

	m_pDeviceClass->AddRef();

	(*ppGraphicDev) = m_pDeviceClass->GetDevice();
	(*ppGraphicDev)->AddRef();

	// FONT
	wstring wstrFontType	= L"±Ã¼­";
	wstring wstrFontType1	= L"¹ÙÅÁ";
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Default",	L"±Ã¼­", 20, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Second",	L"¸¼Àº °íµñ", 11, 35, FW_THIN), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Third",	L"¸¼Àº °íµñ", 11, 35, FW_THIN), E_FAIL);

	// INPUTDEV
	FAILED_CHECK_RETURN(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

	//	SoundMgr
	FAILED_CHECK_RETURN(Engine::Initialize_Sound(), E_FAIL);

	return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 & pGraphicDev, Engine::CManagement ** ppManagement)
{
	Engine::CScene*		pScene = nullptr;

	FAILED_CHECK_RETURN(Engine::Create_Management(pGraphicDev, ppManagement), E_FAIL);
	(*ppManagement)->AddRef();

	pScene = CLogo::Create(pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	FAILED_CHECK_RETURN((*ppManagement)->SetUp_Scene(pScene), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Renderer(pGraphicDev), E_FAIL);

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp* pInst = new CMainApp;

	if (FAILED(pInst->Ready_MainApp()))
		Safe_Release(pInst);

	return pInst;
}

void CMainApp::Free()
{
	//::ShowCursor(TRUE);
	
	CPlayerPool::GetInstance()->DestroyInstance();
	Engine::Safe_Release(m_pMouse);
	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pDeviceClass);
	Engine::Safe_Release(m_pManagement);

	Engine::Release_Utility();
	Engine::Release_System();
}
