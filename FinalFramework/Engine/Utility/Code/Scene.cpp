#include "Scene.h"
USING(Engine)

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CScene::~CScene()
{

}

const CComponent * CScene::Get_Component(const wstring& wstrLayerTag, const wstring& wstrObjTag, const wstring& wstrComponentTag, COMPONENTID eID) const
{
	auto iter = m_mapLayer.find(wstrLayerTag);

	if (m_mapLayer.end() == iter)
		return nullptr;

	return iter->second->Get_Component(wstrObjTag, wstrComponentTag, eID);
}

const CGameObject * CScene::Get_GameObjcet(const wstring& wstrLayerTag, const wstring& wstrObjTag) const
{
	auto iter = m_mapLayer.find(wstrLayerTag);

	if (m_mapLayer.end() == iter)
		return nullptr;

	return iter->second->Get_GameObjcet(wstrObjTag);
}

const CLayer * CScene::Get_Layer(const wstring& wstrLayerTag) const
{
	auto iter = m_mapLayer.find(wstrLayerTag);

	if (m_mapLayer.end() == iter)
		return nullptr;

	return iter->second;
}



HRESULT Engine::CScene::Ready_Scene()
{
	//NULL_CHECK_RETURN(m_pCollisionMgr, E_FAIL);
	return S_OK;
}

_int Engine::CScene::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = 0;
	
	for (auto& iter : m_mapLayer)
	{
		iExit = iter.second->Update_Layer(fTimeDelta);
		if (iExit == 1)
			return 1;

		if (iExit & 0x80000000)
			return -1;
	}



	return 0;
}

_int CScene::LateUpdate_Scene(const _float & fTimeDelta)
{


	return _int();
}

void Engine::CScene::Render_Scene(const _float& fTimeDelta)
{
	//for (auto& iter : m_mapLayer)
	//	iter.second->Render_Layer();
}

HRESULT CScene::Add_Layer(const wstring& wstrLayerTag, CLayer * pLayer)
{
	auto iter = m_mapLayer.find(wstrLayerTag);

	if (m_mapLayer.end() != iter)
	{
		MSG_BOX("Add_Layer Failed");
		return E_FAIL;
	}

	m_mapLayer.emplace(wstrLayerTag, pLayer);

	return S_OK;
}

HRESULT CScene::Add_GameObject(const wstring& wstrLayerTag, const wstring& wstrObjTag, CGameObject* pObj)
{
	auto iter = m_mapLayer.find(wstrLayerTag);

	if (m_mapLayer.end() == iter)
	{
		MSG_BOX("Add_GameObject Failed. Layer does not exist");
		return E_FAIL;
	}

	iter->second->Add_GameObject(wstrObjTag, pObj);

	return S_OK;
}

void CScene::Die_GameObject(const wstring & wstrLayerTag, const wstring & wstrObjTag)
{
	auto iter = m_mapLayer.find(wstrLayerTag);

	if (m_mapLayer.end() == iter)
		return;

	iter->second->Die_Object(wstrObjTag);
}

void Engine::CScene::Free()
{
	//m_pCollisionMgr->DestroyInstance();

	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();

	Safe_Release(m_pGraphicDev);
}

