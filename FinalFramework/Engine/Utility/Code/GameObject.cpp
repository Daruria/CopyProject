#include "GameObject.h"

USING(Engine)

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CGameObject::~CGameObject()
{

}

const CComponent * CGameObject::Get_Component(const wstring& wstrComponentTag, COMPONENTID eID) const
{
	auto iter = m_mapComponent[eID].find(wstrComponentTag);

	if (m_mapComponent[eID].end() == iter)
		return nullptr;

	return iter->second;
}

CComponent * CGameObject::Get_Component(COMPONENTID eID, const wstring & wstrComponentTag)
{
	auto iter = m_mapComponent[eID].find(wstrComponentTag);

	if (m_mapComponent[eID].end() == iter)
		return nullptr;

	return iter->second;
}

HRESULT Engine::CGameObject::Ready_GameObject()
{
	return S_OK;
}

HRESULT CGameObject::Ready_GameObject(const _ushort & wContainerIdx, const wstring & wstrBufferTag, const wstring & wstrName)
{
	return S_OK;
}

_int Engine::CGameObject::Update_GameObject(const _float& fTimeDelta)
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->Update_Component(fTimeDelta);

	return 0;
}

_int CGameObject::LateUpdate_GameObject(const _float & fTimeDelta)
{
	return 0;
}

void Engine::CGameObject::Render_GameObject(const _float& fTimeDelta)
{

}

void CGameObject::Render_GameObject(const _float & fTimeDelta, const _uint & iIndex)
{

}

HRESULT CGameObject::Add_Component(const _ushort & wContainerIdx, const wstring & wstrBufferTag, const _ulong& dwFlags)
{
	return S_OK;
}

void Engine::CGameObject::Free()
{
	for (_int i = 0; i < ID_END; ++i)
	{
		//for (auto& Pair : m_mapComponent[i])
		//	Safe_Release(Pair.second);

		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());

		m_mapComponent[i].clear();
	}

	Safe_Release(m_pGraphicDev);
}

