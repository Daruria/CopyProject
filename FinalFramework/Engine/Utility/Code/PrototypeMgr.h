#ifndef PrototypeMgr_h__
#define PrototypeMgr_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CPrototypeMgr : public CBase
{
	DECLARE_SINGLETON(CPrototypeMgr)

private:
	explicit	CPrototypeMgr(void);
	virtual		~CPrototypeMgr(void);

public:
	HRESULT			Ready_Prototype(const wstring& wstrProtoTag, CComponent* pInstance);
	CComponent*		Clone_Prototype(const wstring& wstrProtoTag);

private:
	CComponent*		Find_Prototype(const wstring& wstrProtoTag);

private:
	map<wstring, CComponent*>			m_mapPrototype;
public:
	virtual void	Free(void);
};

END
#endif // PrototypeMgr_h__
