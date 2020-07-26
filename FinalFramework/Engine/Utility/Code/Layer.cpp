#include "Layer.h"

USING(Engine)

Engine::CLayer::CLayer()
{

}

Engine::CLayer::~CLayer()
{

}

const CComponent * CLayer::Get_Component(const wstring& wstrObjTag, const wstring& wstrComponentTag, COMPONENTID eID) const
{
	auto iter = m_mapObject.find(wstrObjTag);

	if (m_mapObject.end() == iter)
		return nullptr;

	return iter->second->Get_Component(wstrComponentTag, eID);
}

const CGameObject * CLayer::Get_GameObjcet(const wstring& wstrObjTag) const
{
	auto iter = m_mapObject.find(wstrObjTag);

	if (m_mapObject.end() == iter)
		return nullptr;

	return iter->second;
}

CGameObject * CLayer::Get_GameObject_NC(const wstring& wstrObjTag)
{
	auto iter = m_mapObject.find(wstrObjTag);

	if (m_mapObject.end() == iter)
		return nullptr;

	return iter->second;
}

void CLayer::All_Die()
{
	for (auto& iter : m_mapObject)
	{
		iter.second->Die();
	}
}

void CLayer::Die_Object(const wstring & wstrObjTag)
{
	auto iter = m_mapObject.find(wstrObjTag);

	if (iter == m_mapObject.end())
		return;

	iter->second->Die();
}

HRESULT Engine::CLayer::Ready_Layer()
{
	return S_OK;
}

_int Engine::CLayer::Update_Layer(const _float& fTimeDelta)
{
	_int iExit = 0;

	auto iter		= m_mapObject.begin();
	auto iterEnd	= m_mapObject.end();

	for (auto iter = m_mapObject.begin(); iter != m_mapObject.end(); )
	{
		iExit = iter->second->Update_GameObject(fTimeDelta);

		if (iExit & 0x80000000)
			return -1;

		if (iter->second->IsDead())
		{
			Safe_Release(iter->second);
			iter = m_mapObject.erase(iter);
		}
		else
			++iter;
	}

	return iExit;
}

_int CLayer::LateUpdate_Layer(const _float & fTimeDelta)
{
	_int iExit = 0;

	auto iter = m_mapObject.begin();
	auto iterEnd = m_mapObject.end();

	for (auto iter = m_mapObject.begin(); iter != m_mapObject.end(); )
	{
		iExit = iter->second->LateUpdate_GameObject(fTimeDelta);

		if (iExit & 0x80000000)
			return -1;

		if (iter->second->IsDead())
		{
			Safe_Release(iter->second);
			iter = m_mapObject.erase(iter);
		}
		else
			++iter;
	}

	return iExit;
}

void Engine::CLayer::Render_Layer(const _float& fTimeDelta)
{
	for (auto& iter : m_mapObject)
		iter.second->Render_GameObject(fTimeDelta);
}

HRESULT Engine::CLayer::Add_GameObject(const wstring& wstrObjTag, CGameObject* pGameObject)
{
	m_mapObject.emplace(wstrObjTag, pGameObject);

	return S_OK;
}

Engine::CLayer* Engine::CLayer::Create()
{
	CLayer* pInst = new CLayer;

	if (FAILED(pInst->Ready_Layer()))
		Safe_Release(pInst);

	return pInst;
}

void Engine::CLayer::Free()
{
	for_each(m_mapObject.begin(), m_mapObject.end(), CDeleteMap());
	m_mapObject.clear();
}

