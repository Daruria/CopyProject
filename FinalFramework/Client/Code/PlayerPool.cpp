#include "stdafx.h"
#include "PlayerPool.h"
#include "Dummy.h"

IMP_SINGLETON(CPlayerPool)

CPlayerPool::CPlayerPool()
{
}

CPlayerPool::~CPlayerPool()
{
	Free();
}

HRESULT CPlayerPool::Ready_PlayerPool(LPDIRECT3DDEVICE9	pGraphicDev, const wstring& wstrDummyTag, 
									const wstring& wstrFilePath, const wstring& wstrFileName, 
									const _uint& iCount, const _ulong& dwFlags)
{
	auto iter = m_mapDummyList.find(wstrDummyTag);

	if (m_mapDummyList.end() != iter)
		return E_FAIL;

	list<pair<_bool, CDummy*>>* pList = new list<pair<_bool, CDummy *>>;

	for (_uint i = 0; i < iCount; ++i)
		pList->push_back(make_pair(false, CDummy::Create(pGraphicDev, wstrFilePath, wstrFileName, dwFlags)));

	m_mapDummyList.insert(make_pair(wstrDummyTag, pList));
	return S_OK;
}

HRESULT CPlayerPool::Get_Dummy(const wstring & wstrDummyTag, CDummy ** ppOutDummy)
{
	auto iter = m_mapDummyList.find(wstrDummyTag);

	if (m_mapDummyList.end() == iter)
		return E_FAIL;

	for (auto& DummyPair : *(iter->second))
	{
		if (!DummyPair.first)
		{
			*ppOutDummy = DummyPair.second;
			(*ppOutDummy)->AddRef();
			DummyPair.first = true;
			return S_OK;
		}
	}

	*ppOutDummy = nullptr;

	return S_OK;
}

_int CPlayerPool::LateUpdate_PlayerPool(const _float & fTimeDelta)
{
	for (auto MapPair : m_mapDummyList)
	{
		for (auto& DummyPair : *(MapPair.second))
		{
			if (DummyPair.second->IsDead())
			{
				DummyPair.first = false;
				DummyPair.second->Reset();
			}
		}
	}

	return 0;
}

void CPlayerPool::Free()
{
	for (auto MapPair : m_mapDummyList)
	{
		for (auto DummyPair : *MapPair.second)
			::Safe_Release(DummyPair.second);

		MapPair.second->clear();

		if (MapPair.second != nullptr)
		{
			delete MapPair.second;
			MapPair.second = nullptr;
		}
	}
	m_mapDummyList.clear();

}
