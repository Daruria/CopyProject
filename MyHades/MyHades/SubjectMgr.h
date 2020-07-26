#pragma once
#include "Subject.h"

class CSubjectMgr
{

	DECLARE_SINGLETON(CSubjectMgr)

private:
	CSubjectMgr();
	~CSubjectMgr();

public:
	HRESULT AddSubject(const wstring& wstrSubjectKey, CSubject* pSubject);
	void Notify(const wstring& wstrSubjectKey, int iMsg, void* pData);
	void Subscribe(const wstring& wstrSubjectKey, CObserver* pObserver);
	void Unsubscribe(const wstring& wstrSubjectKey, CObserver* pObserver);

private:
	void Release();

private:
	unordered_map<wstring, CSubject*> m_umapSubject;
};

