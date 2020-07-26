#include "stdafx.h"
#include "EndingScene.h"

#include "Export_Function.h"

#include "LoadingScene.h"
#include "Logo.h"


CEndingScene::CEndingScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CEndingScene::~CEndingScene()
{
}

HRESULT CEndingScene::Ready_Scene()
{
	FAILED_CHECK_RETURN(Engine::CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"Layer_GameLogic"), E_FAIL);

	Engine::Stop_All();

	Engine::Play_Video(Engine::CGraphicDev::GetInstance()->GetSDK(), m_pGraphicDev, NULL, L"..\\Bin\\Video\\Level_17_End.avi");

	m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_CLEAR);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	return S_OK;
}

_int CEndingScene::Update_Scene(const _float& fTimeDelta)
{
	if (!Engine::Is_VideoOpen())
	{
		CLogo* pLogo = CLogo::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pLogo, -1);
		FAILED_CHECK_RETURN(Engine::SetUp_Scene(pLogo), -1);
		return 1;
	}

	if (m_pLoading->Get_Finish() && Engine::KeyDown(DIK_RETURN))
	{
		Engine::Stop_Video();

		CLogo* pLogo = CLogo::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pLogo, -1);
		FAILED_CHECK_RETURN(Engine::SetUp_Scene(pLogo), -1);

		return 1;
	}

	_int iExit = CScene::Update_Scene(fTimeDelta);

	return iExit;
}

_int CEndingScene::LateUpdate_Scene(const _float& fTimeDelta)
{
	_int iExit = CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CEndingScene::Render_Scene(const _float& fTimeDelta)
{
}

HRESULT CEndingScene::Ready_GameLogic_Layer(const wstring& wstrLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();

	Engine::CGameObject* pObj = nullptr;

	m_mapLayer.emplace(wstrLayerTag, pLayer);

	return S_OK;
}


CEndingScene * CEndingScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEndingScene*	pInst = new CEndingScene(pGraphicDev);

	if (FAILED(pInst->Ready_Scene()))
		Engine::Safe_Release(pInst);

	return pInst;
}

void CEndingScene::Free()
{
	::Safe_Release(m_pLoading);
	CScene::Free();
}

