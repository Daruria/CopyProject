#ifndef Component_h__
#define Component_h__

#include "Base.h"
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CComponent : public CBase
{
protected:
	explicit	CComponent();
	virtual		~CComponent();

public:
	virtual _int	Update_Component(const _float& fTimeDelta) { return 0; }

public:
	virtual CComponent* Clone(void) { return nullptr; }

protected:
	virtual void Free() = 0;

};

END

#endif // Component_h__
