#ifndef ThrowSpear_h__
#define ThrowSpear_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CCollider;

END

class CThrowSpear : public Engine::CGameObject
{
private : 
	explicit CThrowSpear(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CThrowSpear();

public:
	HRESULT Ready_GameObject(const _vec3* pStartPos, const _vec3* pMoveDir, const _vec3* pAngle, const _float& fSpeed, const _float& fHeight);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(const _float& fTimeDelta);

	void Set_LifeTime(const _float& fLifeTime) { m_fLifeTime = fLifeTime; }

private:
	HRESULT Add_Component();



private:
	Engine::CTransform*			m_pTransformCom = nullptr;
	Engine::CCollider*			m_pColliderCom	= nullptr;

	_float	m_fLifeTime = 0.f;
	_float	m_fLifeCnt = 0.f;

	_float	m_fFxItvalTime = 0.f;

	_vec3	m_vMoveDir = {};
	_vec3	m_vAngle = {};

	_float	m_fSpeed = 10.f;
	_float	m_fHeight = 0.f;

public:
	static CThrowSpear* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pStartPos, const _vec3* pMoveDir, const _vec3* pAngle, const _float& fSpeed, const _float& fHeight);

private:
	virtual void Free() override;
};

#endif // ThrowSpear_h__
