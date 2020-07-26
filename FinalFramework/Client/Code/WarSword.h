#ifndef WarSword_h__
#define WarSword_h__

#include "Weapon.h"

BEGIN(Engine)

class CCollider;

END

class CWarSword : public CWeapon
{
private:
	explicit CWarSword(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWarSword();

public:
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const wstring& wstrName) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(const _float& fTimeDelta) override;
	virtual void Attack();
	virtual void Stop();
	
public:
	virtual void Collision_Enter(CGameObject* pOpponent) override;
	virtual void Collision_Stay(CGameObject* pOpponent) override;
	virtual void Collision_Leave(CGameObject* pOpponent) override;

private:
	Engine::CCollider*	m_pColliderCom = nullptr;
	class CSwordTrail*	m_pSwordTrail = nullptr;

	_bool		m_bTrail = false;

protected:
	virtual HRESULT Add_Component(const _ushort& wContainerIdx, const wstring& wstrBufferTag, const _ulong& dwFlags = 0) override;

public:
	static CWarSword* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

#endif // WarSword_h__
