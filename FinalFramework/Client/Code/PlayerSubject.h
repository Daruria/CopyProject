#ifndef PlayerSubject_h__
#define PlayerSubject_h__

#include "Defines.h"
#include "Subject.h"

BEGIN(Engine)

class CTransform;

END

class CPlayerSubject : public Engine::CSubject
{
private:
	explicit CPlayerSubject();
	virtual ~CPlayerSubject();

private:
	HRESULT Ready_Subject();

public:
	void Update_Subject(int iMsg, void* pData);
	void Get_Data(int iMsg, void* pData);

private:
	Engine::CTransform*	m_pTransform = nullptr;
	_int				m_iHP = 0;
	_int				m_iHPMax = 0;
	_ulong				m_dwNaviIndex = 0;

public:
	static CPlayerSubject* Create();

private:
	virtual void Free() override;
};

#endif // PlayerSubject_h__
