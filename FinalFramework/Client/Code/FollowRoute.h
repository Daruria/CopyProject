#ifndef FollowRoute_h__
#define FollowRoute_h__

#include "Defines.h"
#include "MoveOnRoute.h"

BEGIN(Engine)

class CTransform;
class CGameObject;

END

class CFollowRoute : public Engine::CMoveOnRoute
{
protected:
	explicit CFollowRoute();
	virtual ~CFollowRoute();

public:
	HRESULT Ready_Component(Engine::CTransform* pTransform, Engine::CGameObject* pParent);
	virtual _int Update_Component(const _float& fTimeDelta) override;

private:
	Engine::CTransform* m_pTransform = nullptr;
	Engine::CGameObject*		m_pParent = nullptr;

public:
	static CFollowRoute*	Create(Engine::CTransform* pTransform, Engine::CGameObject* pParent);

protected:
	virtual void Free() override;

};


#endif // FollowRoute_h__
