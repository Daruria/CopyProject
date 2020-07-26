#ifndef FollowGlowRoute_h__
#define FollowGlowRoute_h__

#include "Defines.h"
#include "MoveOnRoute.h"

BEGIN(Engine)

class CTransform;
class CGameObject;

END

class CFollowGlowRoute : public Engine::CMoveOnRoute
{
protected:
	explicit CFollowGlowRoute();
	virtual ~CFollowGlowRoute();

public:
	HRESULT Ready_Component(Engine::CTransform* pTransform, Engine::CGameObject* pParent);
	virtual _int Update_Component(const _float& fTimeDelta) override;

	void	Set_Scaling(const _float& fMinScale, const _float& fMaxScale, const _float& fSpeed);

private:
	Engine::CTransform*		m_pTransform = nullptr;
	Engine::CGameObject*	m_pParent = nullptr;

	_float	m_fSpeed		= 0.f;
	_float	m_fScale		= 0.f;
	_float	m_fAmp			= 0.f;

	_float	m_fScalingTime	= 0.f;

public:
	static CFollowGlowRoute*	Create(Engine::CTransform* pTransform, Engine::CGameObject* pParent);

protected:
	virtual void Free() override;

};


#endif // FollowGlowRoute_h__
