#ifndef InvisibleWeapon_h__
#define InvisibleWeapon_h__

#include "Weapon.h"

BEGIN(Engine)

class CCollider;

END

class CInvisibleWeapon : public CWeapon
{
private:
	explicit CInvisibleWeapon(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CInvisibleWeapon();

public:
	virtual HRESULT Ready_GameObject() override;
	HRESULT		Ready_GameObject(const _float& fRadius);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;
	virtual void Attack();
	virtual void Stop();
	
private:
	Engine::CCollider*	m_pColliderCom = nullptr;

protected:
	HRESULT Add_Component(const _float& fRadius, const _ulong& dwFlags = 0);

public:
	static CInvisibleWeapon* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fRadius);

private:
	virtual void Free() override;
};

#endif // InvisibleWeapon_h__
