#ifndef ChargeTrailRoute_h__
#define ChargeTrailRoute_h__

#include "Defines.h"
#include "MoveOnRoute.h"

BEGIN(Engine)

class CTransform;

END

class CChargeTrailRoute : public Engine::CMoveOnRoute
{
protected:
	explicit CChargeTrailRoute();
	virtual ~CChargeTrailRoute();

public:
	HRESULT Ready_Component(Engine::CTransform* pTransform, const _vec3* pStartPos, const _vec3* pDestPos, const _float& fSpeed, const _float& fAngularSpeed);
	virtual _int Update_Component(const _float& fTimeDelta) override;

private:
	Engine::CTransform* m_pTransform = nullptr;
	_vec3				m_vDestPos = {};
	_vec3				m_vAxis = {};
	_vec3				m_vDir = {};
	_float				m_fRadius = 0.f;
	_float				m_fSpeed = 0.1f;
	_float				m_fAngularSpeed = 0.1f;
	_float				m_fAngle = 0.f;

public:
	static CChargeTrailRoute*	Create(Engine::CTransform* pTransform, const _vec3* pStartPos, const _vec3* pDestPos, const _float& fSpeed, const _float& fAngularSpeed);

protected:
	virtual void Free() override;

};


#endif // ChargeTrailRoute_h__
