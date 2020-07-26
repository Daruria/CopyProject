#ifndef SubjectMgr_h__
#define SubjectMgr_h__

#include "Subject.h"

BEGIN(Engine)

class ENGINE_DLL CSubjectMgr : CBase
{
	DECLARE_SINGLETON(CSubjectMgr)

private:
	explicit CSubjectMgr();
	virtual ~CSubjectMgr();

public:
	HRESULT Add_Subject(const wstring& wstrSubjectKey, CSubject* pSubject);
	HRESULT Remove_Subject(const wstring& wstrSubjectKey);
	HRESULT Clear_Subject();

	CSubject* Get_Subject(const wstring& wstrSubjectKey);

	void	Update_Subject(const wstring& wstrSubjectKey, int iMsg, void* pData);
	void	Get_Data(const wstring& wstrSubjectKey, int iMsg, void* pOut);

private:
	map<wstring, CSubject*> m_mapSubject;

private:
	virtual void Free();
};

END

#endif // SubjectMgr_h__
