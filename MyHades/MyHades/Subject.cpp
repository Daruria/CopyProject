#include "stdafx.h"
#include "Subject.h"
#include "Observer.h"

CSubject::CSubject()
{
}

CSubject::~CSubject()
{
	m_ObserverList.clear();
}

void CSubject::Notify(int iMsg, void * pData)
{
	for (auto pObserver : m_ObserverList)
		pObserver->Update(iMsg, pData);
}

void CSubject::Subscribe(CObserver * pObserver)
{
	NULL_CHECK(pObserver);

	m_ObserverList.push_back(pObserver);
}

void CSubject::Unsubscribe(CObserver * pObserver)
{
	NULL_CHECK(pObserver);

	auto iter = find(m_ObserverList.begin(), m_ObserverList.end(), pObserver);

	if (m_ObserverList.end() == iter)
		return;

	m_ObserverList.erase(iter);
}