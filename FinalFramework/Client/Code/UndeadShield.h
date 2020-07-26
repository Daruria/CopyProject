#ifndef GUndeadShield_h__
#define GUndeadShield_h__

#include "Weapon.h"

BEGIN(Engine)

class CCollider;

END

class CUndeadShield : public CWeapon
{
private:
	explicit CUndeadShield(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUndeadShield();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;
	virtual void Attack();
	virtual void Stop();

private:
	Engine::CCollider*	m_pColliderCom = nullptr;

protected:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags = 0) override;

public:
	static CUndeadShield* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

#endif // GUndeadShield_h__
