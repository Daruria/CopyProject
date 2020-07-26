#ifndef Subject_h__
#define Subject_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CSubject : public CBase
{
protected:
	explicit CSubject();
	virtual ~CSubject();

public:
	virtual void Update_Subject(int iMsg, void* pData);
	virtual void Get_Data(int iMsg, void* pData);

public:
	static CSubject* Create() { return new CSubject; }

protected:
	virtual void Free();
};

END


#endif // Subject_h__
