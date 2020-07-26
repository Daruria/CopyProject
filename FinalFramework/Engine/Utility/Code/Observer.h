#ifndef Observer_h__
#define Observer_h__

#include "Subject.h"

BEGIN(Engine)

class ENGINE_DLL CObserver : public CBase
{
protected:
	explicit CObserver();
	virtual ~CObserver();

public:
	virtual HRESULT Ready_Observer(const wstring& wstrSubjectKey);
	
	void Get_Data(int iMsg, void* pOut);

protected:
	CSubject*	m_pSubject = nullptr;

public:
	static CObserver* Create(const wstring& wstrSubjectKey);

protected:
	virtual void Free();
};

END

#endif // Observer_h__
