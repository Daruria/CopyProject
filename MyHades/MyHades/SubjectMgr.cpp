#include "stdafx.h"
#include "SubjectMgr.h"


IMPLEMENT_SINGLETON(CSubjectMgr)

CSubjectMgr::CSubjectMgr()
{
}

CSubjectMgr::~CSubjectMgr()
{
	Release();
}

HRESULT CSubjectMgr::AddSubject(const wstring & wstrSubjectKey, CSubject * pSubject)
{
	NULL_CHECK_RETURN(pSubject, E_FAIL);
	
	auto iter = m_umapSubject.find(wstrSubjectKey);

	if (m_umapSubject.end() != iter)
		return E_FAIL;

	m_umapSubject.insert(make_pair(wstrSubjectKey, pSubject));

	return S_OK;
}

void CSubjectMgr::Notify(const wstring & wstrSubjectKey, int iMsg, void * pData)
{
	NULL_CHECK(pData);

	auto iter = m_umapSubject.find(wstrSubjectKey);

	if (m_umapSubject.end() == iter)
		return;

	iter->second->Notify(iMsg, pData);
}

void CSubjectMgr::Subscribe(const wstring & wstrSubjectKey, CObserver * pObserver)
{
	NULL_CHECK(pObserver);

	auto iter = m_umapSubject.find(wstrSubjectKey);

	if (m_umapSubject.end() == iter)
		return;

	iter->second->Subscribe(pObserver);
}

void CSubjectMgr::Unsubscribe(const wstring & wstrSubjectKey, CObserver * pObserver)
{
	NULL_CHECK(pObserver);
	
	auto iter = m_umapSubject.find(wstrSubjectKey);

	if (m_umapSubject.end() == iter)
		return;

	iter->second->Unsubscribe(pObserver);
}

void CSubjectMgr::Release()
{
	for (auto Pair : m_umapSubject)
		SafeDelete(Pair.second);

	m_umapSubject.clear();
}
