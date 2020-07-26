#ifndef MoveOnRoute_h__
#define MoveOnRoute_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CMoveOnRoute : public CComponent
{
protected:
	explicit	CMoveOnRoute();
	virtual		~CMoveOnRoute();

public:
	virtual HRESULT		Ready_Component();
	virtual _int		Update_Component(const _float& fTimeDelta);

public:
	const _bool&	IsDead() { return m_bIsDead; }
protected:
	_bool	m_bIsDead = false;

protected:
	virtual void Free() override;

};

END

#endif // MoveOnRoute_h__
