#include "Observer.h"
#include "Export_Function.h"

USING(Engine)

CObserver::CObserver()
{
}

CObserver::~CObserver()
{
}

HRESULT CObserver::Ready_Observer(const wstring& wstrSubjectKey)
{
	m_pSubject = Get_Subject(wstrSubjectKey);
	NULL_CHECK_RETURN(m_pSubject, E_FAIL);
	m_pSubject->AddRef();

	return S_OK;
}

void CObserver::Get_Data(int iMsg, void * pOut)
{
	m_pSubject->Get_Data(iMsg, pOut);
}

CObserver * CObserver::Create(const wstring & wstrSubjectKey)
{
	CObserver* pInst = new CObserver;

	if (FAILED(pInst->Ready_Observer(wstrSubjectKey)))
		Safe_Release(pInst);

	return pInst;
}

void CObserver::Free()
{
	Safe_Release(m_pSubject);
}
