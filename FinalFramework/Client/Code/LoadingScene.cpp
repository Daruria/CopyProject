#include "stdafx.h"
#include "LoadingScene.h"

#include "Export_Function.h"

#include "PlaneObject.h"
#include "StaticObject.h"

#include "Stage.h"


CLoadingScene::CLoadingScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CLoadingScene::~CLoadingScene()
{
}

HRESULT CLoadingScene::Ready_Scene()
{
	FAILED_CHECK_RETURN(Engine::CScene::Ready_Scene(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_UI_Layer(L"Layer_UI"), E_FAIL);

	m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_STAGE);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	return S_OK;
}

_int CLoadingScene::Update_Scene(const _float& fTimeDelta)
{
	m_pCircleTransform->Rotation(ROT_Z, D3DXToRadian(10.f * fTimeDelta));
	_int iExit = CScene::Update_Scene(fTimeDelta);

	if (m_pLoading->Get_Finish())
	{
		CStage* pScene = CStage::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pScene, -1);

		Engine::SetUp_Scene(pScene);

		return 1;
	}

	return iExit;
}

_int CLoadingScene::LateUpdate_Scene(const _float& fTimeDelta)
{
	_int iExit = CScene::LateUpdate_Scene(fTimeDelta);

	
	return iExit;
}

void CLoadingScene::Render_Scene(const _float& fTimeDelta)
{
	Engine::Render_Font(L"Font_Second", m_pLoading->Get_LoadingText(), &_vec2(WINCX * 0.5f - 100.f, WINCY - 150.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

HRESULT CLoadingScene::Ready_UI_Layer(const wstring & wstrLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();

	Engine::CGameObject* pObj = nullptr;


	Engine::CResources* pResources = Engine::Get_Resources(Engine::RESOURCE_LOGO, L"Texture_LoadingBG");
	Engine::CTexture* pTexture = dynamic_cast<Engine::CTexture*>(pResources);
	const Engine::TEX_INFO* pTexInfo = pTexture->Get_TexInfo();

	pObj = CPlaneObject::Create(m_pGraphicDev, 
								&_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 0.f, 0.f), 
								&_vec3((_float)pTexInfo->tImgInfo.Width, (_float)pTexInfo->tImgInfo.Height, 1.f),
								Engine::RESOURCE_STATIC,	L"Buffer_RcTex", 
								Engine::RESOURCE_LOGO, 		L"Texture_LoadingBG", L"LoadingScene_BackGround");

	NULL_CHECK_RETURN(pObj, E_FAIL);
	pLayer->Add_GameObject(L"LoadingScene_BackGround", pObj);

	pResources = Engine::Get_Resources(Engine::RESOURCE_LOGO, L"Texture_LoadingImage");
	pTexture = dynamic_cast<Engine::CTexture*>(pResources);
	pTexInfo = pTexture->Get_TexInfo();
	pObj = CPlaneObject::Create(m_pGraphicDev,
								&_vec3(-200.f, -400.f, 0.f), &_vec3(0.f, 0.f, 0.f),
								&_vec3((_float)pTexInfo->tImgInfo.Width, (_float)pTexInfo->tImgInfo.Height, 1.f), 
								Engine::RESOURCE_STATIC, L"Buffer_RcTex", 
								Engine::RESOURCE_LOGO, L"Texture_LoadingImage", L"LoadingScene_Cicle");
	NULL_CHECK_RETURN(pObj, E_FAIL);
	pLayer->Add_GameObject(L"LoadingScene_BackGround", pObj);
	m_pCircleTransform = dynamic_cast<Engine::CTransform*>(pObj->Get_Component(Engine::ID_DYNAMIC, L"Com_Transform"));
	m_pCircleTransform->AddRef();

	m_mapLayer.emplace(wstrLayerTag, pLayer);

	return S_OK;
}

CLoadingScene * CLoadingScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLoadingScene*	pInst = new CLoadingScene(pGraphicDev);

	if (FAILED(pInst->Ready_Scene()))
		Engine::Safe_Release(pInst);

	return pInst;
}

void CLoadingScene::Free()
{
	::Safe_Release(m_pCircleTransform);
	Engine::Safe_Release(m_pLoading);
	CScene::Free();
}

