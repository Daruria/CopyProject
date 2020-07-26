#include "stdafx.h"
#include "ToolScene.h"

CToolScene::CToolScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CToolScene::~CToolScene()
{
}

HRESULT CToolScene::Ready_Scene()
{
	return S_OK;
}

_int CToolScene::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = CScene::Update_Scene(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return iExit;
}

_int CToolScene::LateUpdate_Scene(const _float& fTimeDelta)
{
	_int iExit = CScene::LateUpdate_Scene(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	return iExit;
}

void CToolScene::Render_Scene(const _float& fTimeDelta)
{
	
}

HRESULT CToolScene::Ready_EnvironmentLayer(const wstring & wstrLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_mapLayer.emplace(wstrLayerTag, pLayer);

	return S_OK;
}

HRESULT CToolScene::Ready_GameLogicLayer(const wstring & wstrLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_mapLayer.emplace(wstrLayerTag, pLayer);

	return S_OK;
}

HRESULT CToolScene::SaveData(const wstring & wstrPath)
{
	return S_OK;
}

HRESULT CToolScene::LoadData(const wstring & wstrPath)
{
	return S_OK;
}

CToolScene * CToolScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CToolScene* pInst = new CToolScene(pGraphicDev);

	if (FAILED(pInst->Ready_Scene()))
		Safe_Release(pInst);

	return pInst;
}

void CToolScene::Free()
{
	CScene::Free();
}

