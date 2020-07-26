#include "SubjectMgr.h"

USING(Engine)

IMPLEMENT_SINGLETON(CSubjectMgr)

CSubjectMgr::CSubjectMgr()
{
}

CSubjectMgr::~CSubjectMgr()
{
	Free();
}

HRESULT CSubjectMgr::Add_Subject(const wstring & wstrSubjectKey, CSubject * pSubject)
{
	NULL_CHECK_RETURN(pSubject, E_FAIL);

	auto iter = m_mapSubject.find(wstrSubjectKey);

	if (m_mapSubject.end() != iter)
		return E_FAIL;

	m_mapSubject.insert(make_pair(wstrSubjectKey, pSubject));

	return S_OK;
}

HRESULT CSubjectMgr::Remove_Subject(const wstring & wstrSubjectKey)
{
	auto iter = m_mapSubject.find(wstrSubjectKey);

	if (m_mapSubject.end() == iter)
		return E_FAIL;

	Safe_Release(iter->second);
	m_mapSubject.erase(iter);

	return S_OK;
}

HRESULT CSubjectMgr::Clear_Subject()
{
	for_each(m_mapSubject.begin(), m_mapSubject.end(), CDeleteMap());
	m_mapSubject.clear();

	return S_OK;
}

CSubject * CSubjectMgr::Get_Subject(const wstring & wstrSubjectKey)
{
	auto iter = m_mapSubject.find(wstrSubjectKey);

	if (m_mapSubject.end() == iter)
		return nullptr;

	return iter->second;
}

void CSubjectMgr::Update_Subject(const wstring & wstrSubjectKey, int iMsg, void * pData)
{
	auto iter = m_mapSubject.find(wstrSubjectKey);

	if (m_mapSubject.end() == iter)
		return;

	iter->second->Update_Subject(iMsg, pData);
}

void CSubjectMgr::Get_Data(const wstring & wstrSubjectKey, int iMsg, void * pOut)
{
	auto iter = m_mapSubject.find(wstrSubjectKey);

	if (m_mapSubject.end() == iter)
		return;

	iter->second->Get_Data(iMsg, pOut);
}

void CSubjectMgr::Free()
{
	for_each(m_mapSubject.begin(), m_mapSubject.end(), CDeleteMap());
	m_mapSubject.clear();
}
