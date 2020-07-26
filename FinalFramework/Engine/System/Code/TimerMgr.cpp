#include "TimerMgr.h"

USING(Engine)

IMPLEMENT_SINGLETON(CTimerMgr)

Engine::CTimerMgr::CTimerMgr()
{

}

Engine::CTimerMgr::~CTimerMgr()
{
	Free();
}

void Engine::CTimerMgr::Set_TimeDelta(const wstring& wstrTimerTag)
{
	CTimer* pInst = Find_Timer(wstrTimerTag);

	if (nullptr == pInst)
		return;

	pInst->Set_TimeDelta();
}

_float Engine::CTimerMgr::Get_TimeDelta(const wstring& wstrTimerTag)
{
	CTimer*		pInst = Find_Timer(wstrTimerTag);

	if (nullptr == pInst)
		return 0.f;

	return pInst->Get_TimeDelta();
}

_float CTimerMgr::Get_CummulativeTime(const wstring& wstrTimerTag)
{
	CTimer*		pInst = Find_Timer(wstrTimerTag);

	if (nullptr == pInst)
		return 0.f;

	return pInst->Get_CumulativeTime();
}

HRESULT Engine::CTimerMgr::Ready_Timer(const wstring& wstrTimerTag)
{
	CTimer*		pInst = Find_Timer(wstrTimerTag);

	if (nullptr != pInst)
		return E_FAIL;

	pInst = CTimer::Create();
	NULL_CHECK_RETURN(pInst, E_FAIL);

	m_mapTimer.emplace(wstrTimerTag, pInst);

	return S_OK;
}

void Engine::CTimerMgr::Free()
{
	for_each(m_mapTimer.begin(), m_mapTimer.end(), CDeleteMap());
	m_mapTimer.clear();
}

Engine::CTimer* Engine::CTimerMgr::Find_Timer(const wstring& wstrTimerTag)
{
	auto iter = m_mapTimer.find(wstrTimerTag);

	if (iter == m_mapTimer.end())
		return nullptr;

	return iter->second;
}