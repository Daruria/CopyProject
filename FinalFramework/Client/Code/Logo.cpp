#include "stdafx.h"
#include "Logo.h"

#include "Export_Function.h"

#include "PlaneObject.h"
#include "StaticObject.h"

#include "LoadingScene.h"


CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{
	FAILED_CHECK_RETURN(Engine::CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Resource(m_pGraphicDev, Engine::RESOURCE_END), E_FAIL);

	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"Layer_GameLogic"), E_FAIL);

	Engine::Play_BGM(L"mus_mainmenu.OGG");
	//Engine::Play_BGM(L"Credits.wav");
	
	Engine::Set_Volume(CSoundMgr::BGM, 1.f);

	//Engine::Play_Video(Engine::CGraphicDev::GetInstance()->GetSDK(), m_pGraphicDev, NULL, L"..\\Bin\\Video\\Level_17_End.avi");

	return S_OK;
}

_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = CScene::Update_Scene(fTimeDelta);

	if (Engine::KeyDown(DIK_RETURN))
	{
		CLoadingScene* pScene = CLoadingScene::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pScene, -1);

		SetUp_Scene(pScene);
		return 1;
	}

	return iExit;
}

_int CLogo::LateUpdate_Scene(const _float& fTimeDelta)
{
	_int iExit = CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CLogo::Render_Scene(const _float& fTimeDelta)
{
}

HRESULT CLogo::Ready_GameLogic_Layer(const wstring& wstrLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();

	Engine::CGameObject* pObj = nullptr;

	pObj = CPlaneObject::Create(m_pGraphicDev, &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 0.f, 0.f), &_vec3(WINCX, WINCY, 1.f),
		Engine::RESOURCE_STATIC, L"Buffer_RcTex", Engine::RESOURCE_LOGO, L"Texture_Logo", L"MainLogo_BackGround");

	NULL_CHECK_RETURN(pObj, E_FAIL);
	pLayer->Add_GameObject(L"MainLogo_BackGround", pObj);

	m_mapLayer.emplace(wstrLayerTag, pLayer);

	return S_OK;
}

HRESULT CLogo::Ready_Resource(LPDIRECT3DDEVICE9& pGraphicDev, Engine::RESOURCETYPE eMax)
{
	Engine::Reserve_ContainerSize(eMax);

	Engine::CComponent* pCom = Engine::Get_Resources(Engine::RESOURCE_STATIC, L"Buffer_RcTex");
	if (!pCom)
		FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_RcTex",	Engine::BUFFER_RCTEX), E_FAIL);

	pCom = Engine::Get_Resources(Engine::RESOURCE_STATIC, L"Buffer_RcColTex");
	if (!pCom)
		FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_RcColTex", Engine::BUFFER_RCCOLTEX), E_FAIL);

	// ÅØ½ºÃÄ
	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev, Engine::RESOURCE_LOGO, L"Texture_Logo", Engine::TEX_NORMAL,
		L"../Bin/Resources/Texture/MainLogo/DSG_Loading_Title1.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev, Engine::RESOURCE_LOGO, L"Texture_LoadingBG", Engine::TEX_NORMAL,
		L"../Bin/Resources/Texture/Loading/DSG_Loading_Emblem2.tga"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev, Engine::RESOURCE_LOGO, L"Texture_LoadingImage", Engine::TEX_NORMAL,
		L"../Bin/Resources/Texture/Loading/UI_Collection_TrinketBG_Base_0.tga"), E_FAIL);

	return S_OK;
}

CLogo * CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo*	pInst = new CLogo(pGraphicDev);

	if (FAILED(pInst->Ready_Scene()))
		Engine::Safe_Release(pInst);

	return pInst;
}

void CLogo::Free()
{
	CScene::Free();
}

