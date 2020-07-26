#include "PrototypeMgr.h"


USING(Engine)
IMPLEMENT_SINGLETON(CPrototypeMgr)

Engine::CPrototypeMgr::CPrototypeMgr(void)
{

}

Engine::CPrototypeMgr::~CPrototypeMgr(void)
{
	Free();
}

HRESULT Engine::CPrototypeMgr::Ready_Prototype(const wstring& wstrProtoTag, CComponent* pInstance)
{
	CComponent*	pPrototype = Find_Prototype(wstrProtoTag);

	if (nullptr != pPrototype)
		return E_FAIL;

	m_mapPrototype.emplace(wstrProtoTag, pInstance);

	return S_OK;
}

Engine::CComponent* Engine::CPrototypeMgr::Clone_Prototype(const wstring& wstrProtoTag)
{
	CComponent*		pPrototype = Find_Prototype(wstrProtoTag);

	if (nullptr == pPrototype)
		return nullptr;

	return pPrototype->Clone();
}

Engine::CComponent* Engine::CPrototypeMgr::Find_Prototype(const wstring& wstrProtoTag)
{
	auto	iter = m_mapPrototype.find(wstrProtoTag);

	if (iter == m_mapPrototype.end())
		return nullptr;

	return iter->second;
}

void Engine::CPrototypeMgr::Free(void)
{
	for_each(m_mapPrototype.begin(), m_mapPrototype.end(), CDeleteMap());
	m_mapPrototype.clear();
}

