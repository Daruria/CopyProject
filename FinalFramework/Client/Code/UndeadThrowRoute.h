#ifndef UndeadThrowRoute_h__
#define UndeadThrowRoute_h__

#include "Defines.h"
#include "MoveOnRoute.h"

BEGIN(Engine)

class CTransform;

END

class CUndeadThrowRoute : public Engine::CMoveOnRoute
{
protected:
	explicit CUndeadThrowRoute();
	virtual ~CUndeadThrowRoute();

public:
	HRESULT Ready_Component(Engine::CTransform* pTransform, const _vec3* pStartPos, const _vec3* pCenterPos, const _vec3* pMoveDir, const _float& fSpeed, const _float& fAngularSpeed);
	virtual _int Update_Component(const _float& fTimeDelta) override;

private:
	Engine::CTransform* m_pTransform = nullptr;
	_vec3				m_vCenterPos = {};
	_vec3				m_vAxis = {};
	_vec3				m_vOutDir = {};
	_vec3				m_vMoveDir;
	_float				m_fRadius = 0.f;
	_float				m_fSpeed = 0.1f;
	_float				m_fAngularSpeed = 0.1f;
	_float				m_fAngle = 0.f;

public:
	static CUndeadThrowRoute*	Create(Engine::CTransform* pTransform, const _vec3* pStartPos, const _vec3* pCenterPos, const _vec3* pMoveDir, const _float& fSpeed, const _float& fAngularSpeed);

protected:
	virtual void Free() override;

};


#endif // UndeadThrowRoute_h__
