#ifndef Trail_h__
#define Trail_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTrail : public CComponent
{
protected:
	explicit CTrail();
	explicit CTrail(const CTrail& rhs);
	virtual ~CTrail();

public:
	HRESULT		Ready_Component();
	virtual _int Update_Component(const _float& fTimeDelta) override;

public:
	virtual CTrail* Create();
	virtual CComponent* Clone(void) override;

protected:
	virtual void Free() override;

};

END

#endif // Trail_h__
